#ifndef __MONSTERS_H
#define __MONSTERS_H

/* for monster movement */

#define viable(x,y) (((screen[y][x] == ' ') || (screen[y][x] == ':') ||\
        (screen[y][x] == '@') || (screen[y][x] == '+') ||\
        (screen[y][x] == 'S')) && (y >= 0) &&\
        (x >= 0) && (y < NOOFROWS) && (x < ROWLEN))

struct mon_rec
{
    int x, y, mx, my;
    char under;
    struct mon_rec *next, *prev;
};

struct mon_rec *make_monster(int x, int y);

int move_monsters(int *mxp, int *myp, long *score, char *howdead,
                  int sx, int sy, int nf, int bell, int x, int y,
                  int diamonds);

#endif // __MONSTERS_H
