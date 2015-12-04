/* file icon.c */

#include "wand_head.h"

/********************************************************
*                     draw_symbol                       *
*********************************************************/
void draw_symbol(x,y,ch)      /* this is where the pretty graphics are   */
                              /* all defined - change them if you want.. */
int  x,y;
char ch;
{
    char icon[2][4],
         (*iconrow)[4] = icon;
    switch(ch)
    {
    case ' ':                    /*  space  */
        strcpy(*iconrow++,"   ");
        strcpy(*iconrow,  "   ");
        break;
    case '#':                   /*  rock  */
        strcpy(*iconrow++,"###");
        strcpy(*iconrow,  "###");
        break;
    case '<':                   /*  arrows  */
        strcpy(*iconrow++,"<--");
        strcpy(*iconrow,  "<--");
        break;
    case '>':
        strcpy(*iconrow++,"-->");
        strcpy(*iconrow,  "-->");
        break;
    case 'O':                    /* boulder  */
        strcpy(*iconrow++,"/^\\");
        strcpy(*iconrow,  "\\_/");
        break;
    case ':':                    /*  earth  */
        strcpy(*iconrow++,". .");
        strcpy(*iconrow,  " . ");
        break;
    case '/':                    /*  slopes */
        strcpy(*iconrow++," _/");
        strcpy(*iconrow,  "/  ");
        break;
    case '\\':
        strcpy(*iconrow++,"\\_ ");
        strcpy(*iconrow,  "  \\");
        break;
    case '*':                     /*  diamond  */
        strcpy(*iconrow++,"/$\\");
        strcpy(*iconrow,  "\\$/");
        break;
    case '=':                     /*  rock  */
        strcpy(*iconrow++,"=-=");
        strcpy(*iconrow,  "-=-");
        break;
    case '@':                     /*  YOU!!! */
        strcpy(*iconrow++," o ");
        strcpy(*iconrow,  "<|>");
        break;
    case '~':                     /* pushable thingy */
         strcpy(*iconrow++,"\\|/");
         strcpy(*iconrow  ,"/|\\");
         break;
    case 't':
    case 'T':                    /*  teleport  */
        strcpy(*iconrow++,"(*)");
        strcpy(*iconrow,  "(*)");
        break;
    case 'X':                    /*  exits  */
        strcpy(*iconrow++,"Way");
        strcpy(*iconrow,  "Out");
        break;
    case '!':                    /*  landmine  */
        strcpy(*iconrow++," I ");
        strcpy(*iconrow,  " o ");
        break;
    case 'M':                     /* big monster  */
        strcpy(*iconrow++,"}o{");
        strcpy(*iconrow,  "/^\\");
        break;
    case 'S':                     /* baby monster */
        strcpy(*iconrow++,"-o-");
        strcpy(*iconrow,  "/*\\");
        break;
    case '^':                      /* balloon */
        strcpy(*iconrow++,"/~\\");
        strcpy(*iconrow,  "\\_X");
        break;
    case 'C':                      /* time capsule */
        strcpy(*iconrow++,"   ");
        strcpy(*iconrow,  "<O>");
        break;
    case '+':                      /* cage */
        strcpy(*iconrow++,"TTT");
        strcpy(*iconrow,  "III");
        break;
    case '_':                      /* more wall characters */
        strcpy(*iconrow++,"_|_");
         strcpy(*iconrow  ,"_|_");
         break;
    case 'B':                      /* the bomb */
         strcpy(*iconrow++,"/\\*");
         strcpy(*iconrow  ,"\\/ ");
          break;
    case '%':                      /* the explosion */
         strcpy(*iconrow++,"Ba\\");
         strcpy(*iconrow  ,"\\ng");
          break;
    default:                       /* this is what it uses if it doesnt */
                                   /* recognise the character  */
        strcpy(*iconrow++,"OoO");
        strcpy(*iconrow,  "oOo");
        break;
    };
    move(y+1,x+1);
    iconrow--;
    addstr(*iconrow++);
    move(y+2,x+1);
    addstr(*iconrow);
}
