
#ifndef _PUZZLE_CLI_
#define _PUZZLE_CLI_

//[handle console maipulation]
#include <windows.h>  
#include <iostream>
#include <conio.h>
#include <cstdio>
#include <iomanip>

#include <vector>//vector STL container
#include <ctime>//for seeding (seed) seed with the same time over and over
#include <cstdlib>//random functions
#include <algorithm>
#include <map>

/* ANSI.SYS screen control #defines follow: */
#define _BOLD "\x1B[1m"
#define _NORMAL "\x1B[0m"
#define _RED "\x1B[31m"
#define _BLACK "\x1B[30m"
#define _GREEN "\x1B[32m"
#define _CLEAR_SCREEN "\x1B[2J"
#define _CLEAR_EOL "\x1B[K"
#define _MOVE_CURSOR "\x1B[%d;%df"


//[NAVIGATION CONTROL]

//for keyboard arrow keys
#define UP    '\x48'  //H
#define DOWN  '\x50'//P
#define LEFT  '\x4B'//K
#define RIGHT '\x4D'//M


//further definitions for navigation(ie W-A-S-D)

// * - Uppercase *
//*l - Lowercase *

//extended arrow keys:: WASD


#define UPx    'W' //H
#define DOWNx  'S'//P
#define LEFTx  'A'//K
#define RIGHTx 'D'//M

//extended lowercase:: wasd

#define UPxl    'w' //H
#define DOWNxl  's'//P
#define LEFTxl  'a'//K
#define RIGHTxl 'd'//M

//escape
#define ESC 27

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#define _LOCATE(z,s)  SetConsoleCursorPosition((HANDLE)hConsole, {s,z})//depreceated
#define _GOTOXY(a,b)  LOCATE(b, a)//depreciated

//overloading functions to also take a COORD
void LOCATE(int z, int s){//form (y, x)
     COORD pos={short(s),short(z)};//COORD defined in windows.h is struct
     SetConsoleCursorPosition((HANDLE)hConsole, pos);
}
void LOCATE(COORD pos){//form (y, x)
     SetConsoleCursorPosition((HANDLE)hConsole, pos);
}

void GOTOXY(int a, int b){//locate(y,x) === gotoxy(x,y)
	LOCATE(b, a);
}

//case 3: self-defined nb: range(1-256)  for foreground colors(1-15) for background colors(16-255)
#define BLACK  0
#define DEEPBLUE  1
#define BLUE DEEPBLUE
#define GREEN  2
#define SKYBLUE 3
#define AQUA SKYBLUE
#define DARKRED  4
#define PURPLE  5
#define OLIVE  6
#define WHITE  7
#define DEFAULT WHITE
#define NORMAL DEFAULT
#define SILVER 8
#define GRAY  SILVER
#define LIGHTBLUE 9
#define SHINYGREEN  10
#define SHINYSKYBLUE 11
#define LIGHTAQUA SHINYSKYBLUE
#define SHINYRED  12
#define RED SHINYRED
#define SHINYPURPLE  13
#define VIOLET  SHINYPURPLE
#define SHINYYELLOW   14
#define YELLOW  SHINYYELLOW
#define SHINYWHITE  15
HANDLE hstdout=GetStdHandle(STD_OUTPUT_HANDLE);

/*TO GET A PARTICULAR COLOR say A ON A PARTICULAR BACKGROUND say B
YOU HAVE GET AS
 A+B+(15*B) + 1*/

//TEXTCOLOR
#define TEXTCOLOR(color)  SetConsoleTextAttribute(hstdout, color)//a is the color
//textcolor with background
#define  set2(p,q) (p+(q<<4))//(p+(16*q))
#define  BTEXTCOLOR(fg,bg)  SetConsoleTextAttribute(hstdout, set2(fg,bg))//color(a) on bg (b)

void clear_screen(){
     system("color");
    system("cls");
}


#endif

