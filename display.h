#ifndef __DISPLAY_H
#define __DISPLAY_H

                /*********** CBREAK ***********/

/* cbreak switching via curses package.                                  */
/* on some Ultrix systems you may need to use crmode() and nocrmode()    */
/* if so, just change the #defs to the necessary. I also know that Xenix */
/* systems have to use crmode, so..                                      */
#ifdef XENIX
#define CBON crmode()
#define CBOFF nocrmode()
#else
#define CBON cbreak()
#define CBOFF nocbreak()
#endif

/* I wouldnt change these if I were you - it wont give you a bigger screen */
#define ROWLEN 40
#define NOOFROWS 16

void display(int cx, int cy, char (*row_ptr)[ROWLEN + 1]);
int inform_me(char *s, int qable);
void map(char (*row_ptr)[ROWLEN + 1]);
void redraw_screen(int *bell, int maxmoves, int num, long score,
                   int nf, int diamonds, int mx, int sx, int sy,
                   char (*frow)[ROWLEN + 1]);

#endif // __DISPLAY_H
