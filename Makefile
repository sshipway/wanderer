# Makefile for wanderer - modified by Bill Randle 6/30/88
# modified again by play@cwi.nl
# and again by me.. maujp@uk.ac.warwick.cu
# and yet again by adb@bucsf.bu.edu
# Make clean - Marina Brown marina@surferz.net 2001,2

TOOLS = convert passwords
SRC_TOOLS = $(addsuffix .c,$(TOOLS))

SRCS = $(filter-out $(SRC_TOOLS),$(wildcard *.c))
OBJS = $(patsubst %.c,%.o,$(SRCS))

CC       ?= cc
CFLAGS   ?= -O -s
CFLAGS   += -std=c99 -Wall -Wextra
CPPFLAGS += -D_POSIX_C_SOURCE=199309L
LDLIBS   += -lncurses 

all: wanderer
.PHONY: all

wanderer: $(OBJS)

convert: CPPFLAGS += -D__CONVERT
convert: convert.c encrypt.c encrypt.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< encrypt.c -o $@

passwords: passwords.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.c %.h wand_head.h

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
.PHONY: install

clean:
	$(RM) $(OBJS)
.PHONY: clean

distclean:
	$(RM) $(OBJS)
	$(RM) wanderer
	$(RM) $(TOOLS)
.PHONY: distclean

uninstall:
	$(RM)    /usr/local/bin/wanderer
	$(RM) -r /usr/local/share/wanderer/
	$(RM)    /usr/share/man/man6/wanderer.6
.PHONY: uninstall
