/* file wand_head.h */

#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <fcntl.h>

#undef MSDOS /* Marina */

#ifndef MSDOS
#include <sys/file.h>
#endif

#define VERSION "3.2"

                /********** FILES ************/

/* Change these to the necessary directories or files */
#define SCREENPATH  "/usr/local/share/wanderer/screens"
#define HISCOREPATH "/usr/local/share/wanderer/hiscore"
#define DICTIONARY  "/usr/share/dict/words"
#define LOCKFILE    "/tmp/wanderer.lock"

                /********** PASSWORDS *********/

/* change this to anything, but dont forget what                         */
#define MASTERPASSWORD "chocolate chips"

/* change the numbers in this as well, but keep it in the same form         */
#define PASSWD (num * num * 4373 + num * 16927 + 39)

/* this is the randon number seed used for encryption                          */
#define BLURFL 32451
/* the word blurfl is used for historical reasons                         */

                /********** OPTIONS ***********/

/* To disable the recording of hiscores from games restored from saves         */
/* #define NO_RESTORED_GAME_HISCORES  */
/* #define COMPARE_BY_NAME  /* define this to compare by name, not uid         */
/* #define NO_ENCRYPTION /* define this to disable the savefile encryptor */
#define NOISY    /* do we want bells in the game ? */

                /****** OTHER PARAMETERS ******/

#define GUESTUID 0    /* guestuid always compared by name         */
#define EMSIZE 1024   /* size of editor moves memory              */
#define ENTRIES 15    /* size of hiscore file                     */

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

            /**** NOTHING TO CHANGE BELOW HERE ****/

/* I wouldnt change these if I were you - it wont give you a bigger screen */
#define ROWLEN 40
#define NOOFROWS 16

/* MSDOS modifications (M001) by Gregory H. Margo        */
#ifdef        MSDOS
#define        R_BIN        "rb"        /* binary mode for non-text files */
#define        W_BIN        "wb"
# ifdef        VOIDPTR
#  define VOIDSTAR        (void *)
# else
#  define VOIDSTAR        (char *)
# endif
#define        ASKNAME                /* ask user's name if not in environment         */
#define        COMPARE_BY_NAME        /* compare users with name, not uid                */
#undef        getchar                /* remove stdio's definition to use curses'         */
#define        getchar()        getch()        /* use curse's definition instead */

#else /* not MSDOS */
#define        R_BIN        "r"
#define        W_BIN        "w"
#define        VOIDSTAR
#endif

/* Save and Restore game additions (M002) by Gregory H. Margo        */
/* mon_rec structure needed by save.c */
struct mon_rec
{
    int x,y,mx,my;
    char under;
    struct mon_rec *next,*prev;
};

struct        save_vars        {
        int        z_x, z_y,
                z_sx, z_sy,
                z_tx, z_ty,
                z_mx, z_my,
                z_diamonds,
                z_nf;
};

struct        old_save_vars        {
        int        z_x, z_y,
                z_nx, z_ny,
                z_sx, z_sy,
                z_tx, z_ty,
                z_lx, z_ly,
                z_mx, z_my,
                z_bx, z_by,
                z_nbx, z_nby,
                z_max_score,
                z_diamonds,
                z_nf,
                z_hd,
                z_vd,
                z_xdirection,
                z_ydirection;
};

/* prototypes added by Gregory H. Margo */
#ifdef        LINT_ARGS        /* M001 */
/* DISPLAY.c */
extern  void map(char (*)[ROWLEN+1]);
extern  void display(int ,int ,char (*)[ROWLEN+1],int );

/* EDIT.C */
extern  void instruct(void);
extern  void noins(void);
extern  void editscreen(int ,int *,int *,int ,char *);

/* FALL.C */
extern  int check(int *,int *,int ,int ,int ,int ,int ,int ,char *);
extern  int fall(int *,int *,int ,int ,int ,int ,char *);

/* GAME.C */
extern  struct mon_rec *make_monster(int ,int );
extern  char *playscreen(int *,int *,int *,int ,char *);

/* ICON.C */
extern  void draw_symbol(int ,int ,char );

/* JUMP.C */
extern  int scrn_passwd(int ,char *);
extern  void showpass(int );
extern  int jumpscreen(int );
extern  int getnum(void);

/* READ.C */
extern  int rscreen(int ,int *);
extern  int wscreen(int ,int );

/* SAVE.C */
extern  void save_game(int ,int *,int *,int ,struct mon_rec *,struct mon_rec *);
extern  void restore_game(int *,int *,int *,int *,struct mon_rec *,struct mon_rec **);

/* SCORES.C */
extern  int savescore(char *,int ,int ,char *);
extern  void delete_entry(int );
extern  int erase_scores(void);

#else

/* DISPLAY.c */
extern  void map();
extern  void display();

/* EDIT.C */
extern  void instruct();
extern  void noins();
extern  void editscreen();

/* FALL.C */
extern  int check();
extern  int fall();

/* GAME.C */
extern  struct mon_rec *make_monster();
extern  char *playscreen();

/* ICON.C */
extern  void draw_symbol();

/* JUMP.C */
extern  int scrn_passwd();
extern  void showpass();
extern  int jumpscreen();
extern  int getnum();

/* READ.C */
extern  int rscreen();
extern  int wscreen();

/* SAVE.C */
extern  void save_game();
extern  void restore_game();

/* SCORES.C */
extern  int savescore();
extern  void delete_entry();
extern  int erase_scores();

#endif

/* for monster movement */

#define viable(x,y) (((screen[y][x] == ' ') || (screen[y][x] == ':') ||\
        (screen[y][x] == '@') || (screen[y][x] == '+') ||\
        (screen[y][x] == 'S')) && (y >= 0) &&\
        (x >= 0) && (y < NOOFROWS) && (x < ROWLEN))
