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
CFLAGS=-Wall
DIRSEP=/


# platform-independant part
all: choose-language

choose-language:
	@echo ""
	@echo "Please choose a language:"
	@echo ""
	@echo "deutsch                   make german"
	@echo "english                   make english"
	@echo ""

german: clean
	$(LN) $(LNOPT) ..$(DIRSEP)lang$(DIRSEP)german.h lang.h
	$(LN) $(LNOPT) ..$(DIRSEP)setup.c setup.c
	$(CC) $(CFLAGS) $(CC_COMPILE) os.c
	$(CC) $(CFLAGS) $(CC_COMPILE) setup.c
	$(CC) $(CFLAGS) $(EXENAMEFLAG) setup$(EXE) os$(OBJ) setup$(OBJ)
	.$(DIRSEP)setup

english: clean
	$(LN) $(LNOPT) ..$(DIRSEP)lang$(DIRSEP)english.h lang.h
	$(LN) $(LNOPT) ..$(DIRSEP)setup.c setup.c
	$(CC) $(CFLAGS) $(CC_COMPILE) os.c
	$(CC) $(CFLAGS) $(CC_COMPILE) setup.c
	$(CC) $(CFLAGS) $(EXENAMEFLAG) setup$(EXE) os$(OBJ) setup$(OBJ)
	.$(DIRSEP)setup

clean:
	-$(RM) $(RMOPT) lang.h
	-$(RM) $(RMOPT) setup$(EXE)
	-$(RM) $(RMOPT) setup.c
	-$(RM) $(RMOPT) *$(OBJ)
	-$(RM) $(RMOPT) *~

