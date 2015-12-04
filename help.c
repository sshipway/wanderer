/* file help.c */
/* From jason@uucp.aeras */

#include "wand_head.h"

char *help[]={
"      **  W A N D E R E R  **      ", /* 0 */
"    ===========================    ", /* 1 */
"        by Steven Shipway          ", /* 2 */
"How to play:                       ", /* 3 */
" Collect all the treasure:    /$\\  ", /* 4 */
"                              \\$/  ", /* 5 */
" Then go through the exit:    Way  ", /* 6 */
"                              out  ", /* 7 */
"  h  Left       j  Down            ", /* 8 */
"  k  Up         l  Right           ", /* 9 */
#ifdef NOISY
"  1  Loud       q  Quit game       ", /* 10 */
"  0  Quiet      !  Look at map     ", /* 11 */
"  S  Save game  R  Restore Game    ", /* 12 */
"  ?  Help mode  @  Center screen   ", /* 13 */
"  ~  Jump level #  Switch mode     ", /* 14 */
" (no bonus)     W,^L Redraw screen ", /* 15 */
#else
"  q  Quit game  !  Look at map     ", /* 10 */
"  S  Save game  R  Restore Game    ", /* 11 */
"  ?  Help mode  @  Center screen   ", /* 12 */
"  #  Other mode W,^L Redraw screen ", /* 13 */
"  ~  Jump level (no bonus recieved)", /* 14 */
"                                   ", /* 15 */
#endif

"This is you:  You are a spider.    ", /* 0 */
"      o       (At least, that's    ", /* 1 */
"     <|>      what you look like)  ", /* 2 */
"                                   ", /* 3 */
"The other items you will find are: ", /* 4 */
"                                   ", /* 5 */
"  ###     -=- Solid Rock. The first", /* 6 */
"  ### and =-= cannot be blown up.  ", /* 7 */
"                                   ", /* 8 */
"  <O>   Time capsule (5 points,    ", /* 9 */
"        +250 extra moves)          ", /* 10 */
"   .                               ", /* 11 */
"  . .   Passable earth (one point) ", /* 12 */
"                                   ", /* 13 */
" (*)   Teleport  (50 points for    ", /* 14 */
" (*)              using it)        ", /* 15 */

"  /^\\   Boulder (falls down, other ", /* 0 */
"  \\_/   boulders and arrows fall   ", /* 1 */
"        off of it)                 ", /* 2 */
"                                   ", /* 3 */
"  <--     -->  Arrows              ", /* 4 */
"  <-- and -->  (run left and right)", /* 5 */
"                                   ", /* 6 */
"  TTT   Cage - holds baby monsters ", /* 7 */
"  III   and changes into diamonds  ", /* 8 */
"                                   ", /* 9 */
"  /$\\    (10 points)               ", /* 0 */
"  \\$/    Money  (collect it)       ", /* 1 */
"                                   ", /* 2 */
"  -o-  Baby monster (kills you)    ", /* 3 */
"  /*\\                              ", /* 4 */
"                                   ", /* 5 */

"When a baby monster hits a cage it ", /* 0 */
"is captured and you get 50 points. ", /* 1 */
"The cage also becomes a diamond.   ", /* 2 */
"                                   ", /* 3 */
"  I   Instant annihilation         ", /* 4 */
"  o                                ", /* 5 */
"                                   ", /* 6 */
" \\_       _/   Slopes (boulders    ", /* 7 */
"   \\ and /     and etc slide off)  ", /* 8 */
"                                   ", /* 9 */
" }o{  Monster  (eats you up whole. ", /* 0 */
" /^\\  Yum Yum yum..) (100 points)  ", /* 1 */
"      (kill with a rock or arrow)  ", /* 2 */
"                                   ", /* 3 */
" Way  Exit -- Must collect all the ", /* 4 */
" out  treasure first. (250 bonus)  ", /* 5 */

" /~\\  Balloon -- rises, and is     ", /* 0 */
" \\_X   popped by arrows. It does   ", /* 1 */
"        *not* kill you.            ", /* 2 */
"                                   ", /* 3*/
" OOO  Unrecognised symbol in map.  ", /* 4*/
" OOO  This is probably a **bug** ! ", /* 5*/
"                                   ", /* 6 */
" \\|/  'Thingy' - can be pushed in  ", /* 7 */
" /|\\   any direction by you or by  ", /* 8 */
"        arrows/boulders/balloons   ",  /* 9 */
"                                   ",  /* 0 */
" _|_  Another wall character, for  ",  /* 1 */
" _|_  more variety to the game     ",  /* 2 */
"                                   ",  /* 3 */
" /\\*  Bomb - explodes when hit by  ",  /* 4 */
" \\/  arrow/boulder. Destroys area. ",  /* 5 */

"  SCORING:                         ",  /* 0 */
"    Score 1 for earth, 10 for money",  /* 1 */
" 5 for capsule, 20 for teleporting ", /* 2 */
" 100 for killing monster, 20 for   ", /* 3 */
" catching baby monster, 250 for    ", /* 4 */
" escaping with all the money.      ", /* 5 */
"                                   ", /* 6 */
"                                   ", /* 7 */
" ENVIRONMENT VARIABLES:            ", /* 8 */
"                                   ", /* 9 */
"   NEWNAME,NAME : Checked in that  ", /* 0 */
"       order for the hiscore table ", /* 1 */
"   NEWKEYS : Redefine movement keys", /* 2 */
"       eg- 'hlkj' for default      ", /* 3 */
"   SAVENAME : File used for saved  ", /* 4 */
"       games.                      ", /* 5 */

"  Options:                         ",
"                                   ",
"  -r (file) : record moves to file ",
"  -t (file) : test screen          ",
"  -e (file) : edit screen          ",
"  -c        : show credits file    ",
"  -v        : show version  number ",
"  -m        : edit hiscore file    ",
"  -s        : show hiscore file    ",
"  -k string : define keys          ",
"  -f        : full-screen mode     ",
#ifdef NOISY
"  -1 and -0 : start wil bell on/off",
#else
"                                   ",
#endif
"                                   ",
" *** Thanks to everyone who helped ",
"     with the beta testing!        ",
"                                   ",
NULL };

char *edhelp[] = {
" The Keys:                         ",
"                                   ",
"   n: play game with full screen   ",
"   p: play with normal screen      ",
"   c: change screen name           ",
"   m: change moves allowance       ",
"                                   ",
"   \": next character is literal    ",
"   L: check screen legality        ",
"   q: quit, saving screen          ",
"   x: exit, do not save screen     ",
"   CTRL-R: read moves memory       ",
"   CTRL-W: write moves memory      ",
"   CTRL-L: redraw screen           ",
"   CTRL-G: read screen file  (get) ",
"   CTRL-P: write screen file (put) ",

"  THE WANDERER SCREEN EDITOR HELP  ",
"  -------------------------------  ",
"                                   ",
"  The editor has a number of       ",
" special features. The main ones   ",
" are summarised at the bottom of   ",
" the screen, but there are also    ",
" a few more advanced features.     ",
"  These are described on the next  ",
" page.                             ",
"                                   ",
"  When playing a screen, you will  ",
" not die (usually) but will instead",
" be given the option of continuing ",
" the game from where you died.     ",
"                                   ",

" L: check legality                 ",
"                                   ",
"  The legality checker can also be ",
" called via the -t option. Any     ",
" minor errors - ie ones the game   ",
" can survive - are prefixed with   ",
" '+++' and any major (fatal) errors",
" are prefixed '***'. A screen      ",
" should have no errors.            ",
"                                   ",
"                                   ",
" CTRL-W, CTRL-R: moves memory      ",
"                                   ",
"  When playing the game you have   ",
" the ability to record your moves, ",
" and to play them back again.      ",

"  You can save this memory to a    ",
" text file, or load from a suitable",
" file, using these keys. When      ",
" playing the game, press '(' to    ",
" start recording and ')' to end.   ",
" Also, pressing '+' will introduce ",
" a checkpoint.                     ",
"  Press '*' to playback the moves  ",
" from the beginning. The playback  ",
" will stop at the end, or at a     ",
" checkpoint. Press '-' to continue ",
" from a checkpoint.                ",
"  Pressing '&' will start recording",
" from the current position - this  ",
" will either extend the memory,    ",
" or, if used when playback has     ",

" stopped at a checkpoint, it will  ",
" overwrite any following data.     ",
"                                   ",
"  The memory is a maximum of 1024  ",
" bytes (this can be changed in the ",
" wand_head.h file).                ",
"                                   ",
"                                   ",
" CTRL-G and CTRL-P: get and put    ",
"                                   ",
"  These will read and write the    ",
" screen data. As with the memory   ",
" functions, a file name is prompted",
" for. Pressing return will use the ",
" default provided.                 ",
"                                   ",

NULL };

/**********************************************
*               function helpme               *
***********************************************/
void
helpme(htype)   /* routine to show help menu. */
int htype;      /* 0 == editor                */
{
         int i = 0, i2 = 0;   /* loop counters      */
         char *ptr;           /* pointer in array.. */
         char ch;
         char **helplist;

         if( htype )
             helplist = help;
         else
             helplist = edhelp;

         /* clear board - fullscreen mode is too big to overlay */
        for(i = 0; i < 18; i++)
        {
                  move(i,0);
                  addstr("                                           ");
         }

         while( helplist[i2] )  /* times to show loop. */
         {
                  for(i = 0; i < 16; i++)         /* show one menu. */
                  {
                           ptr = helplist[i2++];
                           move(i,0);  /* move to start of line. */
                           addstr(ptr);
                  }
                  move(i,0);  /* move to start of line. */
#ifdef TVI
                  addstr(TVI);
#else
                  standout();
#endif
                  if( helplist[i2] )
                      addstr("Press any key to continue, <q> to exit");
                 else
                      addstr("End of help. Press any key to exit    ");
#ifdef NOTVI
                  addstr(NOTVI);
#else
                  standend();
#endif
                  refresh();               /* show on screen. */
                  ch = (char) getchar();   /* just for now, get anything. */
                  if(ch == 'q')            /* if return or what ever.. */
                           break;          /* exit routine now. */
         }
         move(i,0);  /* move to start of line. */
         addstr("                                        ");
}
