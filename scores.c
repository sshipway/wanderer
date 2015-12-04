/* file scores.c */

#include "wand_head.h"

#ifdef  MSDOS        /* M001 */

#define LOCK
#define UNLOCK

#else
/*#define LOCK     flock( fp->_cnt , LOCK_EX ) /* peeked at stdio.h ! */
/*#define UNLOCK   flock( fp->_cnt , LOCK_UN ) /*                     */
/* #define LOCK while((lock = creat(LOCKFILE,0))==-1) Not a good way  */

#define LOCK if((lock = open(LOCKFILE, O_CREAT | O_TRUNC | O_WRONLY, "r"))==-1){errx (1,"%s","Lockfile creation failed\n"); exit(1);} /* Marina */

#define UNLOCK unlink(LOCKFILE)

#endif

#ifndef   MSDOS        /* M001 */
extern int getuid();
#else
#define   getuid()        0
#endif

#ifdef    COMPARE_BY_NAME
#define   SAMEUSER(p)        (strcmp((p)->name, name) == 0)
#else
#define   SAMEUSER(p)        ((p)->uid == user_id)
#endif

extern int saved_game;  /* prevent recording of hiscore if  */
                        /* NO_RESTORED_GAME_HISCORES is #def'd */

/***********************************
*       score_entry structure      *
************************************/
typedef struct
{
    char howdead[25];
    char name[20];
    int  score;
    int  level;
    int  uid;
} score_entry;

#ifdef LINT_ARGS  /* M001 */
void show_scores(score_entry *,int );
int readtable(score_entry *);
#else
void show_scores();
int readtable();
#endif

/**/
/************************
*      show_scores      *
*************************/
void show_scores(score_entry *table,int num)
{
    int tot = num;

    printf("\nNo. Score Level           Names                 How they died\n");
    printf("=============================================================================\n");
    while(num > 0)
    {
        num--;
        printf("%2d %5d %3d      %-20s     killed by %-s\n",(tot - num),table->score,table->level,table->name,table->howdead);
        table++;
    }
    printf("\n\n");
}

/**/
/*************************
*   readtable function   *
**************************/
int readtable(score_entry *table_ptr)
{
    FILE *fp;
    int  numread;

    if((fp = fopen(HISCOREPATH,R_BIN)) == NULL)
    {
        numread = 0;
    }
    else
    {
        numread = fread( VOIDSTAR table_ptr, sizeof(score_entry), ENTRIES, fp);
        fclose(fp);
    }
    return numread;
}

/**/
/********************
*     savescore     *
*********************/

int  savescore(char *howdead,int score,int level,char *name)
{
    score_entry table[ENTRIES + 2],
                *table_ptr = table,
                new_entry,
                temp_entry;
    int numread;
    int index = 1; 
    int numsaved;
    int lock; 
    int already = 0; 
    int output_value = 1;
    int user_id; 
    int position;

    FILE *fp;

#ifdef NO_RESTORED_GAME_HISCORES

    if(saved_game)
    {
        printf("No hiscores recorded from restored games.\n");
        printf("\nWanderer (C) 1988  S.Shipway.\n\n");
        return 1;
    }
#endif

    user_id = getuid();
    strncpy(new_entry.howdead,howdead,25);
    new_entry.howdead[24] = '\0'; /* M002 strncpy does not null terminate */
    strncpy(new_entry.name,name,20);
    new_entry.name[19] = '\0'; /* M002 strncpy does not null terminate */
    new_entry.score = score;
    new_entry.level = level;
    new_entry.uid = user_id;
    LOCK;    /* BUG is HERE !!! */
    numread = readtable(table_ptr);
    if (numread > 0)
        if(table[numread-1].score > 99999) /* stop system errors messing it up*/
        {
            numread--;
            printf("Erasing spurious entry in table.\n");
        }
    if(score == -1)
    {
        show_scores(table,numread);
        UNLOCK;
        return 0;
    }
/* HERE */
    if (numread > 0)
    {
        numread++; /* scan through until correct insertion point */
        /* pass table entries with higher scores */
        while((table_ptr->score > score)&&(index < numread))
        {
            if(SAMEUSER(table_ptr))
            {
                already = 1;
                break;
            }
            table_ptr++;
            index++;
        }
       /* pass table entries with equal score but higher or equal level */

        while((table_ptr->level>=level)&&
            (index<numread)&&
            (table_ptr->score==score))
        {
            if(SAMEUSER(table_ptr))
            {
                already = 1;
                break;
            }
            table_ptr++;
            index++;
        }
        position = index;
        /* if already found: done */
       if(already == 1)
       {
           numread--;
           UNLOCK;
           return numread;
       }
    /* shift down score list */
       while(index < numread)
       {
           /* swap *table_ptr and new_entry */
           temp_entry = *table_ptr;
           *table_ptr = new_entry;
           new_entry = temp_entry;
           if(SAMEUSER(&new_entry))
           {
               already = 1;
               numread--; /* an older entry found */
               break;
           }
           table_ptr++;
           index++;
       }
       /* if all shifted without finding an older entry */
       if(already==0)
           *table_ptr = new_entry;
       if( position <= ENTRIES )
           printf("You rank %d in the hiscore table.\n",position);
       else
           printf("Sorry, you didnt make it into the hiscore table this time.\n");
    }
    else
    {
        printf("\nCreating new hiscore table.\n\n");
        *table_ptr = new_entry;
        numread++;
    }
    numread = ( (numread > ENTRIES) ? ENTRIES : numread );
    
    if((fp = fopen(HISCOREPATH,W_BIN)) != NULL)
    {
        table_ptr = table;
        numsaved = fwrite(VOIDSTAR table_ptr, sizeof(score_entry), numread, fp);
        chmod(HISCOREPATH,0666);
        if(numsaved < numread)
        {
            printf("ERROR! Only %d items saved from %d !\n",numsaved,numread);
            output_value = 0;
        }
        fclose(fp);
    }
    else
        err(1,"%s","Error in savescore - fopen HISCOREPATH failed\n");
    
    UNLOCK;
    return output_value;
}

/**/
/*************************
*      delete_entry      *
**************************/
void delete_entry(int num)
{
    score_entry table[ENTRIES + 22],
                  *table_ptr = table,
                new_entry,temp_entry;
    int  numread,index = 1, numsaved, lock, output_value = 1;
    FILE *fp;

    LOCK;
    numread = readtable(table_ptr);
    if (numread == 0) {
        printf("Missing or unreadable hiscore table.\n\n");
        UNLOCK;
        exit(1);
    }
    if (num > numread) {
        printf("Invalid entry, choose again\n");
        UNLOCK;
        return;
    }
    while(index < num)
    {
        index++;
        table_ptr++;
    }
    while(index < numread)
    {
        index++;
        *table_ptr = *(table_ptr+1);
        table_ptr++;
    }
    numread--;
    
    if((fp = fopen(HISCOREPATH,W_BIN)) != NULL)
    {
        table_ptr = table;
        numsaved = fwrite(VOIDSTAR table_ptr, sizeof(score_entry), numread, fp);
        chmod(HISCOREPATH,0666);
        if(numsaved < numread)
        {
            printf("ERROR! Only %d items saved from %d !\n",numsaved,numread);
            output_value = 0;
        }
        fclose(fp);
    }
    else
        err(1, "%s", "Could not open Scorefile\n");
    UNLOCK;
    show_scores(table,numsaved);
}

/**/
/************************
*      erase_scores     *
*************************/
erase_scores()
{
    int erasenum;
    int numread;
    int index = 0;
    char correct[20], c;
    score_entry table[ENTRIES + 2], *table_ptr = table;

    printf("Please enter password:");
    while((c = getchar()) != '\n' && index <19)
    {
        correct[index++] = c;
    }
    correct[index] = 0;
    if(strcmp(correct,MASTERPASSWORD))
    {
        printf("\nFoo, charlatan!\n");
        return 0;
    }
    numread = readtable(table_ptr);
    show_scores(table,numread);
    printf("\n");

    for(;;)
    {
        printf("Number to erase (0 to exit): ");
        scanf("%d",&erasenum);
        printf("\n");
        if(erasenum == 0)
            break;
        delete_entry(erasenum);
        printf("\n");
    }
    printf("Byee!\n");
}
