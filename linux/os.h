/* platform-specific header file, linux version */

#ifndef __OS_H__
#define __OS_H__

#define UNIX
#define LINUX
#define OS "Linux"
#define dirSepC '/'
#define dirSepS "/"

extern char *defaults[numIdx+1];

// returns 0 if file exists
int fexist(char *fname);

// returns 0 if directory exists
int direxist(char *dname);

// returns 0 if successfull
int copyFile(char *sourceName, char *destName);

// returns 0 if successfull
int touchFile(char *fname);

// returns 0 if successfull
int linkFile(char *source, char *dest);

// display a file
void showFile(char *fname);

// get the contents of a file
char *readFile(char *fname);

// create directory and parent directories (if needed), returns 0 if
// successfull
int mkdirp(char *dirname);

void waitForKey();

// checks if all a program is installed, returns 0 if found
int checkprogram(char *name);

// checks if all needed programs are installed, returns 0 if everything found
int checkprograms();

// returns 0 if successfull
int createusers();

// set access rights
void setMode(char *fname, int mode);

// set default permission mask
void setUmask(int mask);

// set user and group
void setOwner(char *path, char *userName, char *groupName);

// returns 0 if successfull
int createUserConfig(char *userName, char *groupName);

// returns 0 if successfull
int createSystemConfigPrecompile();

// returns 0 if successfull
int createSystemConfigAfterInstall();

// returns 0 if user has root privileges
int checkRootAccess();

// call a function as the specified user and group
int callAsUser(char *userName, char *groupName,
	       int func(char *userName, char *groupName) );

#endif

