/* File monsters.c */

#include "wand_head.h"

typedef struct { int d[2]; } direction;

#ifdef        LINT_ARGS        /* M001 */
direction new_direction(int, int, int, int);
#else
direction new_direction();
#endif

extern void draw_symbol();
extern int debug_disp;
extern int edit_mode;
extern char screen[NOOFROWS][ROWLEN+1];

/* Add a spirit to the chain */
/* Maintain a doubly linked list to make reuse possible.
   tail_of_list is *NOT* the last monster allocated, but
   the last monster alloted to a screen.  start_of_list
   is a dummy entry to ease processing. last_of_list
   is the last entry allocated. */

extern struct mon_rec *last_of_list, *tail_of_list;
extern struct mon_rec start_of_list;

/********************************************************
*                 function make_monster                 *
*********************************************************/
struct mon_rec *make_monster(x,y)
int x,y;
{
    char *malloc();
    #define MALLOC (struct mon_rec *)malloc(sizeof(struct mon_rec))
    struct mon_rec *monster;
    if(tail_of_list->next == NULL)
    {
        if((last_of_list = MALLOC) == NULL)
            return NULL;
        tail_of_list->next = last_of_list;
        last_of_list->prev = tail_of_list;
        last_of_list->next = NULL;
    }
    monster = tail_of_list = tail_of_list->next;
    monster->x = x;
    monster->y = y;
    monster->mx = 1;      /* always start moving RIGHT. (fix later)  */
    monster->my = 0;
    monster->under = ' ';
    return monster;
}

/********************************************************************
*                      function direction                           *
*********************************************************************/
/* 'follow lefthand wall' algorithm for baby monsters */

direction new_direction(x,y,bx,by)
                        int x,y,bx,by;
{
    direction out;
    if(viable((x+by),(y-bx)))
    {
        out.d[0] = by;
        out.d[1] = -bx;
        return out;
    }
    if(viable((x+bx),(y+by)))
    {
        out.d[0] = bx;
        out.d[1] = by;
        return out;
    }
    if(viable((x-by),(y+bx)))
    {
        out.d[0] = -by;
        out.d[1] = bx;
        return out;
    }
    if(viable((x-bx),(y-by)))
    {
        out.d[0] = -bx;
        out.d[1] = -by;
        return out;
    }
    out.d[0] = -bx;
    out.d[1] = -by;
    return out;
}

/***********************************************************
*                   function move_monsters                 *
************************************************************/
int move_monsters(mxp, myp, score, howdead, sx, sy, nf, bell, x, y, diamonds)
                  int *mxp, *myp, *score, sx, sy, nf, bell, x, y, diamonds;
                  char *howdead;
{
    int xdirection, ydirection, hd, vd;
    int deadyet = 0;
    int bx, by, nbx, nby, tmpx,tmpy;
    direction new_disp;
    struct mon_rec *monster,*current;
    char buffer[25];

/* big monster first */
    if(*mxp == -2)                       /* has the monster been killed ? */
    {
        *score+=100;
        *mxp = *myp = -1;
        move(3,48);
        sprintf(buffer,"%d\t %d\t",*score,nf);
        (void) addstr(buffer);
        draw_symbol(50,11,' ');
        move(12,56); addstr("              ");
        move(13,56); addstr("              ");
        move(16,0);
        refresh();
    }                                     /* if monster still alive */

    if(*mxp != -1)                        /* then move that monster ! */
    {
        screen[*myp][*mxp] = ' ';
        if(*mxp>x)
            xdirection = -1;
        else
            xdirection = 1;
        if(!debug_disp)
        {
            if((*myp<(sy+4))&&(*myp>(sy-4))&&(*mxp<(sx+6))&&(*mxp>(sx-6)))
                draw_symbol((*mxp-sx+5)*3,(*myp-sy+3)*2,' ');
        }
        else
        {
            move(*myp+1,*mxp+1);
            addch(' ');
        }
        if((hd = (*mxp-x))<0)
            hd = -hd;
        if((vd = (*myp-y))<0)
            vd = -vd;
        if((hd>vd)&&((*mxp+xdirection)<ROWLEN)&&((screen[*myp][*mxp+xdirection] == ' ')||(screen[*myp][*mxp+xdirection] == '@')))
            *mxp+=xdirection;
        else
        {
            if(*myp>y)
                ydirection = -1;
            else
                ydirection = 1;
            if(((*myp+ydirection)<NOOFROWS)&& ((screen[*myp+ydirection][*mxp]
== ' ')||(screen[*myp+ydirection][*mxp] == '@')))
                *myp+=ydirection;
            else
                if(((*mxp+xdirection)<ROWLEN)&&(screen[*myp][*mxp+xdirection] == ' ')||(screen[*myp][*mxp+xdirection] == '@'))
            *mxp+=xdirection;
        }
        if(!debug_disp)
        {
            if((*myp<(sy+4))&&(*myp>(sy-4))&&(*mxp<(sx+6))&&(*mxp>(sx-6)))
                draw_symbol((*mxp-sx+5)*3,(*myp-sy+3)*2,'M');
        }
        else
        {
            move(*myp+1,*mxp+1);
            addch('M');
        }
        if(screen[*myp][*mxp] == '@')                     /* ha! gottim! */
        {
            strcpy(howdead,"a hungry monster");
            move(16,0);
            refresh();
            deadyet = 1;
        }
        screen[*myp][*mxp] = 'M';
        move(16,0);
        refresh();
    }

    current = &start_of_list;                         /* baby monsters now */
    while((current != tail_of_list)&&(!deadyet))
    /* deal with those little monsters */
    {
        monster = current->next;
        new_disp = new_direction( monster->x, monster->y, monster->mx, monster->my );
        if(monster->under!='S')             /* if on top of another baby */
        {
            screen[monster->y][monster->x] = monster->under;
            if(!debug_disp)
            {
                if((monster->y < (sy+4)) && (monster->y > (sy-4)) && (monster->x < (sx+6)) && (monster->x > (sx-6)))
                    draw_symbol((monster->x-sx+5)*3,(monster->y-sy+3)*2,monster->under);
            }
            else
            {
                move(monster->y+1,monster->x+1);
                addch(monster->under);
            }
            if(monster->under == ' ')
                deadyet+=check(&*mxp,&*myp,monster->x,monster->y,new_disp.d[0],new_disp.d[1],sx,sy,howdead);
        }
        else
            monster->under=' ';
        monster->mx = new_disp.d[0];
        monster->my = new_disp.d[1];
        monster->x += monster->mx;
        monster->y += monster->my;
        monster->under = screen[monster->y][monster->x];
        screen[monster->y][monster->x] = 'S';        /* move into new space */
        if(!debug_disp)
        {
            if((monster->y < (sy+4)) && (monster->y > (sy-4)) && (monster->x < (sx+6)) && (monster->x > (sx-6)))
                draw_symbol((monster->x-sx+5)*3,(monster->y-sy+3)*2,'S');
        }
        else
        {
            move(monster->y+1,monster->x+1);
            addch('S');
        }
        if(monster->under == '@')                     /* monster hit you? */
        {
            strcpy(howdead,"the little monsters");
            move(16,0);
            refresh();
            deadyet = 1;
            monster->under = ' ';
        }
        if(monster->under == '+')                    /* monster hit cage? */
        {
#ifdef NOISY
            if(bell) printf("\007");
#endif
            *score +=20;
            move(3,48);
            sprintf(buffer,"%d\t %d\t %d ",*score,nf,diamonds);
            (void) addstr(buffer);
        /* remove from chain, and insert at the end (at last_of_list) */
            if(monster == tail_of_list)
                tail_of_list = tail_of_list->prev;
            else
            {
                current->next = monster-> next;
                current->next->prev = current;
                monster->next = NULL;
                monster->prev = last_of_list;
                last_of_list->next = monster;
                last_of_list = monster;
            }
            screen[monster->y][monster->x] = '*';
            if(!debug_disp)
            {
                if((monster->y < (sy+4)) && (monster->y > (sy-4)) && (monster->x < (sx+6)) && (monster->x > (sx-6)))
                    draw_symbol((monster->x-sx+5)*3,(monster->y-sy+3)*2,'*');
            }
            else
            {
                move(monster->y+1,monster->x+1);
                addch('*');
            }
        }
        else
            current = monster;
        move(16,0);
        refresh();
    }
    return deadyet;
}
