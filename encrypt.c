/* File encrypt.c */

#include "wand_head.h"

/* Uses seeded random xor to encrypt because setkey doesnt work on our
   system.                                                                 */

crypt_file(name)
char *name;
{
char buffer[1024];
int fd,length,loop;

if((fd = open(name,O_RDONLY)) == -1) {
        endwin();
        sprintf(buffer,"Wanderer: cannot open %s",name);
        perror(buffer);
        exit(1);
}
if((length = read(fd,buffer,1024)) < 1) {
        endwin();
        sprintf(buffer,"Wanderer: read error on %s",name);
        perror(buffer);
        exit(1);
}
close(fd);

/* Right, got it in here, now to encrypt the stuff */

addstr("Running crypt routine....\n");
refresh();

srand(BLURFL);
for(loop=0;loop<length;loop++)
        buffer[loop]^=rand();

if((fd = open(name,O_WRONLY|O_TRUNC))== -1) {
        endwin();
        sprintf(buffer,"Wanderer: cannot write to %s",name);
        perror(buffer);
        exit(1);
}
if(write(fd,buffer,length)!=length) {
        endwin();
        sprintf(buffer,"Wanderer: write error on %s",name);
        perror(buffer);
        exit(1);
}
close(fd);

/* ok, file now contains encrypted/decrypted game. */
/* lets go back home... */
}
