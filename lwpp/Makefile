SHELL		= /bin/sh

NAME		= lwpp
VERSION		= 1.0#
DATE		= 1999-Aug-15#

DESTDIR		=#
PREFIX		= /usr/local
BINDIR		= $(PREFIX)/bin
MANDIR		= $(PREFIX)/man/man1
SBINDIR		= $(PREFIX)/sbin
ETCDIR		= /etc

CAT		= cat
CP		= cp -fp
CHMOD		= chmod
GZIP		= gzip -9
INSTALL		= install
MKDIR		= mkdir
RM		= rm -f
SED		= sed
TAR		= tar

INSTALL_BIN	= $(INSTALL) -m 0755 -s
INSTALL_DATA	= $(INSTALL) -m 0644
INSTALL_LIB	= $(INSTALL) -m 0644
INSTALL_MAN	= $(INSTALL) -m 0444
INSTALL_SCRIPT	= $(INSTALL) -m 0755
MKDIRS		= $(MKDIR) -p
#INSTALL_BIN	= $(CP)
#INSTALL_DATA	= $(CP)
#INSTALL_LIB	= $(CP)
#INSTALL_MAN	= $(CP)
#INSTALL_SCRIPT	= $(CP)
#MKDIRS		= $(INSTALL) -d

CC		= gcc
OPTFLAGS	= -O2 -fno-strength-reduce
WARNFLAGS	= -Wall -pedantic
MISCFLAGS	=#
DEFS		= -DVERSION='"$(VERSION)"' -DDATE='"$(DATE)"'
CFLAGS		= $(OPTFLAGS) $(WARNFLAGS) $(MISCFLAGS) $(DEFS)
LDFLAGS		=#

NAME		= lwpp

INCS		=#
SRCS		= $(NAME).c
OBJS		= $(SRCS:.c=.o)

MAN_SRC		=# $(NAME).1.in
MAN_OBJ		=# $(NAME).1

SPEC_SRC	= $(NAME).spec.in
SPEC_OBJ	= $(NAME).spec

DOCS		= INSTALL.txt Lwpp-HOWTO.txt README.txt
EXTRAS		=#
MAKEFILES	= Makefile
SAMPLES		= tmp.txt tmp2.txt

DIST_DIR	= $(NAME)-$(VERSION)
DIST		= $(DIST_DIR).tar.gz
DIST_FILES	= \
 $(DOCS) \
 $(MAKEFILES) \
 $(EXTRAS) \
 $(SAMPLES) \
 $(SPEC_SRC) \
 $(SPEC_OBJ) \
 $(SRCS) \
 $(INCS) \
 $(MAN_SRC)

.PHONY: all sed man spec install extra install-extra
.PHONY: clean realclean spotless distclean
.SUFFIXES:
.SUFFIXES: .c .o

#all: $(NAME) man
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

#man: $(MAN_OBJ)
#
#$(MAN_OBJ): $(MAN_SRC)
#	$(CAT) $< | $(SED) \
#		-e 's~@VERSION@~$(VERSION)~g' \
#		-e 's~@DATE@~$(DATE)~g' \
#		>$@

sed: spec
spec: $(SPEC_OBJ)

$(SPEC_OBJ): $(SPEC_SRC)
	$(CAT) $< | $(SED) \
		-e 's~@NAME@~$(NAME)~g' \
		-e 's~@VERSION@~$(VERSION)~g' \
		-e 's~@DOCS@~$(DOCS)~g' \
		-e 's~@SAMPLES@~$(SAMPLES)~g' \
		>$@

install: $(NAME) $(DESTDIR)$(BINDIR) #$(DESTDIR)$(MANDIR)
	$(INSTALL_BIN) $(NAME) $(DESTDIR)$(BINDIR)/
#	$(INSTALL_MAN) $(MAN_OBJ) $(DESTDIR)$(MANDIR)/

$(DESTDIR)$(BINDIR):
	$(MKDIRS) $@

#$(DESTDIR)$(MANDIR):
#	$(MKDIRS) $@
#
#$(DESTDIR)$(SBINDIR):
#	$(MKDIRS) $@
#
#$(DESTDIR)$(ETCDIR):
#	$(MKDIRS) $@

clean:
	$(RM) $(NAME) $(OBJS) $(MAN_OBJ)

realclean: clean
	$(RM) $(SPEC_OBJ) *~ *.LOG

distclean: spotless
spotless: realclean
	$(RM) $(DIST)

dist: tar
tar: spec
	$(RM) -r ./$(DIST_DIR)
	$(MKDIR) ./$(DIST_DIR)
	$(CHMOD) 0755 ./$(DIST_DIR)
	$(CP) $(DIST_FILES) ./$(DIST_DIR)/
	$(CHMOD) 0644 ./$(DIST_DIR)/*
	$(TAR) cvf - ./$(DIST_DIR) | $(GZIP) >./$(DIST)
	$(CHMOD) 0644 ./$(DIST)
	$(RM) -r ./$(DIST_DIR)

