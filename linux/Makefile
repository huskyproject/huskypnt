# platform-specific part
CC=gcc
CC_COMPILE=-c
EXENAMEFLAG=-o
RM=rm
RMOPT=-f
LN=ln
LNOPT=-s
CP=cp
CPOPT=
CFLAGS=-Wall -ggdb
DIRSEP=/
EXE=
OBJ=.o
LIBPREFIX=lib
LIBSUFFIX=.a


# platform-independant part
# please put OS-dependant things ONLY in the part ABOVE. This makes
# maintaining MUCH easier!
all:
	@echo ""
	@echo "Please use the Makefile in the directory above!"
	@echo ""

OBJS=common$(OBJ) os$(OBJ) setup$(OBJ) lang$(OBJ)

choose-language:

setlinks:
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)autocmp.c autocmp.c
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)setup.c setup.c
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)common.h common.h
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)lang.h lang.h
	$(LN) $(LNOPT) ..$(DIRSEP)common$(DIRSEP)common.c common.c
	$(LN) $(LNOPT) ..$(DIRSEP)lwpp$(DIRSEP)lwpp.c lwpp.c
	$(LN) $(LNOPT) ..$(DIRSEP)readline$(DIRSEP)testrl.c testrl.c
	$(LN) $(LNOPT) ..$(DIRSEP)readline$(DIRSEP)rlsubst.c rlsubst.c
	$(LN) $(LNOPT) ..$(DIRSEP)readline$(DIRSEP)rlsubst.h rlsubst.h
	$(CP) $(CPOPT) ..$(DIRSEP)common$(DIRSEP)auto.h auto.h

setgerman:
	$(LN) $(LNOPT) ..$(DIRSEP)german$(DIRSEP)lang.c lang.c

setenglish:
	$(LN) $(LNOPT) ..$(DIRSEP)english$(DIRSEP)lang.c lang.c

objects:
	$(CC) $(CFLAGS) $(CC_COMPILE) common.c
	$(CC) $(CFLAGS) $(CC_COMPILE) lang.c
	$(CC) $(CFLAGS) $(CC_COMPILE) os.c
	$(CC) $(CFLAGS) $(CC_COMPILE) setup.c

lwpp$(EXE):
	$(CC) $(CFLAGS) $(EXENAMEFLAG) lwpp$(EXE) lwpp.c

checkrl:
	-$(CC) $(CFLAGS) $(EXENAMEFLAG) testrl$(EXE) testrl.c -lreadline
	-.$(DIRSEP)testrl
	$(CC) $(CFLAGS) $(EXENAMEFLAG) autocmp$(EXE) autocmp.c
	.$(DIRSEP)autocmp rl 1 $(MAKE) withrl
	.$(DIRSEP)autocmp rl 0 $(MAKE) withoutrl

withrl:
	$(CC) $(CFLAGS) $(EXENAMEFLAG) setup$(EXE) $(OBJS) -lreadline
	.$(DIRSEP)setup

withoutrl:
	$(CC) $(CFLAGS) $(CC_COMPILE) rlsubst.c
	$(CC) $(CFLAGS) $(EXENAMEFLAG) setup$(EXE) $(OBJS) rlsubst$(OBJ)
	.$(DIRSEP)setup

afterlang: setlinks objects lwpp$(EXE) checkrl

german: clean setgerman afterlang 
english: clean setenglish afterlang 


clean:
	-$(RM) $(RMOPT) auto.h
	-$(RM) $(RMOPT) autocmp.c
	-$(RM) $(RMOPT) autocmp$(EXE)
	-$(RM) $(RMOPT) common.c
	-$(RM) $(RMOPT) common.h
	-$(RM) $(RMOPT) common$(OBJ)
	-$(RM) $(RMOPT) lang.c
	-$(RM) $(RMOPT) lang.h
	-$(RM) $(RMOPT) lang$(OBJ)
	-$(RM) $(RMOPT) lwpp.c
	-$(RM) $(RMOPT) lwpp$(EXE)
	-$(RM) $(RMOPT) os$(OBJ)
	-$(RM) $(RMOPT) rlsubst.c
	-$(RM) $(RMOPT) rlsubst.h
	-$(RM) $(RMOPT) rlsubst$(OBJ)
	-$(RM) $(RMOPT) setup.c
	-$(RM) $(RMOPT) setup$(EXE)
	-$(RM) $(RMOPT) testrl.c
	-$(RM) $(RMOPT) testrl$(EXE)
	-$(RM) $(RMOPT) *~
	-$(RM) $(RMOPT) *.bak

