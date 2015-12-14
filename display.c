/* File display.c */

#include "display.h"
#include "wand_head.h"
#include "icon.h"
#include <curses.h>
#include <stdio.h>
#include <string.h>

/***********************************
*        extern variables          *
************************************/
extern int debug_disp;
extern int edit_mode;
extern char screen_name[61];
extern char *edit_memory, *memory_end;


/******************************************
*                   map                   *
*******************************************/
void map(char (*row_ptr)[ROWLEN + 1])
{
    int x, y;
    char ch;

    move(0, 0);
    addch('+');
    for (x = 0; x < ROWLEN; x++)
        addch('-');
    addch('+');
    for (y = 0; y < NOOFROWS; y++)
    {
        move(y + 1, 0);
        addch('|');
        for (x = 0; x < ROWLEN; x++)
        {
            ch = (*row_ptr)[x];
            if (!debug_disp)
            {
                if ((ch == 'M') || (ch == 'S'))
                    ch = ' ';
                addch(ch);
            }
            else if (ch != '\0')
                addch(ch);
            else
                addch('"');
        }
        addch('|');
        row_ptr++;
    }
    move(y + 1, 0);
    addch('+');
    for (x = 0; x < ROWLEN; x++)
        addch('-');
    addch('+');
    if (!debug_disp)
    {
        move(18, 0);
        addstr("Press any key to return to the game.");
        refresh();
        getchar();
        move(18, 0);
        printw("%36s", "");
        refresh();
        for (y = 0; y <= (NOOFROWS + 1); y++)
        {
            move(y, 0);
            for (x = 0; x <= (ROWLEN + 2); x++)
                addch(' ');
        }
    }
    else
        refresh();
}

/**/
/***********************************************
*                   display                    *
************************************************/
void display(int cx, int cy, char (*row_ptr)[ROWLEN + 1])
{
    int x, y = 0, x_coord, y_coord;
    char ch;
    while (y < (cy - 3))
    {
        y++;
        row_ptr++;
    };
    move(0, 0);
    addstr("+---------------------------------+");
    move(15, 0);
    addstr("+---------------------------------+");
    for (y = (cy - 3); y <= (cy + 3); y++)
    {
        y_coord = (y + 3 - cy) * 2;
        if ((y < 0) || (y >= NOOFROWS))
        {
            move(y_coord + 1, 0);
            addstr("|#################################|");
            move(y_coord + 2, 0);
            addstr("|#################################|");
        }
        else
        {
            move(y_coord + 1, 0);
            addch('|');
            move(y_coord + 1, 34);
            addch('|');
            move(y_coord + 2, 0);
            addch('|');
            move(y_coord + 2, 34);
            addch('|');
            for (x = (cx - 5); x <= (cx + 5); x++)
            {
                x_coord = (x + 5 - cx) * 3;
                if ((x < 0) || (x > ROWLEN - 1))
                    draw_symbol(x_coord, y_coord, '#');
                else
                {
                    ch = (*row_ptr)[x];
                    draw_symbol(x_coord, y_coord, ch);
                }
            };
            row_ptr++;
        }                       /*   end if   */
    }                           /* end y loop */
    move(16, 0);
    refresh();
}

/*************************************************************
*                         showname                           *
**************************************************************/
void showname()
{
    move(19, 0);
    if ((screen_name[0] == '#') || (screen_name[0] == '\0'))
    {
        printw("%-40s", "Unnamed screen.");
    }
    else
        addstr(screen_name);
    if (edit_memory)
    {
        move(7, 45);
        addstr("MEMORY: ( Start, ) End,");
        move(8, 53);
        addstr("* Play, & Extend.");
        move(9, 53);
        addstr("- Chkpt, + Cont.");
        move(10, 53);
        if (memory_end == edit_memory)
        {
            addstr("--Empty--");
        }
        else
        {
            addstr("-Occupied-");
        }
    }
}

/**/
/******************************************************************
*                           redraw_screen                         *
*******************************************************************/
void redraw_screen(int *bell, int maxmoves, int num, long score, int nf,
                   int diamonds, int mx, int sx, int sy,
                   char (*frow)[ROWLEN + 1])
{
    char buffer[50];

    clear();
    move(0, 48);
    addstr("Score\t   Diamonds");
    move(1, 48);
    addstr("\tFound\tTotal");
    move(3, 48);
    sprintf(buffer, "%ld\t %d\t %d  ", score, nf, diamonds);
    addstr(buffer);
    if (!edit_mode)
    {
        move(6, 48);
        sprintf(buffer, "Current screen %d", num);
        addstr(buffer);
    }
    if (maxmoves != -1)
        sprintf(buffer, "Moves remaining = %d   ", maxmoves);
    else
        strcpy(buffer, "     Unlimited moves     ");
    move(15, 48);
    addstr(buffer);
    move(17, 56);
    if (*bell)
        addstr("Bell ON ");
    else
        addstr("Bell OFF");
    if (mx != -1)
    {                           /* tell player if monster exists */
        draw_symbol(50, 11, 'M');
        move(12, 56);
        addstr("Monster on the");
        move(13, 56);
        printw("%-14s", "loose!");
    }
    else
    {
        draw_symbol(50, 11, ' ');
        move(12, 56);
        printw("%14s", "");
        move(13, 56);
        printw("%14s", "");
    }

    showname();

    if (!debug_disp)
        display(sx, sy, frow);
    else
        map(frow);
}

/*************************************************
*                   inform_me                    *
**************************************************/
int inform_me(char *s, int qable)
{
    int retval = 0;
    move(20, 0);
#ifdef TVI
    addstr(TVI);
#endif
    standout();
    addstr(s);
#ifdef NOTVI
    addstr(NOTVI);
#endif
    standend();
    addstr(" <MORE>");
    if (qable)
        addstr(" (q stops)");
    refresh();
    if (getch() == 'q')
        retval = 1;
    move(20, 0);
    printw("%77s", "");
    refresh();
    return (retval);
}
