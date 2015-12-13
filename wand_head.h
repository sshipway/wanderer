/* file wand_head.h */

#undef MSDOS                    /* Marina */

#ifndef MSDOS
#include <sys/file.h>
#endif

#define VERSION "3.2"

                /********** FILES ************/

/* Change these to the necessary directories or files */
#ifndef SCREENPATH
#define SCREENPATH  "/usr/local/share/wanderer/screens"
#endif
#ifndef HISCOREPATH
#define HISCOREPATH "/usr/local/share/wanderer/hiscore"
#endif
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
/* #define COMPARE_BY_NAME  // define this to compare by name, not uid         */
/* #define NO_ENCRYPTION // define this to disable the savefile encryptor */
#define NOISY                   /* do we want bells in the game ? */

                /****** OTHER PARAMETERS ******/

#define GUESTUID 0              /* guestuid always compared by name         */
#define EMSIZE 1024             /* size of editor moves memory              */
#define ENTRIES 15              /* size of hiscore file                     */

            /**** NOTHING TO CHANGE BELOW HERE ****/

/* MSDOS modifications (M001) by Gregory H. Margo        */
#ifdef        MSDOS
#define        R_BIN        "rb"        /* binary mode for non-text files */
#define        W_BIN        "wb"
#ifdef        VOIDPTR
#define VOIDSTAR        (void *)
#else
#define VOIDSTAR        (char *)
#endif
#define        ASKNAME          /* ask user's name if not in environment         */
#define        COMPARE_BY_NAME  /* compare users with name, not uid                */
#undef        getchar           /* remove stdio's definition to use curses'         */
#define        getchar()        getch() /* use curse's definition instead */

#else /* not MSDOS */
#define        R_BIN        "r"
#define        W_BIN        "w"
#define        VOIDSTAR
#endif
