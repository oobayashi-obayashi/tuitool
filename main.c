#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "headers/winfunc.h"

#define STRING_SIZE 256


//*************************************************
//when this function was runned,this function set flame and view WINDOW structure
int set_window_frame(
    int my,     int mx,     int cx,         int cy,
    WINDOW **view_f,        WINDOW **view, 
    WINDOW **workview_f,    WINDOW **workview,
    WINDOW **logview_f,     WINDOW **logview
   ) ;
//*************************************************

int main(int argc,char *argv[]){

   initscr();
    noecho();
    cbreak();
    keypad(stdscr,TRUE);
    curs_set(0);
    int x = 0;   int y = 0;
    int mx,my;   

    getmaxyx(stdscr,my,mx);
    int cy,cx;
    cy = my /2;cx = mx/2;
    refresh();

    WINDOW *view_f;       
    WINDOW *view;         
    WINDOW *workview_f; 
    WINDOW *workview;   
    WINDOW *logview_f; 
    WINDOW *logview;  
    int view_flame_flag =  set_window_frame(my,mx,cx,cy,&view_f,&view,&workview_f,&workview,&logview_f,&logview);
    scrollok(logview, TRUE);
    int temp_length = sizeof(argv[1])/sizeof(argv[1][0]);

    if(view_flame_flag != 0) {
    printf("set_window_flame was failed");
    return 0;
    }

    wrefresh(logview);

    int opkey = getch();
    int situation_flag = 0;
    while(situation_flag == 0){
opkey = getch();
    switch(opkey){
    case 116:
    {
    system("tree --charset=ascii -a  ./ > temp.txt");
    file_reader(view,"temp.txt"); // スクロール機能付きの関数
    mvwprintw(logview,7,0,"tree  -a > temp.txt\n");
    wrefresh(logview);
    break;
    }

    case 108:
    {
    system("ls -l -a > temp.txt");
    file_reader(view,"temp.txt"); // スクロール機能付きの関数
    mvwprintw(logview,7,0,"ls -l -a> temp.txt\n");
    wrefresh(logview);
    break;
    }
    case 115:
    {
    char command[sizeof(argv[1])/sizeof(argv[1][0]) + 16];
    strcpy(command,"strings ");
    strcat(command,argv[1]);
    strcat(command," > temp.txt");
    system(command);
    file_reader(view,"temp.txt"); // スクロール機能付きの関数
    mvwprintw(logview,7,0,"%s\n",command);
    wrefresh(logview);
    break;
    }

    case 81:
    situation_flag = 1;
    break;

    dedfine:
    break;
}
flushinp();
}







wprintw(workview,"enter to close");
wrefresh(workview);
    flushinp();
    getch();
    endwin();
    return 0;
}


int set_window_frame(
int my,int mx,int cx,int cy,
WINDOW **view_f,WINDOW **view, 
WINDOW **workview_f,WINDOW **workview,
WINDOW **logview_f,WINDOW **logview){
     *view_f   = newwin(my,mx-cx,0,cx);
     *view   = newwin(my-2,mx-cx-2,1,cx+1);
     *workview_f = newwin(my-10,  cx,0,0);
     *workview   = newwin(my-12,cx-2,1,1);
     *logview_f   = newwin(10,  cx,   my-10,0);
     *logview     = newwin(8,cx-2,   my-9,1);

    box(*workview_f,0,0);
    box(*view_f,0,0);
    box(*logview_f,0,0);
    wrefresh(*view_f);
    wrefresh(*workview_f);
    wrefresh(*logview_f);
    
    return 0;
}










