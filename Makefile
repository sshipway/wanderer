# Makefile for wanderer - modified by Bill Randle 6/30/88
# modified again by play@cwi.nl
# and again by me.. maujp@uk.ac.warwick.cu
# and yet again by adb@bucsf.bu.edu
# Make clean - Marina Brown marina@surferz.net 2001,2

PROGRAM = wanderer

TOOLS = convert passwords
SRC_TOOLS = $(addsuffix .c,$(TOOLS))

SRCS = $(filter-out $(SRC_TOOLS),$(wildcard *.c))
OBJS = $(patsubst %.c,%.o,$(SRCS))

CC       ?= cc
CFLAGS   ?= -O -s
CFLAGS   += -std=c99 -Wall -Wextra
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDLIBS   += -lncurses 

INSTALL      = install
INSTALL_BIN  = $(INSTALL) -D -m 755
INSTALL_DATA = $(INSTALL) -D -m 644
INSTALL_MAN  = $(INSTALL) -D -m 644

PREFIX    = /usr/local
BIN_DIR   = $(PREFIX)/bin
SHARE_DIR = $(PREFIX)/share
MAN_DIR   = $(PREFIX)/share/man
MAN1_DIR  = $(MAN_DIR)/man1
MAN6_DIR  = $(MAN_DIR)/man6

WANDERERPATH = $(SHARE_DIR)/wanderer
SCREENPATH   = $(SHARE_DIR)/wanderer/screens
HISCOREPATH  = $(SHARE_DIR)/wanderer/hiscore

CPPFLAGS += -DPREFIX=$(PREFIX)                 \
	    -DSCREENPATH='"$(SCREENPATH)"'     \
	    -DHISCOREPATH='"$(HISCOREPATH)"'


all: $(PROGRAM)
.PHONY: all

$(PROGRAM): $(OBJS)

convert: CPPFLAGS += -D__CONVERT
convert: convert.c encrypt.c encrypt.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< encrypt.c -o $@

passwords: passwords.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.c %.h wand_head.h

install: 
	$(INSTALL_DATA) -t $(SCREENPATH) screens/*
	touch $(HISCOREPATH)
	[ $(USER) = root ] && chown root:games $(HISCOREPATH) || true
	[ $(USER) = root ] && chmod g+w $(HISCOREPATH) || true
	$(INSTALL_BIN) $(PROGRAM) $(BIN_DIR)/$(PROGRAM)
	[ $(USER) = root ] && chown root:games $(BIN_DIR)/$(PROGRAM) || true
	[ $(USER) = root ] && chmod g+s $(BIN_DIR)/$(PROGRAM) || true
	$(INSTALL_MAN) wanderer.6 $(MAN6_DIR)
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
	$(RM)    $(BIN_DIR)/$(PROGRAM)
	$(RM) -r $(WANDERERPATH)
	$(RM)    $(MAN6_DIR)wanderer.6
.PHONY: uninstall

test: wanderer
	touch hiscore
	echo -e "#hljkSsavegame\n" > keypress
	HISCOREPATH=hiscore ./wanderer < keypress
	./passwords 1000


