# Makefile for wanderer - modified by Bill Randle 6/30/88
# modified again by play@cwi.nl
# and again by me.. maujp@uk.ac.warwick.cu
# and yet again by adb@bucsf.bu.edu
# Make clean - Marina Brown marina@surferz.net 2001,2

OBJ = monsters.o m.o save.o jump.o display.o icon.o game.o read.o help.o fall.o scores.o edit.o encrypt.o

CFLAGS = -O -s
#CFLAGS = -g
LIBS = -lncurses 
CC = cc

all:	wanderer
	@echo DONE

wanderer:	$(OBJ)
	$(CC) $(CFLAGS) -o wanderer $(OBJ) $(LIBS)

convert: convert.c wand_head.h
	$(CC) $(CFLAGS) -o convert convert.c

$(OBJ): wand_head.h

install: 
	mkdir /usr/local/share/wanderer
	mkdir /usr/local/share/wanderer/screens
	cp screens/* /usr/local/share/wanderer/screens
	touch /usr/local/share/wanderer/hiscore
	chown root.game /usr/local/share/wanderer/hiscore
	chmod g+w /usr/local/share/wanderer/hiscore
	cp wanderer /usr/local/bin
	chown root.game /usr/local/bin/wanderer
	chmod +x /usr/local/bin/wanderer
	chmod g+s /usr/local/bin/wanderer
	cp wanderer.6 /usr/share/man/man6

clean:
	rm *.o

distclean:
	rm *.o
	rm wanderer
	rm convert

uninstall:
	rm /usr/local/bin/wanderer
	rm -rf /usr/local/share/wanderer/
	rm -f /usr/share/man/man6/wanderer.6
