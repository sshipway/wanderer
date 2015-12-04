/* file convert.c */

/*************************************************************************
*         There are some serious inconsistancies within this program     *
*         Since it is not likly that anyone will have V2 files avail     *
*         I will not attempt to change this file unless asked.           *
*               - Marina Brown       marina@surferz.net (12/2002)        *
**************************************************************************/

#include "wand_head.h"
#include <errno.h>

/*
    This program converts V2.* save files into V3.* save files.

    It can do this for compressed/ uncompressed files, and defaults
    to the def in wand_head.h

    Usage:

        convert [ -v ] [ -C | -N ] [ -c | -n ] oldfile newfile

    -v:  verbose
    -C etc : force encryption / no encryption on OLDFILE or newfile.

*/

/**********************************************
*          variable declarations              *
***********************************************/
char screen[NOOFROWS][ROWLEN+1];
char screen_name[ROWLEN+1];
char *infile, *outfile;

struct saved_game
{
    short   num;
    long    score;
    short   bell;
    short   maxmoves;
    short   num_monsters;
};

struct    save_vars    zz;
struct  old_save_vars   yy;

int num, bell, maxmoves;
long score;
struct mon_rec  *last_of_list, *start_of_list, *tail_of_list;

int o_enc, n_enc;
int verbose;

/************************************************
*                function crypt_file            *
*************************************************/
crypt_file(name)
char *name;
{
    char buffer[1024];
    int fd,length,loop;

    if((fd = open(name,O_RDONLY)) == -1)
    {
        sprintf(buffer,"Wanderer: cannot open %s",name);
        perror(buffer);
        exit(1);
    } 
    if((length = read(fd,buffer,1024)) < 1) {
        sprintf(buffer,"Wanderer: read error on %s",name);
        perror(buffer);
        exit(1);
    }
    close(fd);

    /* Right, got it in here, now to encrypt the stuff */

    srand(BLURFL);
    for(loop=0;loop<length;loop++)
        buffer[loop]^=rand();

    if((fd = open(name,O_WRONLY|O_TRUNC))== -1) {
        sprintf(buffer,"Wanderer: cannot write to %s",name);
        perror(buffer);
        exit(1);
    }
    if(write(fd,buffer,length)!=length) {
        sprintf(buffer,"Wanderer: write error on %s",name);
        perror(buffer);
        exit(1);
    }
    close(fd);

    /* ok, file now contains encrypted/decrypted game. */
    /* lets go back home...                            */
}

/*****************************************************
*               function make_monster                *
******************************************************/
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

/*************************************************
*               function save_game               *
**************************************************/
void save_game()
{
    char    fname[128], buf[70], *fp;
    FILE    *fo;
    struct    saved_game    s;
    extern    char    *getenv();
    struct    mon_rec    *mp;

    fp = outfile;

    if ((FILE *)NULL == (fo = fopen(outfile, W_BIN))) {
        perror(fp);
        exit(1);
    }

    s.num = num;
    s.score = score;
    s.bell = bell;
    s.maxmoves = maxmoves;
    s.num_monsters = 0;

    mp = start_of_list;        /* first entry is dummy    */
    while (mp != tail_of_list) {
        mp = mp->next;
        s.num_monsters++;    /* count them monsters    */
    }

    if ( (1 != fwrite((char *)&s, sizeof(s), 1, fo)) ||
         (1 != fwrite((char *)screen, sizeof(screen), 1, fo)) ||
         (1 != fwrite((char *)&zz, sizeof(zz), 1, fo)) )
    {
        printf("Write error on '%s'\n", fname);
        fclose(fo);
        unlink(fname);
        exit(1);
    }

    mp = start_of_list;
    while (mp != tail_of_list) {
        /* save them monsters    */
        mp = mp->next;
        if (1 != fwrite((char *)mp, sizeof(struct mon_rec), 1, fo)) {
            printf("Write error on '%s'\n", fname);
            fclose(fo);
            unlink(fname);
            exit(1);
        }
    }
    fwrite(screen_name,sizeof(char),strlen(screen_name),fo);
    fclose(fo);
    if( n_enc )
        crypt_file(outfile,0);   /* encrpyt the saved game */
    printf("Game saved.\n\nWanderer Copyright (C) 1988 S Shipway\n\n");
}

/*******************************************************
*                function restore_game                 *
********************************************************/
void restore_game()
{
    FILE    *fi;
    struct    saved_game    s;
    struct    mon_rec    *mp, *tmp, tmp_monst;
    char    fname[128], *fp;
    FILE    *fo;
    extern    char    *getenv();

    fp = infile;

    if( o_enc )
         crypt_file(infile,1);   /* decrypt it */
    if ((FILE *)NULL == (fi = fopen(infile, R_BIN))) {
        printf("Open error on '%s'\n", fp);
        printf("Cannot restore game --- sorry.\n");
        exit(1);
    }
    if ( (1 != fread((char *)&s, sizeof(s), 1, fi)) ||
         (1 != fread((char *)screen, sizeof(screen), 1, fi)) ||
         (1 != fread((char *)&yy, sizeof(yy), 1, fi)) ) {
        printf("Read error on '%s'\n", fp);
        printf("Cannot restore game --- sorry.\n");
        fclose(fi);
        exit(1);
    }

    num = s.num;
    score = s.score;
    bell = s.bell;
    maxmoves = s.maxmoves;

    /* free any monsters already on chain, to start clean */
    mp = start_of_list->next;
    while ((mp != NULL) && (mp != start_of_list)) {
        /* free them monsters    */
        tmp = mp;
        mp = mp->next;
        free(tmp);
    }

    /* re-initialize the monster list    */
    /* *start_of_list = {0,0,0,0,0,NULL,NULL}; */
    start_of_list->x = 0;
    start_of_list->y = 0;
    start_of_list->mx = 0;
    start_of_list->my = 0;
    start_of_list->under = 0;
    start_of_list->next = (struct mon_rec *)NULL;
    start_of_list->prev = (struct mon_rec *)NULL;

    tail_of_list = start_of_list;

    while (s.num_monsters--) {
        /* use make_monster to allocate the monster structures    */
        /* to get all the linking right without even trying    */
        if ((struct mon_rec *)NULL == (mp = make_monster(0, 0))) {
            printf("Monster alloc error on '%s'\n", fp);
            printf("Try again - it might work.\nBut then,pigs might fly...\n");
            fclose(fi);
            exit(1);
        }
        if (1 != fread((char *)&tmp_monst, sizeof(struct mon_rec), 1, fi)) {
            printf("Monster read error on '%s'\n", fp);
            printf("Cannot restore game --- sorry.\n");
            fclose(fi);
            exit(1);
        }
        /* copy info without trashing links    */
        mp->x     = tmp_monst.x;
        mp->y     = tmp_monst.y;
        mp->mx    = tmp_monst.mx;
        mp->my    = tmp_monst.my;
        mp->under = tmp_monst.under;
    }
    fclose(fi);
}

/*************************************************************
*         external globals - move to top  Marina             *
**************************************************************/
extern int opterr,optind;
extern char *optarg;

/**************************************************************
*                         main program                        *
***************************************************************/
main(argc,argv)
int argc;
char **argv;
{
    char c;
    struct mon_rec mlist;

    start_of_list = &mlist;

    verbose = 0;
#ifdef NO_ENCRYPTION
    o_enc = n_enc = 0;
#else
    o_enc = n_enc = 1;
#endif

    while( ( c = getopt(argc,argv,"CNcnv") ) != -1 )
    switch ( c ) 
    {
        case 'C': o_enc = 1; break;
        case 'c': n_enc = 1; break;
        case 'N': o_enc = 0; break;
        case 'n': n_enc = 0; break;
        case 'v': verbose = 1; break;
        default : printf("Usage: %s [ -v ] [ -C | -c ] [ -N | -n ] oldfile newfile\n",argv[0]);
            printf("-v : verbose\n");
            printf("-C : file is encrypted\n -N : file not encrypted\n");
            printf("Upper case -- old file, lower case -- new file\n");
            exit(1);
    }

    if( (argc - optind) < 2 )
    {
        printf("Usage: %s [ -v ] [ -C | -c ] [ -N | -n ] oldfile newfile\n",argv[0]);
        printf("-v : verbose\n");
        printf("-C : file is encrypted\n -N : file not encrypted\n");
        printf("Upper case -- old file, lower case -- new file\n");
        exit(1);
    }


    infile = argv[optind++]; outfile = argv[optind];
    if(verbose) printf("Converting %s to %s.\n",infile,outfile);

    if(verbose )
    {
        printf( "Reading in file %s. ", infile);
        if( o_enc ) printf("(encrypted)");
        printf("\n");
    }

    restore_game();

    if( verbose ) printf("Giving screen name.\n");

    sprintf(screen_name,"------ Wanderer version %s ------",VERSION);

    if( verbose ) printf(" Copying struct variables... \n");
    
    zz.z_x = yy.z_x;
    zz.z_y = yy.z_y;
    zz.z_tx = yy.z_tx;
    zz.z_ty = yy.z_ty;
    zz.z_mx = yy.z_mx;
    zz.z_my = yy.z_my;
    zz.z_sx = yy.z_sx;
    zz.z_sy = yy.z_sy;
    zz.z_diamonds = yy.z_diamonds;
    zz.z_nf = yy.z_nf;

    if(verbose)
    {
        printf( "Saving to file %s. ", outfile);
        if( n_enc ) printf("(encrypted)");
        printf("\n");
    }

    save_game();

    printf("Done.\n");
}
