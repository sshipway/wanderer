/* File edit.c */

#include "wand_head.h"

extern char *playscreen();
extern void helpme();
extern int inform_me();

extern int debug_disp;
extern char *edit_screen;
extern char screen[NOOFROWS][ROWLEN+1];
extern char *edit_memory;
extern char *memory_end;
extern char screen_name[61];

static char *inst[] = { "    O  Boulder",
                        "  < >  Arrows",
                        "    ^  Balloon",
                        "    :  Earth",
                        "    !  Landmine",
                        "    *  Treasure",
                        "  / \\  Deflectors",
                        "    +  Cage",
                        "_ = #  Rock (# indestructable)",
                        "    T  Teleport",
                        "    A  Arrival (1 max)",
                        "    X  Exit (always 1)",
                        "    @  Start (always 1)",
                        "    M  Big Monster (1 max)",
                        "    S  Baby Monster",
                        "    -  Alternative space",
                        "    C  Time Capsule",
                        "    ~  Thingy",
                        "    B  Bomb",
                        NULL };

/*********************************************************
*                      check_legality                    *
*   this will check: baby monster vs cage numbers,       *
*                    teleport, large monster,            *
*                    exit and player numbers,            *
*                    hanging boulders/arrows etc         *
**********************************************************/
check_legality()
{
int ercount,cages,hanging,bmons,tele,arrival,you,mons,exits;
int x, y;
char buf[80];
ercount = hanging = cages = bmons = tele = arrival = you = mons = exits = 0;

move(20,0);
addstr("Checking screen legality..."); refresh();
for( x = 0 ; x < ROWLEN ; x++)
    for( y = 0 ; y < NOOFROWS ; y++) {
        move(y+1,x+1); addch('?'); refresh();
        switch(screen[y][x]) {
          case '+': cages++; break;
          case 'S': bmons++; break;
          case 'T': tele++; break;
          case 'A': arrival++; break;
          case '@': you++; break;
          case 'M': mons++; break;
          case 'X': exits++; break;
          case '-':
          case ' ': if((screen[y-1][x] == 'O')&&(y>0)) hanging++;
                    if((screen[y+1][x] == '^')&&(y<NOOFROWS)) hanging++;
                    if((screen[y][x-1] == '>')&&(x>0)) hanging++;
                    if((screen[y][x+1] == '<')&&(x<ROWLEN)) hanging++;
                    break;
          default : break;
        }
        move(y+1,x+1); addch(screen[y][x]);
    }
move(20,0); addstr("                         ");
if(cages != bmons) {
        ercount++;
        if( cages < bmons )
            sprintf(buf,"++++ Warning: %d cage(s), %d baby monster(s).",cages,bmons);
        else
            sprintf(buf,"**** Cage imbalance: %d cage(s), %d baby monster(s).",cages,bmons);
        if(inform_me(buf,1)) return;
}
if(tele > 1) {
        ercount++;
        if(inform_me("++++ Warning: Too many teleports",1)) return;
}
if(arrival > ((tele>0)?1:0)) {
        ercount++;
        if(tele == 0) {
            if(inform_me("**** No arrivals without teleports.",1)) return;
        } else if(inform_me("**** Too many arrivals.",1)) return;
}
if(( arrival == 0 ) && ( tele > 0 )) {
        ercount++;
        if(inform_me("**** No arrival for teleport.",1)) return;
}
if( you != 1 ) {
        ercount++;
        if( you == 0 ) {
            if(inform_me("**** No start position.",1)) return;
        } else if(inform_me("**** Too many start positions.",1))return;
}
if( mons > 1 ) {
        ercount++;
        if(inform_me("**** Too many monsters.",1))return;
}
if( exits != 1 ) {
        ercount++;
        if( exits == 0 ) {
            if(inform_me("**** No exit to screen.",1))return;
        } else if(inform_me("++++ Warning: Too many exits.",1))return;
}
if( hanging > 0 ) {
        sprintf(buf,"++++ Warning: %d hanging boulders/arrows/balloons.",hanging);
        if(inform_me(buf,1)) return;
}
ercount += hanging;
move(19,0);
if( ercount == 0 ) inform_me("---- Screen OK.",0);
else {
    sprintf(buf,"---- Total errors: %d",ercount);
    inform_me(buf,0);
}
refresh();
}

/**************************************************************
*                          readstring                         *
***************************************************************/
void readstring(str,size)
    char *str;
    int size;
{
  int count = 0;
  char ch;
  for(;;) {
    ch = getch();
    if( ch == '\n' ) {
        *str = '\0';
        break;
    }
    if(( ch == '\010') || ( ch == '\177' )) {
        if(count == 0)
                continue;
        str--; count--;
        addstr("\b \b");
        refresh();
        continue;
    }
    if(count == size) {
        printf("\007");
        continue;
    }
    addch(ch);
    *str = ch;
    str++; count++;
    refresh();
  }
}

clearbottom()
{
move(20,0);
addstr("                                                                            \n");
addstr("                                                                            \n");
addstr("                                                                            ");
}

/*****************************************
*                 instruct               *
*  Print instructions around the screen  *
******************************************/
void instruct()
{
int loop;
for(loop = 0; inst[loop] ; loop++)
    {
    move(loop,45);
    addstr(inst[loop]);
    }
move(21,0);
addstr("c: change name, m: change moves, q: save and exit, n/p: play game\n");
addstr("Press '?' for full editor instructions. Use wanderer keys to move.");
}


/************************************************
*                     noins                     *
*************************************************/
void noins()
{
int loop;
for(loop =0; inst[loop] ; loop++)
    {
    move(loop,45);
    addstr("                              ");
    }
clearbottom();
}

/********************************
*          screen_save          *
*  save and restore screen data *
*********************************/
screen_save(maxmoves)
int maxmoves;
{
char file[90];
char *oldname;
int y;

clearbottom();
move(20,0);
addstr("Filename to write to? :");
if(edit_screen)
    addstr(edit_screen);
else
    addstr("./screen");
move(20,23); refresh();
addstr("                                   "); move(20,23);
readstring(file,89);
move(20,0);
addstr("                                                                           "); refresh();
oldname = edit_screen;
if( file[0] ) edit_screen = file;
for(y = 0; y<=NOOFROWS;y++)        /* make sure screen written */
    if(screen[y][ROWLEN-1] == ' ') /* correctly...             */
        screen[y][ROWLEN-1] = '-';
wscreen(0,maxmoves);
for(y = 0; y<=NOOFROWS;y++)
    if(screen[y][ROWLEN-1] == '-')
        screen[y][ROWLEN-1] = ' ';
edit_screen = oldname;
}

/*********************************************
*                 screen_read                *
**********************************************/
screen_read(maxmoves)
    int *maxmoves;
{
    static char file[90];
    int y;

    clearbottom();
    move(20,0);
    addstr("Filename to read from? :");
    addstr("Forget it");
    move(20,24); refresh();
    addstr("                                   "); move(20,24);
    readstring(file,89);
    move(20,0);
    addstr("                                                                           "); refresh();
    if( ! file[0] ) return;
    edit_screen = file;
    rscreen(0,maxmoves);
    for(y = 0; y<=NOOFROWS;y++)
        if(screen[y][ROWLEN-1] == '-')
            screen[y][ROWLEN-1] = ' ';
}


/*************************************
*              edit_save             *
*  save and restore edit memory data *
**************************************/
edit_save()
{
    char file[90];
    int i = 0,fd;

    clearbottom();
    move(20,0);
    addstr("Filename to save? :"); refresh();
    readstring(file,89);
    move(20,0); addstr("                                                                           "); refresh();
    if(( fd = open(file,O_WRONLY|O_CREAT|O_TRUNC,0644)) == -1 ) {
        inform_me("File cannot be opened for writing.",0);
    } else {
        i = write(fd, edit_memory, (int)(memory_end - edit_memory));
    if( i < 0 ) inform_me("Write error on file.",0);
    }
    instruct();
}

/*******************************************
*                edit_restore              *
********************************************/
edit_restore()
{
char file[90];
int i = 0,fd;

clearbottom();
move(20,0);
addstr("Filename to load? :"); refresh();
readstring(file,89);
move(20,0); addstr("                                                                           "); refresh();
move(19,0);
if(( fd = open(file,O_RDONLY)) == -1 ) {
    inform_me("File cannot be opened for reading.",0);
} else {
    i = read(fd, edit_memory, EMSIZE);
    if( i < 0 ) inform_me("Read error on file.",0);
    if( i == 0 ) inform_me("File empty.",0);
    if( i > 0 ) {
        sprintf(file,"Read in %d moves.",i);
        inform_me(file,0);
        memory_end = edit_memory + i;
    }
}
instruct();
}

/*************************
*       editscreen       *
*  Actual edit function  *
**************************/
void editscreen(num,score,bell,maxmoves,keys)
    int num;
    int maxmoves, *bell, *score;
    char keys[10];
{
    int  mmbkup,x,y,sx=0,sy=0,quit=0,nx,ny,nosave =0;
    char (*frow)[ROWLEN+1] = screen,
         ch;
    char buffer[50];
    char *howdead;
    char *storage;

    if((storage = (char *) malloc(sizeof(screen)))== NULL) {
        addstr("OOps... cant malloc a backup screen!\n\n");
        return;
    }

    for(x=0;x<=ROWLEN;x++)
        for(y=0;y<NOOFROWS;y++)
        {
            if(screen[y][x] == '@')
            {
                sx = x;
                sy = y;
            }
            if(screen[y][x] == '-')
                screen[y][x] = ' ';
        };
    x=sx;
    y=sy;
    if(maxmoves != 0)
        (void) sprintf(buffer,"Moves   : %d        ",maxmoves);
    else
        (void) strcpy(buffer,"Moves   : Unlimited");
    debug_disp=1;
    map(frow);
    move(18,0);
    addstr(buffer);
    move(19,0);
    addstr("                                                                          ");
    move(19,0);
    addstr("Name    : ");
    addstr(screen_name);

/* ACTUAL EDIT FUNCTION */

    instruct();
/* HERE */
/* Beginning of long while loop */
while(!quit)
{
    move(y+1,x+1);
    refresh();
    ch = (char)getchar();

    nx=x;
    ny=y;

    if(ch == keys[3]||ch == keys[2]||ch == keys[1]||ch == keys[0])
    {
        if(ch == keys[3])
           nx++;
        if(ch == keys[2])
                nx--;
        if(ch == keys[1])
            ny++;
        if(ch == keys[0])
            ny--;
    }
    else if(ch == 'q')
    {
        clearbottom();
        break;
    }
    else if(ch == 'x')
    {
        clearbottom();
        move(20,0);
        addstr("You will lose any changes made this session - are you sure? (y/n)");
        refresh();
        ch = getch();
        if(ch != 'y')
        {
            noins();
            instruct();
            refresh();
            }
        else
        {
            nosave = 1;
            addstr("\n");
            refresh();
            break;
            }
    }
    else if(ch == 'm')         /* change to number of moves for the screen */
    {
        clearbottom();
        move(21,0);
        addstr("How many moves for this screen? :");
        refresh();echo();
        scanf("%d",&maxmoves);noecho();
        if(maxmoves < 0 ) maxmoves = 0;
        if(maxmoves != 0)
            (void) sprintf(buffer,"Moves   : %d        ",maxmoves);
        else
            (void) strcpy(buffer,"Moves   : Unlimited ");
        instruct();
        move(18,0);
        addstr(buffer);
        refresh();        /* for some reason, this seems to add a '.' to */
                              /* the map... Ive no idea why yet... */
        }
    else if(ch == 'c') {        /* change name */
        clearbottom();
        move(21,0);
        addstr("New name: ");
        refresh();
        readstring(screen_name,58);
        screen_name[61] = '\0';
        instruct();
        move(19,0);
        addstr("                                                                          ");
        move(19,0);
        addstr("Name    : ");
        addstr(screen_name);
        refresh();
    }
    else if(ch == 'p' || ch == 'n')       /* play the game (test) */
    {
            noins();
            mmbkup = maxmoves;
            bcopy(screen,storage,sizeof(screen));
            if(ch == 'p')
        {
                debug_disp = 0;
                clear();
        }
            *score = 0;
            howdead = playscreen(&num,score,bell,maxmoves,keys);
            move(20,0);
            if(howdead!=0)
                addstr(howdead);
            else
                addstr("DONE!");
            printw("; hit any key to continue\n");
            refresh();
            ch = (char)getchar();
            clear();
            bcopy(storage,screen,sizeof(screen));
            maxmoves = mmbkup;
            debug_disp = 1;
            map(frow);
            if(maxmoves != 0)
                (void) sprintf(buffer,"Moves   : %d        \n",maxmoves);
            else
                (void) strcpy(buffer,"Moves   : Unlimited\n");
            move(18,0);
            addstr(buffer);
            addstr("Name    : ");
            addstr(screen_name);
            instruct();
    }
    else if(ch == 18) {  /* ctrl r -- read memory data */
        edit_restore();
    }
    else if( ch == 23) { /* ctrl w -- write memory data */
        edit_save();
    }
    else if( ch == 7 ) { /* ctrl g -- read screen */
        screen_read(&maxmoves);
        map(frow);
        instruct();
    }
    else if( ch == 16 ) { /* ctrl p -- write screen */
        screen_save(maxmoves);
        instruct();
    }
    else if( ch == 12 ) { /* ctrl l -- redraw screen  */
        clear();
        map(frow);
        if(maxmoves != 0)
                (void) sprintf(buffer,"Moves   : %d        \n",maxmoves);
        else
            (void) strcpy(buffer,"Moves   : Unlimited\n");
        move(18,0);
        addstr(buffer);
        addstr("Name    : ");
        addstr(screen_name);
        instruct();
    }
    else if( ch == 'L' ) {
        clearbottom();
        check_legality();
        instruct();
    }
    else if(ch == '?' ) {
        helpme(0);
        if(maxmoves != 0)
            (void) sprintf(buffer,"Moves   : %d        \n",maxmoves);
        else
                (void) strcpy(buffer,"Moves   : Unlimited\n");
        map(frow);
    }
    else if((ch == 127)||(ch == 8)) {  /* delete key */
        if(--nx < 0)
            {
            nx = ROWLEN -1;
            if(--ny < 0 ) ny = NOOFROWS -1;
        }
        screen[ny][nx] = ' ';
        move(ny+1,nx+1);
        addch(' ');
    }
    else
    {
        if(ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
        if(ch == '"') ch = (char)getchar();
        if( ! (ch < ' ') )
            {
            screen[y][x] = ch;
                   move(y+1,x+1);
            addch(ch);
            nx++;
        }
        }
    if(nx < 0)
        {
        nx = ROWLEN-1;
        ny--;
        }
    if(nx >= ROWLEN)
        {
        nx = 0;
        ny++;
        }
    if(ny < 0) ny = NOOFROWS-1;
    if(ny >= NOOFROWS) ny = 0;
    move(ny+1,nx+1);
    x=nx;
    y=ny;
    }
    /* End of while loop */
    
    noins();
    move(20,0);
    refresh();
    
    if(! nosave)
        {
        for(y = 0; y<=NOOFROWS;y++) /* certain editors - eg ded - have a */
                                    /* habit of truncating trailing spaces*/
                                            /* so this should stop them! */
            if(screen[y][ROWLEN-1] == ' ')
                screen[y][ROWLEN-1] = '-';
        wscreen(num,maxmoves);
        }
}
