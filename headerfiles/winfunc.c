#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../headers/winfunc.h"

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
                        wprintw(WIN_P,"<page of %d> [%s]\n",page,filename);
                        mvwchgat(WIN_P,0,0,-1, A_REVERSE, 0, NULL);
                        wmove(WIN_P,2,0);
                    }
                    break;

                default:
                    long sabun2 = ftell(fp);
                    sabun2 = sabun2 - page_p[page];
                    fseek(fp,-sabun2,SEEK_CUR);
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











//******************************************************************************************************************************************



int file_reader(WINDOW *WIN_P,char* filename){

    int ln = 0;  //line number    the current line of cursol   current line is the most under of window
    int vx = 0; int vy = 0;
    int vmy = 0; int vmx = 0;
    getmaxyx(WIN_P,vmy,vmx);
    wsetscrreg(WIN_P, 2, vmy-1);
    scrollok(WIN_P, TRUE);
    mvwprintw(WIN_P,0,0,"line:%d",ln);
    wmove(WIN_P,2,0);
    wclear(WIN_P);

    FILE *fp = fopen(filename,"r");

    if(fp == NULL){
        wprintw(WIN_P,"file not found");
        getch();
        return -1;
    }

    long *line_p;
    long *temp = (long *)malloc(sizeof(long)*20);
    if(temp == NULL){
        free(line_p);
        return -1;
    }
    line_p = temp;
    line_p[ln] = ftell(fp);
    char buffer [512];
    while(fgets(buffer,sizeof(buffer),fp) != NULL){
        getyx(WIN_P,vy,vx);
        if(vy+2 < vmy){ 
            wprintw(WIN_P,buffer);
            ln++;

                line_p[ln] = ftell(fp);
                long *temp = realloc(line_p,sizeof(long)*(10+ln)); 
                if(temp == NULL){
                    free(line_p);
                    return -1;
                }

                line_p = temp;

            
        }else{
            wprintw(WIN_P,buffer);
            ln++;
            break;
        }
    }
    wrefresh(WIN_P);
    int key = 0;

    while(key != 119){
        long current_point = 0;
        current_point = ftell(fp);
        key = getch();
        line_p[ln] = ftell(fp);
        long *temp = realloc(line_p,sizeof(long)*(10+ln)); 
        if(temp == NULL){
            free(line_p);
            return -1;
        }
        line_p = temp;

        switch(key){
            case 258:
                fseek(fp,line_p[ln]-current_point,SEEK_CUR);
                fgets(buffer,sizeof(buffer),fp);
                if(!feof(fp)){
                mvwprintw(WIN_P,vmy-1,0,buffer);
                ln++;
                wrefresh(WIN_P);
                }else{
                    return 0;
                }
                break;

            case 259:
                 if(ln > -4 + vmy){
                fseek(fp,line_p[ln - (vmy-3)]-current_point,SEEK_CUR);
                fgets(buffer,sizeof(buffer),fp);
                mvwprintw(WIN_P,2,0,buffer);
                ln--;
                wscrl(WIN_P,-1);
                wrefresh(WIN_P);
                }
                break;

              case 119:
               free(line_p);
               fclose(fp);
               return 0;


        }




    }
}



//**************************************************************************************************************************

int file_reader_p(WINDOW *WIN_P,FILE *fp){

    int ln = 0;  //line number    the current line of cursol   current line is the most under of window
    int vx = 0; int vy = 0;
    int vmy = 0; int vmx = 0;
    getmaxyx(WIN_P,vmy,vmx);
    wsetscrreg(WIN_P, 2, vmy-1);
    scrollok(WIN_P, TRUE);
    mvwprintw(WIN_P,0,0,"line:%d",ln);
    wmove(WIN_P,2,0);

    if(fp == NULL){
        wprintw(WIN_P,"file not found");
        getch();
        return -1;
    }

    long *line_p;
    long *temp = (long *)malloc(sizeof(long)*20);
    if(temp == NULL){
        free(line_p);
        return -1;
    }
    line_p = temp;
    line_p[ln] = ftell(fp);
    char buffer [512];
    while(fgets(buffer,sizeof(buffer),fp) != NULL){
        getyx(WIN_P,vy,vx);
        if(vy+2 < vmy){ 
            wprintw(WIN_P,buffer);
            ln++;

                line_p[ln] = ftell(fp);
                long *temp = realloc(line_p,sizeof(long)*(10+ln)); 
                if(temp == NULL){
                    free(line_p);
                    return -1;
                }

                line_p = temp;

            
        }else{
            wprintw(WIN_P,buffer);
            ln++;
            break;
        }
    }
    wrefresh(WIN_P);
    int key = 0;

    while(key != 119){
        long current_point = 0;
        current_point = ftell(fp);
        key = getch();
        line_p[ln] = ftell(fp);
        long *temp = realloc(line_p,sizeof(long)*(10+ln)); 
        if(temp == NULL){
            free(line_p);
            return -1;
        }
        line_p = temp;

        switch(key){
            case 258:
                
                fseek(fp,line_p[ln]-current_point,SEEK_CUR);
                fgets(buffer,sizeof(buffer),fp);
                mvwprintw(WIN_P,vmy-1,0,buffer);
                wrefresh(WIN_P);
                ln++;
                break;

            case 259:
                 if(ln > -4 + vmy){
                fseek(fp,line_p[ln - (vmy-3)]-current_point,SEEK_CUR);
                fgets(buffer,sizeof(buffer),fp);
                mvwprintw(WIN_P,2,0,buffer);
                ln--;
                wscrl(WIN_P,-1);
                wrefresh(WIN_P);
                }
                break;

              case 119:
               free(line_p);
               return 0;
               pclose(fp);
               break;


        }




    }
}
