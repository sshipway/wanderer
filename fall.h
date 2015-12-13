#ifndef __FALL_H
#define __FALL_H

int bang(int x, int y, int *mx, int *my, int sx, int sy, char *howdead);
int check(int *mx, int *my, int x, int y, int dx, int dy, int sx,
          int sy, char howdead[25]);
int fall(int *mx, int *my, int x, int y, int sx, int sy, char howdead[25]);

#endif // __FALL_H
