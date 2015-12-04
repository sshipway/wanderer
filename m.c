/* File m.c */

#include <sys/time.h>
#include "wand_head.h"

#define SCROLLING 0
#define PAGED 1

/**********************************
*      function declarations      *
***********************************/
extern char *playscreen();
extern int rscreen();
extern int savescore();
extern void map();
extern void check_legality();

/***********************************
*         Global Variables         *
************************************/
int debug_disp = 0;
int no_passwords = 0;
int maxscreens;
char screen[NOOFROWS][ROWLEN+1];
int edit_mode = 0;
int saved_game = 0;
char *edit_screen = (char *) 0;
char *edit_memory = (char *) 0;
char *memory_end = (char *) 0;
char screen_name[61] ;
int record_file = -1;

/**/
/***********************************
*              readline            *
************************************/
int readline(int fd,char *ptr,int max)
{
  int count=0;

  while(read(fd,ptr,1)==1) 
  {
    if(++count==max)
      break;
    if(*ptr=='\n')
      break;
    ptr++;
  }
  *ptr='\0';
  return count;
}

/**************************************************
 *               Show Credits                     *
 * Break this function into 2 discrete functions  *
 * One for scrolling and one for paged Credits    *
 **************************************************/
void show_credits(opt)
        int opt;
{
int maxlines, linecount;
FILE *fp;
char ch, buffer[100];
int inp ,nul;
struct timeval tv;

    sprintf(buffer,"%s/credits",SCREENPATH);
    if((fp = fopen(buffer,"r")) == NULL) {
        printf("Sorry - credits unavailable!\n");
        exit(1);
        }
    if (opt == PAGED)
    {
        initscr();
        CBON;
        noecho();
        maxlines = tgetnum("li") - 3;
    }
    linecount = 0;
    nul = 0;
    tv.tv_sec = 0;
    tv.tv_usec = 500000L;  /* half second between scrolls */
    while( fgets(buffer,100,fp) ) {
            addstr(buffer);
                if(opt == PAGED) {
                if(++linecount == maxlines) {
                        move(maxlines + 2,0);
                        addstr("-- More --");
                        refresh();
                        ch = getch();
                        if( ch == 'q' ) 
                            break;
                        move(maxlines + 2,0);
                        addstr("          ");
                        refresh();
                        clear();
                        linecount = 0;
                }
            } else { /* opt == SCROLLING */
                inp = 1;
                printf("%s",buffer);
                select(1,&inp,&nul,&nul,&tv);
                if(inp) {
                    read(0,&ch,1);
                    if(ch == 'q') 
                        break;
                }
            }
    }
    if(opt == PAGED ) { /* Marina's changes - stdout blocked under curses */
        CBOFF;
        echo();
        move(maxlines+2,0);
        refresh();
        endwin();
    }
}

/**/
/***************************************************************
*                         get_name                             *
* Placed in a new separate function. Is asking really needed ? *
* Call function before initializing curses. Marina             *
****************************************************************/
char *get_name(void)
{
char *name;
char *endchar;

    if((name = (char *)getenv("NEWNAME")) == NULL)
        if((name = (char *)getenv("NAME")) == NULL)
            if((name = (char *)getenv("FULLNAME")) == NULL)
                if((name = (char *)getenv("USER")) == NULL)
                          if((name = (char *)getenv("LOGNAME")) == NULL)
#define ASKNAME /* Marina */
#ifdef        ASKNAME        /* M001 */
                    {
                        name = (char *)malloc(80);
                        if (name == NULL) {
                            printf("malloc error\n"); /* Replace with Err* */
                            exit(1);
                        }
                        printf("Name? "); fflush(stdout);
                        fgets(name,80,stdin); /* get rid of gets Marina*/
                        endchar=strchr(name,'\0'); 
                        endchar='\0';
                        if (name[0] == '\0')
                            name = "noname";
                    }
#else
                    name = "noname";
#endif
    return(name);
}

/**********************************************
*                   get_keys                  *
*  Get movement keys for navigating screen    *
***********************************************/
char *get_keys()
{
    char *keys = NULL;
    if( ! keys ) {
        if((keys = (char *)getenv("NEWKEYS")) == NULL)
            {
            keys = (char *)malloc(5);
           strcpy(keys,"kjhl");
            }
    }
    return(keys);
}

/**/
/***************
*  Why here ?  *
****************/
extern int optind,opterr;
extern char *optarg;

/***************************************
*  Main Program  -- Comment by Marina  *
****************************************/
main(int argc,char **argv)
{
char (*frow)[ROWLEN+1] = screen;
long score = 0;
int fp,
    num = 1,
    bell = 0,
    maxmoves = 0,
    x,y;
char howdead[25],
     buffer[100],
     *name,
     *keys,
     *dead,ch;
char c; 

while(( c = getopt(argc,argv,"01k:et:r:fmCcvsi")) != -1 )
{
    switch(c) 
    {
        case '0': bell = 0; break;
        case '1': bell = 1; break;
        case 'k': keys = optarg; break;
        case 'i': 
                printf("\nWANDERER Copyright (C) 1988 S Shipway. Version %s.\n\n",VERSION);
                printf("Screens in %s.\n",SCREENPATH);
                printf("Hiscore File %s.\n",HISCOREPATH);
                printf("Looking for Dictionary in %s.\n",DICTIONARY);
                printf("Lockfile for Scorefile %s.\n",LOCKFILE);
                exit(0);
                break;
        case 'm': erase_scores(); /* Need noecho() before rec passwd Marina */
                exit(0);
        case 'c': show_credits(PAGED);
                exit(0);
        case 'C': show_credits(SCROLLING);
                exit(0);
        case 's': 
                savescore("-",-1,-1,"-"); /* savescore bug ! */
                exit(0);
        case 'f': debug_disp = 1; break;    /* Full screen mode */
        case 'v': printf("\nWANDERER Copyright (C) 1988 S Shipway. Version %s.\n\n",VERSION);
                exit (0);
        case 'r':
                if((record_file = open(optarg,O_WRONLY|O_CREAT,0600))==-1) {
                        printf("Cannot open file %s for recording.\n",optarg);
                    exit(-1);
                }
                break;
        case 'e': edit_mode = 1;
                 no_passwords = 1;
                memory_end = edit_memory = (char *)malloc(EMSIZE * sizeof(char));
                break;
        case 't': edit_screen = optarg;
                debug_disp = edit_mode = 1;
                rscreen(0,&maxmoves);
                initscr();
                map(frow);
                check_legality();
                move(21,0); refresh();
                endwin();
                exit(0);
        default: fprintf(stderr,"Usage: %s [ -e | -m | -C | -c | -s | [ -r file ] [ -f ] | -t file ] [ -0 | -1 ] [ -i ] [ -k keys ] [ file ]\n",argv[0]);
        exit(1);
        }
    }

if( optind < argc ) edit_screen = argv[optind];

/* check for passwords - if file no_pws is in screen dir no pws! */
sprintf(buffer,"%s/no_pws",SCREENPATH);
if((fp = open(buffer,O_RDONLY)) != -1) {
    close(fp);
    no_passwords = 1;
}

/* count available screens */
for(maxscreens = 0;;maxscreens++) {
    sprintf(buffer,"%s/screen.%d",SCREENPATH,(maxscreens+1));
    if((fp = open(buffer, O_RDONLY)) == -1 )
        break;
    close(fp);
} 

name=get_name();  /* M */
keys=get_keys();  /* M */
initscr();

/* MAIN PROGRAM HERE */

CBON; noecho();

if(!edit_mode) {
        for (;;num++) {
            if (rscreen(num,&maxmoves)) 
            {
                strcpy(howdead,"a non-existant screen");
                break;
                }
            dead = playscreen(&num,&score,&bell,maxmoves,keys);
            if ((dead != NULL) && (*dead == '~')) 
            {
                num = (int)(dead[1]) - 1;
                dead = NULL;
            }
            if (dead != NULL)
            {
                strcpy(howdead,dead);
                break;
            }
        }
    }
else
    {
    if(rscreen(num,&maxmoves))
        {
        for(x=0;x<ROWLEN;x++)
            for(y=0;y<NOOFROWS;y++)
                screen[y][x] =  ' ';
        }
    editscreen(num,&score,&bell,maxmoves,keys);
    }
/* END OF MAIN PROGRAM */

/****************************
*  SAVE ROUTINES FOR SCORES * 
*****************************/
    if(!edit_mode)
    {
        getch();
        echo();
        refresh();
        endwin();
        printf("%s killed by %s with a score of %d on level %d.\n", 
                                            name,howdead,score,num); 
        printf("\nWANDERER (C)1988 S. Shipway\n");
        if((savescore(howdead,score,num,name) == 0)&&(score != 0))
            printf("\nWARNING: %s : score not saved!\n",argv[0]);
    }
    else
    { /* Still need to clean up screen, yes ? Marina */
        
        echo();
        refresh();
        endwin();
    }
    if(record_file != -1)/* Wouldn't it be better !to leave file open?*/
        close(record_file);
}
