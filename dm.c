#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char username[256]="placeholder";
char pwd[256]="";
char cmd[512]="echo 'no command'";

void read_dmrc() {
    char path[512];
    snprintf(path,sizeof(path),"%s/.dmrc",getenv("HOME"));
    FILE *file=fopen(path,"r");
    if(!file) return;

    char line[512];
    while(fgets(line,sizeof(line),file)) {
        char *start=line;
        while(*start==' '||*start=='\t') start++;
        if(*start=='\n'||*start=='#') continue;

        char *eq=strchr(start,'=');
        if(!eq) continue;
        *eq='\0';
        char *key=start;
        char *value=eq+1;

        while(key[strlen(key)-1]==' '||key[strlen(key)-1]=='\t')
            key[strlen(key)-1]='\0';
        while(*value==' '||*value=='\t') value++;
        value[strcspn(value,"\n")]='\0';

        if(!strcmp(key,"username")) strncpy(username,value,sizeof(username)-1);
        else if(!strcmp(key,"pwd")) strncpy(pwd,value,sizeof(pwd)-1);
        else if(!strcmp(key,"cmd")) strncpy(cmd,value,sizeof(cmd)-1);
    }
    fclose(file);
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr,TRUE);

    read_dmrc(); // grab stuff from .dmrc

    int rows=LINES,cols=COLS;
    int start_col=cols/4+strlen("password: ");

    clear();
    int dm_len=strlen("Display Manager");
    int dm_col=(cols-dm_len)/2;
    attron(A_BOLD);
    mvprintw(rows/2-4,dm_col,"Display Manager");
    attroff(A_BOLD);
    mvprintw(rows/2-3,dm_col-3,"---------------------");

    char user_line[512];
    snprintf(user_line,sizeof(user_line),"User: %s",username);
    attron(A_BOLD);
    mvprintw(rows/2-2,(cols-strlen(user_line))/2,"%s",user_line);
    attroff(A_BOLD);

    mvprintw(rows/2,cols/4,"password: ");
    move(rows/2,start_col);
    refresh();

    char input[31]="";
    int pos=0;

    while(1) {
        int ch=getch();
        if(ch=='\n') { // enter key stuff
            if(!strcmp(input,pwd)) {
                endwin();
                system(cmd); // run the command if password’s right
                exit(0);
            } else {
                input[0]='\0';
                pos=0;
                mvprintw(rows/2,start_col,"                              ");
                move(rows/2,start_col);
                refresh();
            }
        }
        else if((ch==KEY_BACKSPACE||ch==127||ch==8)&&pos>0) {
            input[--pos]='\0';
            mvprintw(rows/2,start_col,"                              ");
            move(rows/2,start_col);
            for(int i=0;i<pos;i++) addch('*');
        }
        else if(ch>=' '&&ch<='~'&&pos<30) {
            input[pos++]=ch;
            input[pos]='\0';
            mvprintw(rows/2,start_col,"                              ");
            move(rows/2,start_col);
            for(int i=0;i<pos;i++) addch('*');
        }
        refresh();
    }

    endwin();
    return 0;
}
