#include<conio.h>
#include<iostream.h>
#include<dos.h>
#include<stdlib.h>
#define STOP 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

int gameOver;
 const int width=20;
 const int height=20;
 int x,y,fruitX,fruitY,score;
 int tailX[100],tailY[100];
 int nTail;
 int dir;

 void setup(){
 gameOver = 0;//false
 dir = STOP;
 x=width/2;
 y=height/2;
 fruitX = rand() % width;
 fruitY = rand() % height;
 score = 0;
 }