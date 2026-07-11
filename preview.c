#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#define STRING_SIZE 256


int file_player(WINDOW *WIN_P,char* filename);

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

    WINDOW *view_f;       view_f   = newwin(my,mx-cx,0,cx);
    WINDOW *view;           view   = newwin(my-2,mx-cx-2,1,cx+1);
    WINDOW *workview_f; workview_f = newwin(my,cx,0,0);
    WINDOW *workview;   workview   = newwin(my-2,cx-2,1,1);

    box(workview_f,0,0);
    box(view_f,0,0);
    wrefresh(view_f);
    wrefresh(workview_f);


    wrefresh(workview);
    wrefresh(view);
    getch();

    int vmy,vmx;
    getmaxyx(view,vmy,vmx);
    int vpage = 0;
    for(int i = 1;i < argc;i++){
    file_player(view,argv[i]);
    wclear(view);
    getch();
    }



wprintw(workview,"enter to close");
wrefresh(workview);
    flushinp();
    getch();
    endwin();
    return 0;
}

/********************************************************************************
*********************************************************************************/

int file_player(WINDOW *WIN_P,char* filename){
    FILE *fp = fopen(filename,"r");
    if(fp == NULL){
        printw("file not found");
        getch();
        return -1;
    }
    int ch;
    int vy = 0;
    int vx = 0;
    int vmy = 0;
    int vmx = 0;
    int page = 1;

    long page_p[256];



    getmaxyx(WIN_P,vmy,vmx);
    wprintw(WIN_P,"<page of %d> [%s]\n",page,filename);
    mvwchgat(WIN_P,0,0,-1, A_REVERSE, 0, NULL);
    wmove(WIN_P,2,0);
    wrefresh(WIN_P);
    int body_lim  = vmy-2;


     page_p[page] = ftell(fp);
    while((ch = fgetc(fp)) != EOF){
            getyx(WIN_P,vy,vx);
            if(vy < body_lim){
            wprintw(WIN_P,"%c",ch);
    }else{
            mvwprintw(WIN_P,vmy-1,0,"[please any key]");
            mvwchgat(WIN_P,vmy-1,0,-1, A_REVERSE, 0, NULL);
            wmove(WIN_P,2,0);
            wrefresh(WIN_P);
            flushinp();
            int key = getch();
            switch(key){
                case 258:
                page++;
                wclear(WIN_P);
                wprintw(WIN_P,"<page of %d> [%s]\n",page,filename);
                 mvwchgat(WIN_P,0,0,-1, A_REVERSE, 0, NULL);
                wmove(WIN_P,2,0);
                break;
                
                case 259:
               long sabun = ftell(fp);
               if(page > 1){
               sabun = sabun - page_p[page-1];
               fseek(fp,-sabun,SEEK_CUR);
                page--;
                wclear(WIN_P);
                wprintw(WIN_P,"<page of %d> [%s]\n",page,filename);
                 mvwchgat(WIN_P,0,0,-1, A_REVERSE, 0, NULL);
                wmove(WIN_P,2,0);
               }else{
               sabun = sabun - page_p[page];
               fseek(fp,-sabun,SEEK_CUR);
                wclear(WIN_P);
                wprintw(WIN_P,"<page of %d> [%s]\n",page,filename);
                 mvwchgat(WIN_P,0,0,-1, A_REVERSE, 0, NULL);
                wmove(WIN_P,2,0);
               }
                break;

                default:
               long sabun2 = ftell(fp);
               sabun2 = sabun2 - page_p[page];
               fseek(fp,-sabun2,SEEK_CUR);
                wclear(WIN_P);
                wprintw(WIN_P,"<page of %d> [%s]\n",page,filename);
                 mvwchgat(WIN_P,0,0,-1, A_REVERSE, 0, NULL);
                wmove(WIN_P,2,0);
                break;
            }
     page_p[page] = ftell(fp);
         }

    }
    mvwprintw(WIN_P,vmy-1,0,"<enter to close>");
    mvwchgat(WIN_P,vmy-1,0,-1, A_REVERSE, 0, NULL);
    wrefresh(WIN_P);
    flushinp();
    getch();
    fclose(fp);
    wclear(WIN_P);
    wrefresh(WIN_P);
    return 0;


}
