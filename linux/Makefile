# platform-specific part
CC=gcc
CC_COMPILE=-c
EXENAMEFLAG=-o
RM=rm
RMOPT=-f
LN=ln
LNOPT=-s
OBJ=.o
EXE=
CFLAGS=-Wall -ggdb
DIRSEP=/


# platform-independant part
all: choose-language

OBJS=common$(OBJ) os$(OBJ) setup$(OBJ) lang$(OBJ)

choose-language:
	@echo ""
	@echo "Please choose a language:"
	@echo ""
	@echo "language                  command"
	@echo "--------------------------------------------------------------"
	@echo "deutsch                   make german"
	@echo "english                   make english"
	@echo ""

german: clean
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)setup.c setup.c
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)common.h common.h
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)lang.h lang.h
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)common.c common.c
	$(LN) $(LNOPT) ..$(DIRSEP)german$(DIRSEP)lang.c lang.c
	$(CC) $(CFLAGS) $(CC_COMPILE) common.c
	$(CC) $(CFLAGS) $(CC_COMPILE) lang.c
	$(CC) $(CFLAGS) $(CC_COMPILE) os.c
	$(CC) $(CFLAGS) $(CC_COMPILE) setup.c
	$(CC) $(CFLAGS) $(EXENAMEFLAG) setup$(EXE) $(OBJS)
	.$(DIRSEP)setup

english: clean
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)setup.c setup.c
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)common.h common.h
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)lang.h lang.h
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)common.c common.c
	$(LN) $(LNOPT) ..$(DIRSEP)english$(DIRSEP)lang.c lang.c
	$(CC) $(CFLAGS) $(CC_COMPILE) common.c
	$(CC) $(CFLAGS) $(CC_COMPILE) lang.c
	$(CC) $(CFLAGS) $(CC_COMPILE) os.c
	$(CC) $(CFLAGS) $(CC_COMPILE) setup.c
	$(CC) $(CFLAGS) $(EXENAMEFLAG) setup$(EXE) $(OBJS)
	.$(DIRSEP)setup

clean:
	-$(RM) $(RMOPT) lang.h
	-$(RM) $(RMOPT) lang.c
	-$(RM) $(RMOPT) common.h
	-$(RM) $(RMOPT) common.c
	-$(RM) $(RMOPT) setup$(EXE)
	-$(RM) $(RMOPT) setup.c
	-$(RM) $(RMOPT) *$(OBJ)
	-$(RM) $(RMOPT) *~

