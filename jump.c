/* File jump.c */

#include "jump.h"
#include "wand_head.h"
#include <curses.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern int debug_disp;
extern int no_passwords;
extern int maxscreens;

/************************************************************ 
*                   function scrn_passwd                    *
*              reads password num into passwd               *
*************************************************************/
int scrn_passwd(int num, char *passwd)  /* reads password num into passwd */
{
    long position;
    FILE *fp;

    position = PASSWD;
    while (position > 200000)
        position -= 200000;
    if ((fp = fopen(DICTIONARY, "r")) == NULL)
        return 0;
    fseek(fp, position, ftell(fp));
    while (fgetc(fp) != '\n');
    if (fscanf(fp, "%s\n", passwd) == EOF && errno != 0)
    {
        fprintf(stderr, "fscanf error\n");
        exit(EXIT_FAILURE);
    }
    /* read a word into passwd */
    fclose(fp);
    return (1);
}

/*******************************************************
*                   function showpass                  *
********************************************************/
void showpass(int num)
{
    char correct[20];
    if (no_passwords)
        return;
    if (!debug_disp)
        move(18, 0);
    else
        move(20, 0);
    if (!scrn_passwd(num, correct))
        return;
    printw("The password to jump to level %d ( using ~ ) is : %s        \n",
           (num + 1), correct);
    printw("%-65s\n", "PRESS ANY KEY TO REMOVE IT AND CONTINUE");
    refresh();
    getch();
    if (!debug_disp)
        move(18, 0);
    else
        move(20, 0);
    printw("%72s\n", "");
    printw("%46s", "");
    if (!debug_disp)
        move(18, 0);
    else
        move(20, 0);
    refresh();
}

/**********************************************************
*                    function jumpscreen                  *
***********************************************************/
int jumpscreen(int num)
{
    char word[20], correct[20];
    int index = 0;
    int scrn;
    struct timespec t;

    if (no_passwords == 1)
    {
        if (!debug_disp)
            move(16, 0);
        else
            move(18, 0);
        addstr("Enter number of desired level.\n");
        refresh();
        scrn = getnum();
        if (scrn > num)
        {
            if (!debug_disp)
                move(16, 0);
            else
                move(18, 0);
            printw("*48s", "");
            return scrn;
        }
        if (!debug_disp)
            move(16, 0);
        else
            move(18, 0);
        addstr("No way, Jose! Back-jumping is prohibited!");
        refresh();
        return num;
    }

    if (!debug_disp)
        move(16, 0);
    else
        move(18, 0);
    addstr("Please enter password of screen to jump to:");
    refresh();
    while (((word[index++] = getch()) != '\n') && (index < 19))
    {
        addch('*');
        refresh();
    }
    word[--index] = '\0';
    if (!debug_disp)
        move(16, 0);
    else
        move(18, 0);
    printw("%-58s\n", "Validating...");
    refresh();

    if (strcmp(word, MASTERPASSWORD) == 0)
    {
        if (!debug_disp)
            move(16, 0);
        else
            move(18, 0);
        addstr("Enter number of desired level.");
        refresh();
        num = getnum();
        (void) scrn_passwd(num - 1, correct);
        if (!debug_disp)
            move(16, 0);
        else
            move(18, 0);
        printw("Certainly master, but the correct word is %s.       \n",
               correct);
        printw("%-65s\n", "PRESS ANY KEY TO REMOVE IT AND CONTINUE");
        refresh();
        getchar();
        if (!debug_disp)
            move(16, 0);
        else
            move(18, 0);
        printw("%61s", "");
        if (!debug_disp)
            move(17, 0);
        else
            move(19, 0);
        printw("%61s", "");
        if (!debug_disp)
            move(16, 0);
        else
            move(18, 0);
        refresh();
        return num;
    }

    for (scrn = num; scrn < maxscreens; scrn++)
    {
        if (!scrn_passwd(scrn, correct))
            break;
        if (strcmp(correct, word) == 0)
        {
            if (!debug_disp)
                move(16, 0);
            else
                move(18, 0);
            printw("%-58s",
                   "Password Validated..... Jumping to desired screen.");
            refresh();
            return ++scrn;
        }
    }

    if (!debug_disp)
        move(16, 0);
    else
        move(18, 0);
    printw("%-44s", "PASSWORD NOT RECOGNISED!");
    refresh();
    t.tv_sec = 0;
    t.tv_nsec = 750000000;
    nanosleep(&t, NULL);
    if (!debug_disp)
        move(16, 0);
    else
        move(18, 0);
    printw("%61s", "");

    return num;
}

/***********************************************************
*                     function getnum                      *
************************************************************/
int getnum()
{
    char ch;
    int num = 0;

    for (ch = getch(), addch(ch), refresh(); ch >= '0' && ch <= '9';
         ch = getch(), addch(ch), refresh())
    {
        num = num * 10 + ch - '0';
    }
    return num;
}
