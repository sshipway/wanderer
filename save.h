#ifndef __SAVE_H
#define __SAVE_H

/* Save and Restore game additions (M002) by Gregory H. Margo        */

struct save_vars
{
    int z_x, z_y, z_sx, z_sy, z_tx, z_ty, z_mx, z_my, z_diamonds, z_nf;
};

struct old_save_vars
{
    int z_x, z_y, z_nx, z_ny, z_sx, z_sy, z_tx, z_ty, z_lx, z_ly,
        z_mx, z_my, z_bx, z_by, z_nbx, z_nby, z_max_score, z_diamonds, z_nf,
        z_hd, z_vd, z_xdirection, z_ydirection;
};

#ifndef __CONVERT

void restore_game(int *num, long *score, int *bell, int *maxmoves);
void save_game(int num, long *score, int *bell, int maxmoves);

#endif // __CONVERT

#endif // __SAVE_H
