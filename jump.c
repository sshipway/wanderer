/* File jump.c */

#include "wand_head.h"

extern int debug_disp;
extern int no_passwords;
extern int maxscreens;

/************************************************************ 
*                   function scrn_passwd                    *
*              reads password num into passwd               *
*************************************************************/
int scrn_passwd(num, passwd)    /* reads password num into passwd */
int num;
char *passwd;
{
        long position;
        FILE *fp;

        position = PASSWD;
        while(position > 200000)
                position -= 200000;
        if((fp = fopen(DICTIONARY,"r")) == NULL)
                return 0;
        fseek(fp,position,ftell(fp));
        while(fgetc(fp) != '\n');
        fscanf(fp,"%s\n",passwd);
        /* read a word into passwd */
        fclose(fp);
        return (1);
}

/*******************************************************
*                   function showpass                  *
********************************************************/
void showpass(num)
     int num;
{
    long position;
    char correct[20];
    char buffer[100];
    FILE *fp;
    char ch;
    if(no_passwords)
        return;
    if(!debug_disp)
        move(18,0);
    else
        move(20,0);
    if(!scrn_passwd(num,correct))
        return;
    (void) sprintf(buffer,"The password to jump to level %d ( using ~ ) is : %s        \n",(num+1),correct);
    addstr(buffer);
    addstr("PRESS ANY KEY TO REMOVE IT AND CONTINUE                          \n");
    refresh();
    ch = getch();
    if(!debug_disp)
        move(18,0);
    else
        move(20,0);
    addstr("                                                                        \n");
    addstr("                                              ");
    if(!debug_disp)
        move(18,0);
    else
        move(20,0);
    refresh();
}

/**********************************************************
*                    function jumpscreen                  *
***********************************************************/
int jumpscreen(num)
    int num;
{
    char word[20],
         buffer[100],
         correct[20];
    int index=0, input;
    char ch;
    long position;
    int  fp, scrn;

    if(no_passwords == 1) {
        if(!debug_disp)
            move(16,0);
        else
            move(18,0);
        addstr("Enter number of desired level.\n");
        refresh();
        scrn = getnum();
        if(scrn > num) {
            if(!debug_disp)
                move(16,0);
            else
                move(18,0);
            addstr("                                                ");
            return scrn;
            }
        if(!debug_disp)
            move(16,0);
        else
            move(18,0);
        addstr("No way, Jose! Back-jumping is prohibited!");
        refresh();
        return num;
    }

    if(!debug_disp)
        move(16,0);
    else
        move(18,0);
    addstr("Please enter password of screen to jump to:");
    refresh();
    while(((word[index++] = getch()) != '\n')&&(index < 19))
    {
        addch('*');
        refresh();
    }
    word[--index]='\0';
    if(!debug_disp)
        move(16,0);
    else
        move(18,0);
    addstr("Validating...                                             \n");
    refresh();

    if(strcmp(word,MASTERPASSWORD) == 0)
    {
        if(!debug_disp)
            move(16,0);
        else
            move(18,0);
        addstr("Enter number of desired level.");
        refresh();
        num = getnum();
        (void) scrn_passwd(num-1,correct);
        sprintf(buffer,"Certainly master, but the correct word is %s.       \n",correct);
        if(!debug_disp)
            move(16,0);
        else
            move(18,0);
        addstr(buffer);
        addstr("PRESS ANY KEY TO REMOVE IT AND CONTINUE                          \n");
        refresh();
        getchar();
        if(!debug_disp)
            move(16,0);
        else
            move(18,0);
        addstr("                                                             ");
        if(!debug_disp)
            move(17,0);
        else
            move(19,0);
        addstr("                                                             ");
        if(!debug_disp)
            move(16,0);
        else
            move(18,0);
        refresh();
        return num;
    }

    for(scrn = num;scrn < maxscreens;scrn++) {
        if(!scrn_passwd(scrn,correct))
            break;
        if(strcmp(correct,word) == 0)
        {
            if(!debug_disp)
                move(16,0);
            else
                move(18,0);
            addstr("Password Validated..... Jumping to desired screen.        ");
            refresh();
            return ++scrn;
        }
    }

    if(!debug_disp)
        move(16,0);
    else
        move(18,0);
    addstr("PASSWORD NOT RECOGNISED!                    ");
    refresh();
    usleep(750000);  /* Marina */
    if(!debug_disp)
        move(16,0);
    else
        move(18,0);
    addstr("                                                          ");

    return num;
}

/***********************************************************
*                     function getnum                      *
************************************************************/
int getnum()
{
    char ch;
    int num = 0;

    for(ch = getch(),addch(ch),refresh(); 
        ch >= '0' && ch <= '9'; 
        ch = getch(),addch(ch),refresh())
    {
        num = num * 10 + ch - '0';
    }
    return num;
}
