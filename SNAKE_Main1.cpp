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

 void Draw(){
 system("cls");
 for(int i=0; i<width+2;i++)
   cout<<'#';
cout<<endl;
for(i=0;i<height;i++){
  for(int j=0;j<width;j++){
    if(j==0)
      cout<<"#";
    if(i==y && j==x)
      cout<<"0";
    else if(i==fruitY && j==fruitX)
      cout<<"*";
    else{
     int print = 0;
     for(int k=0;k<nTail;k++){
       if(tailX[k] == j && tailY[k] == i){
	 cout<<"o";
	 print = 1;
       }
     }
     if(!print)
       cout<<" ";
    }
    if(j == width-1)
      cout<<"#";
  }
  cout<<endl;
}
for(i=0;i<width+2;i++)
  cout<<"#";
cout<<endl;
cout<<"Score: "<<score<<endl;
 }
void Input(){
  if(kbhit()){
    switch(getch()){
    case 'a':
      dir = LEFT;
      break;
    case 'd':
      dir = RIGHT;
      break;
    case 'w':
      dir = UP;
      break;
    case 's':
      dir = DOWN;
      break;
    case 'x':
      gameOver = 1;
      break;
    }
  }
}

void logic(){
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;
  for(int i=1;i<nTail;i++){
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }
  switch (dir){
  case LEFT:
    x--;
    break;
  case RIGHT:
    x++;
    break;
  case UP:
    y--;
    break;
  case DOWN:
    y++;
    break;
  default:
    break;
  }
  //collision with wall
  if(x>=width || x<0 || y>=height||y<0)
    gameOver=1;
  //collision with tail
  for(i=0;i<nTail;i++){
    if(tailX[i]==x && tailY[i]==y)
      gameOver=1;      }
  //eating fruit
  if(x==fruitX && y==fruitY){
  score +=10;
  fruitX = rand() % width;
  fruitY = rand() % height;
  nTail++;
  }
}
int main(){
clrscr();
setup();
while(!gameOver){
  Draw();
  Input();
  logic();
  delay(150);// controls speed
}
cout<<"\nGame Over! Final Score = "<<score<<endl;
getch();
return 0;
}