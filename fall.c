/* File fall.c */

#include "wand_head.h"

extern void draw_symbol();
extern int debug_disp;
extern char screen[NOOFROWS][ROWLEN+1];

int moving = 0; /* so that bombs explode only if something *hits* them */

/***************************************************** 
*                 Function check                     *
******************************************************/
/* check for any falling caused by something moving out of x,y along  *
*  vector dx,dy. All the others are constant and should really have   *
*  been global...                                                     */

int check(mx,my,x,y,dx,dy,sx,sy,howdead)
    int x,y,sx,sy,dx,dy, *mx, *my;
    char howdead[25];
{
    int ret=0;
    ret+=fall(mx,my,x,y,sx,sy,howdead);
    ret+=fall(mx,my,x-dx,y-dy,sx,sy,howdead);
    ret+=fall(mx,my,x-dy,y-dx,sx,sy,howdead);
    ret+=fall(mx,my,x+dy,y+dx,sx,sy,howdead);
    ret+=fall(mx,my,x-dx-dy,y-dy-dx,sx,sy,howdead);
    ret+=fall(mx,my,x-dx+dy,y-dy+dx,sx,sy,howdead);
    return ret;
}

/***************************************************** 
*                 Function fall                      *
******************************************************/
/* recursive function for falling *
*  boulders and arrows            *
***********************************/
int fall(mx,my,x,y,sx,sy,howdead)  
    int  x,y,sx,sy, *mx, *my;
    char howdead[25];
{
int nx = x,nxu = x,nyl = y,nyr = y,ny = y,retval = 0;
if ((y>(NOOFROWS-1))||(y<0)||(x<0)||(x>(ROWLEN-1)))
    return(0);
if(screen[y][x] == '~') {
   if( screen[y-1][x] == ' ' ) fall(mx,my,x,y-1,sx,sy,howdead);
   if( screen[y+1][x] == ' ' ) fall(mx,my,x,y+1,sx,sy,howdead);
   if( screen[y][x-1] == ' ' ) fall(mx,my,x-1,y,sx,sy,howdead);
   if( screen[y][x+1] == ' ' ) fall(mx,my,x+1,y,sx,sy,howdead);
}
if((screen[y][x] != 'O') && (screen[y][x] != ' ') && (screen[y][x] != 'M') &&
   (screen[y][x] !='\\') && (screen[y][x] != '/') && (screen[y][x] != '@') &&
   (screen[y][x] != '^') && (screen[y][x] != 'B'))
    return(0);
if((screen[y][x] == 'B') && (moving == 0 )) return 0;
if(screen[y][x] == 'O')
    {
    if((screen[y][x-1] == ' ') && (screen[y-1][x-1] == ' '))
        nx--;
    else
        {
        if((screen[y][x+1] == ' ') && (screen[y-1][x+1] == ' '))
            nx++;
        else
            nx = -1;
        }
    if((screen[y][x-1] == ' ') && (screen[y+1][x-1] == ' '))
        nxu--;
    else
        {
        if((screen[y][x+1] == ' ') && (screen[y+1][x+1] == ' '))
            nxu++;
        else
            nxu = -1;
        }
    if((screen[y-1][x] == ' ') && (screen[y-1][x+1] == ' '))
        nyr--;
    else
        {
        if((screen[y+1][x] == ' ') && (screen[y+1][x+1] == ' '))
            nyr++;
        else
            nyr = -1;
        }
    if((screen[y-1][x] == ' ') && (screen[y-1][x-1] == ' '))
        nyl--;
    else
        {
        if((screen[y+1][x] == ' ') && (screen[y+1][x-1] == ' '))
            nyl++;
        else
            nyl = -1;
        }
    }
if(screen[y][x] == '\\')
    {
    if(screen[y-1][++nx] != ' ')
        nx = -1;
    if(screen[y+1][--nxu] != ' ')
        nxu = -1;
    if(screen[--nyr][x+1] != ' ')
        nyr = -1;
    if(screen[++nyl][x-1] != ' ')
        nyl = -1;
    }
if(screen[y][x] == '/')
    {
    if(screen[y-1][--nx] != ' ')
        nx = -1;
    if(screen[y+1][++nxu] != ' ')
        nxu = -1;
    if(screen[++nyr][x+1] != ' ')
        nyr = -1;
    if(screen[--nyl][x-1] != ' ')
        nyl = -1;
    }
if((screen[y][nx] != ' ') && (screen[y][nx] != 'M') && (screen[y][nx] != 'B'))
    nx = -1;
if((screen[y-1][x] == 'O') && (nx >= 0) && (y > 0)) /* boulder falls ? */
    {
    moving = 1;
    screen[y-1][x] = ' ';
    if(screen[y][nx] == '@')
        {
            strcpy(howdead,"a falling boulder");
            retval=1;
            }
    if(screen[y][nx] == 'M')
        {
            *mx = *my = -2;
        screen[y][nx] = ' ';
            }
    if(screen[y][nx] == 'B')
        {
        retval = bang(nx,y,mx,my,sx,sy,howdead);
        return retval;
        }
    screen[y][nx] = 'O';
    if(!debug_disp)
        {
        if((y<(sy+5)) && (y>(sy-3)) && (x>(sx-6)) && (x<(sx+6)))
            draw_symbol((x-sx+5)*3,(y-sy+2)*2,' ');
        if((y<(sy+4)) && (y>(sy-4)) && (nx>(sx-6)) && (nx<(sx+6)))
            draw_symbol((nx-sx+5)*3,(y-sy+3)*2,'O');
        }
    else
        {
        move(y,x+1);
        addch(' ');;
        move(y+1,nx+1);
        addch('O');
    }
    refresh();
    retval+=fall(mx,my,nx ,y+1,sx,sy,howdead);
    moving = 0;
    retval+=check(mx,my,x,y-1,0,1,sx,sy,howdead);
    if(screen[y+1][nx] == '@')
        {
            strcpy(howdead,"a falling boulder");
            return(1);
            }
    if(screen[y+1][nx] == 'M')
        {
            *mx = *my = -2;
        screen[y+1][nx] = ' ';
            }
    }
if((screen[nyr][x] != '^')&&(screen[nyr][x] != ' ')&&(screen[nyr][x] != 'M')
   &&(screen[nyr][x] != 'B'))
    nyr = -1;
if((screen[y][x+1] == '<')&&(nyr>=0)&&(x+1<ROWLEN)) /* arrow moves ( < ) ? */
    {
    moving = 1;
    screen[y][x+1] = ' ';
    if(screen[nyr][x] == '@')
        {
            strcpy(howdead,"a speeding arrow");
            retval = 1;
            }
    if(screen[nyr][x] == 'M')
        {
            *mx = *my = -2;
        screen[nyr][x] = ' ';
            }
    if(screen[nyr][x] == 'B')
        {
        retval = bang(x,nyr,mx,my,sx,sy,howdead);
        return retval;
        }
    screen[nyr][x] = '<';
    if(!debug_disp)
        {
        if((y<(sy+4)) && (y>(sy-4)) && (x<(sx+5)) && (x>(sx-7)))
            draw_symbol((x-sx+6)*3,(y-sy+3)*2,' ');
        if((nyr<(sy+4)) && (nyr>(sy-4)) && (x<(sx+6)) && (x>(sx-6)))
            draw_symbol((x-sx+5)*3,(nyr-sy+3)*2,'<');
        }
    else
        {
        move(y+1,x+2);
        addch(' ');
        move(nyr+1,x+1);
        addch('<');
        }
    refresh();
    retval+=fall(mx,my,x-1,nyr,sx,sy,howdead);
    moving = 0;
    retval+=check(mx,my,x+1,y,-1,0,sx,sy,howdead);
    if(screen[nyr][x-1] == '@')
        {
            strcpy(howdead,"a speeding arrow");
            return(1);
            }
    if(screen[nyr][x-1] == 'M')
        {
            *mx = *my = -2;
        screen[nyr][x-1] = ' ';
            }
    }
if((screen[nyl][x] != ' ')&&(screen[nyl][x] != '^')&&(screen[nyl][x] != 'M')
    &&( screen[nyl][x] != 'B'))
    nyl = -1;
if((screen[y][x-1] == '>')&&(nyl>=0)&&(x>0))       /* arrow moves ( > ) ? */
    {
    moving = 1;
    screen[y][x-1] = ' ';
    if(screen[nyl][x] == '@')
        {
            strcpy(howdead,"a speeding arrow");
            retval = 1;
            }
    if(screen[nyl][x] == 'M')
        {
            *mx = *my = -2;
        screen[nyl][x] = ' ';
            }
    if(screen[nyr][x] == 'B')
        {
        retval = bang(x,nyr,mx,my,sx,sy,howdead);
        return retval;
        }
    screen[nyl][x] = '>';
    if(!debug_disp)
        {
        if((y<(sy+4)) && (y>(sy-4)) && (x<(sx+7)) && (x>(sx-5)))
            draw_symbol((x-sx+4)*3,(y-sy+3)*2,' ');
        if((nyl<(sy+4)) && (nyl>(sy-4)) && (x<(sx+6)) && (x>(sx-6)))
            draw_symbol((x-sx+5)*3,(nyl-sy+3)*2,'>');
        }
    else
        {
        move(y+1,x);
        addch(' ');
        move(nyl+1,x+1);
        addch('>');
        }
    refresh();
    retval+=fall(mx,my,x+1,nyl,sx,sy,howdead);
    moving = 0;
    retval+=check(mx,my,x-1,y,1,0,sx,sy,howdead);
    if(screen[nyl][x+1] == '@')
        {
            strcpy(howdead,"a speeding arrow");
            return(1);
            }
    if(screen[nyl][x+1] == 'M')
        {
            *mx = *my = -2;
        screen[nyl][x+1] = ' ';
            }
    }

if(screen[y][nxu] != ' ')
    nxu = -1;

if((screen[y+1][x] == '^') && (nxu >= 0) && (y < NOOFROWS) &&
   (screen[y][x] != '^')&&(screen[y][x] != 'B')) /* balloon rises? */
    {
    screen[y+1][x] = ' ';
    screen[y][nxu] = '^';
    if(!debug_disp)
        {
        if((y<(sy+3)) && (y>(sy-5)) && (x>(sx-6)) && (x<(sx+6)))
            draw_symbol((x-sx+5)*3,(y-sy+4)*2,' ');
        if((y<(sy+4)) && (y>(sy-4)) && (nxu>(sx-6)) && (nxu<(sx+6)))
            draw_symbol((nxu-sx+5)*3,(y-sy+3)*2,'^');
        }
    else
        {
        move(y+2,x+1);
        addch(' ');
        move(y+1,nxu+1);
        addch('^');
    }
    refresh();
    retval+=fall(mx,my,nxu ,y-1,sx,sy,howdead);
    retval+=check(mx,my,x,y+1,0,-1,sx,sy,howdead);
    }

nx = x; ny = y;

if(screen[y][x] == ' ') {     /* thingy moves? */
    if((y>1)&&(screen[y-1][x]=='~')&&(screen[y-2][x]=='O'))
        /* boulder pushes */
        ny--;
    else {
    if((x>1)&&(screen[y][x-1]=='~')&&(screen[y][x-2]=='>'))
        /* arrow pushes */
        nx--;
    else {
    if((x<(ROWLEN-1))&&(screen[y][x+1]=='~')&&(screen[y][x+2]=='<'))
        /* arrow pushes */
        nx++;
    else {
    if((y<(NOOFROWS-1))&&(screen[y+1][x]=='~')&&(screen[y+2][x]=='^'))
        /* balloon pushes */
        ny++;
    }}}

    if(( x != nx )||( y != ny )) {
        screen[y][x] = '~';
        screen[ny][nx] = screen[2*ny-y][2*nx-x];
        screen[2*ny-y][2*nx-x] = ' ';
           if(!debug_disp) {
            if(((2*ny-y)<(sy+4)) && ((2*ny-y)>(sy-4))
                && ((2*nx-x)>(sx-6)) && ((2*nx-x)<(sx+6)))
                    draw_symbol((2*nx-x-sx+5)*3,(2*ny-y-sy+3)*2,' ');
            if((y<(sy+4)) && (y>(sy-4)) && (x>(sx-6)) && (x<(sx+6)))
                    draw_symbol((x-sx+5)*3,(y-sy+3)*2,'~');
            if((ny<(sy+4)) && (ny>(sy-4)) && (nx>(sx-6)) && (nx<(sx+6)))
                    draw_symbol((nx-sx+5)*3,(ny-sy+3)*2,screen[ny][nx]);
        } else {
            move(ny*2-y+1,nx*2-x+1);
            addch(' ');
            move(ny+1,nx+1);
            addch(screen[ny][nx]);
            move(y+1,x+1);
            addch('~');
        }
        refresh();
        retval+=fall(mx,my,2*x-nx,2*y-ny,sx,sy,howdead);
        retval+=check(mx,my,2*nx-x,2*ny-y,nx-x,ny-y,sx,sy,howdead);
    }
}

if(retval>0)
    return(1);
return(0);
}

/**********************************************
*                function bang                *
***********************************************/
int bang(x,y,mx,my,sx,sy,howdead) /* explosion centre x,y */
    int x,y,sx,sy,*mx,*my;
    char *howdead;
{
    int retval = 0;
    int ba,bb;  /* abbrevs for 'bang index a' and 'bang index b' :-) */
    int gottim = 0;
    screen[y][x] = ' ';
/* fill with bangs */
    for(ba = -1; ba < 2; ba++ )
        for(bb = -1; bb < 2; bb++ ) {
            if(screen[y+ba][x+bb] == '#') continue; /* rock indestructable */
            if(screen[y+ba][x+bb] == '@') gottim = 1;
            if(screen[y+ba][x+bb] == 'M') *mx = *my = -2; /* kill monster */
            if(screen[y+ba][x+bb] == 'B')
                gottim += bang(x+bb,y+ba,mx,my,sx,sy,howdead);
            screen[y+ba][x+bb] = ' ';
               if(!debug_disp) {
                if(((y+ba)<(sy+4)) && ((y+ba)>(sy-4)) && ((x+bb)>(sx-6))
                && ((x+bb)<(sx+6)) && (y+ba >= 0 ) && (x +bb >= 0) &&
                ((x+bb)<ROWLEN) && ((y+ba)<NOOFROWS) )
                        draw_symbol((x+bb-sx+5)*3,(y+ba-sy+3)*2,'%');
            } else {
                if( ((x+bb)>-1)     && ((y+ba)>-1) &&
                ((x+bb)<ROWLEN) && ((y+ba)<NOOFROWS) ) {
                    move(y+ba+1,x+bb+1);
                    addch('%');
                }
            }
        }
    refresh();
    if(gottim) {
        strcpy(howdead,"an exploding bomb");
        return 1;
    }
    /* erase it all */
    for(ba = -1; ba < 2; ba++ )
        for(bb = -1; bb < 2; bb++ ) {
            if(screen[y+ba][x+bb] == '#') continue;
               if(!debug_disp) {
                if(((y+ba)<(sy+4)) && ((y+ba)>(sy-4)) && ((x+bb)>(sx-6))
                && ((x+bb)<(sx+6)) && (y+ba > -1) && (x+bb > -1) &&
                ((x+bb)<ROWLEN) && ((y+ba)<NOOFROWS) )
                        draw_symbol((x+bb-sx+5)*3,(y+ba-sy+3)*2,' ');
            } else {
                if( ((x+bb)>-1)     && ((y+ba)>-1) &&
                ((x+bb)<ROWLEN) && ((y+ba)<NOOFROWS) ) {
                    move(y+ba+1,x+bb+1);
                        addch(' ');
                }
            }
        }

    /* make all the necessary falling */
    retval = check(mx,my,x-1,y-1,1,0,sx,sy,howdead);
    retval += check(mx,my,x-1,y+1,0,-1,sx,sy,howdead);
    retval += check(mx,my,x+1,y-1,0,1,sx,sy,howdead);
    retval += check(mx,my,x+1,y+1,-1,0,sx,sy,howdead);
    return retval;
}
