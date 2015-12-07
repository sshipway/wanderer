# wanderer
Wanderer text-based boulderdash clone game

This program requires the header files string.h and curses.h, and
the curses and termcap libraries libcurses.a and libtermcap.a. If your
system doesn't have them then you might as well give up now.

Still here? Good - then everything's OK.

First thing to do is edit the wand_head.h file. Most of the filenames
are self-explanatory - the lock file is usually in /tmp , you probably
dont need to change it. SCREENPATH is the directory in which the screen
files, screen.* , are kept. HISCOREPATH is the name of the hiscore table
file - this will be created when you run the program, but must have group
and other read and write permission. Also, the directory SCREENPATH is used
to hold the credits file.

The masterpassword is the password that you can give when you try to
jump screen with ~ (see file wand.info) that will allow you to go to any
screen at will. This is for wanderer 'wizards' only. Separate passwords
for each screen are drawn from the file /usr/dict/words - if you dont have
it on your system then you'll have to change the file jump.c . These passwords
are given to the player upon completing each screen. The passwords can be
disabled by creating a file in the SCREENPATH directory called no_pws .

New screens can be created by using the editor (thanx play@nl.cwi) that
is built into the game. You enter it with the -e flag, and the screen you edit
is held in the file ./screen . To add this one to the others, place it in the
SCREENPATH directory with a name of the form screen.* , making sure that it is
readable by everyone and that the screens number is subsequent to the last of
the screens already held in the directory. Screens may be tested with
either 'p' or 'n', and by using 'm' the number of moves for the screen may be
altered.

The hiscore table holds only one entry per username - this is to
prevent one or two people from taking over the entire table.  If you object to
this feature, it is easily removed from the scores.c file. The table can also
be edited by the 'wizard' using the -m flag. This prompts for the
masterpassword before allowing you to remove any entry by typing its
number. Use '0' to exit the editor.

Not much more to say, really. Info on playing the game can be found in
the wand.info file. Once you have edited the header file, just type 'make'
to run the makefile. You may need to edit the makefile so that it conforms
with your system. If you dont know how to do this, there's bound to be
someone around who does.

##Compiling

Under Linux, you will need GCC, ncurses-devel, and make.

Edit `wand_head.h` and change at least HISCOREPATH, SCREENPATH, and anything 
else that looks wrong.  Make sure that the directory SCREENPATH exists and 
holds the screens.  Create an empty file for the hiscore file.

Run `make` and it should build in a second or two.  Ignore the various warnings.

Now you should be able to run `wanderer`.  If the screens are unavailable 
it will give you an error.  

There seems to be something weird in the environment variable retrieval that doesnt work
properly under 64bit GCC; you get an invalid pointer for the name which segfaults
when writing the highscore or the goodbye message.

##Environment variables

Wanderer uses several of these. Here they are...

    SAVEFILE ---- the path of the file to be used for saved games
    NEWKEYS  ---- if you want to redefine the keys
    NEWNAME, NAME, USER ---- scanned in that order to get a name for the hiscore
                         table.

The MSDOS port and save routines are courtesy of Greg Margo, who has also
tidied up a lot of my messy programming :-). Any problems with them, send
to HIM, since I wont know what to do.

All that remains is for me to say that the source was written by me,
Steven Shipway,  on a sun/3-160 called poppy at Warwick University. The
latter are completely unaware of this major breakthrough in software
design (:-), and this is probably a good idea, since they may well disapprove
of this use of computing time and facilities! (Only kidding)

##Summary of flags:

    -c  Reads credits file from SCREENPATH/credits
    -s  Shows hiscore table from HISCOREPATH
    -e  Enters screen editor working on file ./screen
    -m  Enters hiscore table editor after prompting for the MASTERPASSWORD
    -f  Starts game with map-type screen instead of normal screen - although I
        personally think this makes the game too easy...
    -v  Prints version
    -t  Tests the screen
    -r Record mode. Records keypresses into the given file
