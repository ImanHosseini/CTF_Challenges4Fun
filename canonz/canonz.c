#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define W 60
#define H 30
#define X_TARGET 59
#define H_TARGET 4
#define dt 0.04f
float V = 6.7f;
char map[W*H];

void catflag() {
    FILE *fp;
    char c;
	
    fp = fopen("flag.txt", "r");

    while((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            addch(c); // print '\n', as it is part of the line
            break;
        }
        addch(c);
    }
 
    fclose(fp);
}

void fill1(){
	for(int i=0;i<W*H;i++){
	if(i%2==0) map[i] = 'o';
	else map[i]='.';	
	}
}

void fill2(){
	for(int i=0;i<W*H;i++){
	if(i%2==0) map[i] = '|';
	else map[i]='#';	
	}
}

void fill_zero(){
	for(int i=0;i<W*H;i++){
	map[i]='.';	
	}
	map[H_TARGET*W+X_TARGET]='X';
}

void display(){
	clear();
	for(int i=0;i<H;i++){
	char dst[W+1];
	strncpy(dst, map+i*W, W);
	dst[W] = 0;
	printw("%s\n",dst);
	}
	refresh();
}

void play(){

    struct gdata {
	char name[10];
        float g;
  
    } gdata;
	struct gdata data= {.name = "iman",.g = -1.0f};

	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 40000000L;
 

	printw("Hello! I'm Iman, welcome to Canonz. The game is simple: hit the X spot.\nFirst, give me your name:");
	refresh();
	scanw("%s",data.name);
	printw("hello %s!\n",data.name);
	refresh();
	getch();
// Main Loop
	float g = data.g;
	while(true){
	float x=0.0f,vx=0.0f,y=0.0f,vy=0.0f;
	float angle = 0.0f;
	// CHOOSE ANGLE
	printw("CHOOSE AN ANGLE:");
	scanw("%f",&angle);
	if(angle>0.0f && angle<90.0f){
	vx = 8.0f*cos(angle*0.01745f);
	vy = 8.0f*sin(angle*0.01745f);
	}
	while(true){
	// SETUP MAP
	fill_zero();
	int xd = (int) x;
	int yd = (int) y;
	if(W*(H-yd-1)+xd == H_TARGET*W+X_TARGET){
		printw("YOU WIN!\n");
		catflag();
		refresh();
		getch();
		return;
	}
	if(xd>-1 & xd<W & yd>-1 & yd<H){
	// H - yd - 1, xd
	map[W*(H-yd-1)+xd] = 'o';
	}else{
		break;	
	}
	// PRINT
	display();
	// PHYSICS
	nanosleep(&tim,&tim2);
	y += vy*dt;
	x += vx*dt;
	vy += dt*g;
	
	}
printw("TRY AGAIN. %f \n",g);
	refresh();
	}
	
	getch();
}

int main()
{	
	initscr();			/* Start curses mode 		  */
	printw("*** CANONZ ***\n");	/* Print Hello World		  */
	// catflag();
	refresh();			/* Print it on to the real screen */
	// getch();			/* Wait for user input */
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 300000000L;
	while(true){
	play();
	fill1();
	display();
	nanosleep(&tim,&tim2);
	fill2();
	display();
	nanosleep(&tim,&tim2);
	}
	getch();
	endwin();			/* End curses mode		  */

	return 0;
}
