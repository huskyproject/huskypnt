/* platform-specific functions, linux version */

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>

#include "common.h"
#include "os.h"
#include "lang.h"

char *defaults[numIdx+1] =
{
  "",
  "fido",
  "fido",
  "",
  "/usr/local/lib",
  "/usr/local/bin",
  "/usr/local/man",
  "/usr/local/etc/fido",
  "/var/log/fido",
  "/usr/local/include",
  "0",
  "",
  "",
  "",
  "/var/spool/fido/work",
  "/var/spool/fido/in",
  "/var/spool/fido/in/protect",
  "/var/spool/fido/in/local",
  "/var/spool/fido/in/arctmp",
  "/var/spool/fido/out/tmp",
  "/var/spool/fido/out/zone",
  "/var/spool/fido/echo",
  "/var/spool/fido/nodelist",
  "/var/spool/fido/netmail",
  "/usr/local/scripts",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "zip",
  "",
  "",
  "",
  "",
  "",
  "",
  ""
};

// returns 0 if file exists
int fexist(char *fname)
{
  int rc;
  struct stat sb;

  rc = stat(fname, &sb);

  if (rc != 0) return rc;

  if (S_ISREG(sb.st_mode)) return 0;

  return 1;
}

// returns 0 if directory exists
int direxist(char *dname)
{
  int rc;
  struct stat sb;

  rc = stat(dname, &sb);

  if (rc != 0) return rc;

  if (S_ISDIR(sb.st_mode)) return 0;

  return 1;
}

// returns 0 if successfull
int copyFile(char *sourceName, char *destName)
{
  FILE *source;
  FILE *dest;
  char *buf[1024];
  int br;

  source = fopen(sourceName, "r");
  if (source == NULL)
  {
    printf("Could not open '%s' for reading while copying to '%s'\n",
	   sourceName, destName);

    return 1;
  }

  dest = fopen(destName, "w");
  if (dest == NULL)
  {
    fclose(source);

    printf("Could not open '%s' for writing while copying '%s'\n", destName,
	   sourceName);

    return 2;
  }

  while (feof(source) == 0)
  {
    br = fread(buf, 1, 1024, source);
    fwrite(buf, 1, br, dest);
  }

  fclose(source);
  fclose(dest);

  return 0;
}

// returns 0 if successfull
int touchFile(char *fname)
{
  FILE *f;

  f = fopen(fname, "a");
  if (f == NULL)
  {
    printf("Could not touch '%s'!\n", fname);

    return 1;
  }
  fclose(f);

  return 0;
}

// returns 0 if successfull
int linkFile(char *source, char *dest)
{
  char *cmdline;
  int rc;

  cmdline = malloc(strlen(source)+strlen(dest)+11);
  sprintf(cmdline, "ln -s %s %s", source, dest);
  rc = system(cmdline);
  free(cmdline);

  return rc;
}

// create directory and parent directories (if needed), returns 0 if
// successfull
int mkdirp(char *dirname)
{
  char *cmdline;
  int rc;

  cmdline = malloc(strlen(dirname)+10);
  sprintf(cmdline, "mkdir -p %s", dirname);
  rc = system(cmdline);
  free(cmdline);

  return rc;
}

void waitForKey()
{
  int c;

  printf(pressReturnText);

  do
  {
    usleep(1000);
    c = fgetc(stdin);
  } while (c == EOF);
}

// checks if all a program is installed, returns 0 if found
int checkprogram(char *name)
{
  int rc;
  char *cmdline;

  cmdline = malloc(strlen(name) + 20);
  sprintf(cmdline, "which %s > /dev/null", name);
  rc = system(cmdline);
  free(cmdline);

  return rc;
}

// checks if all needed programs are installed, returns 0 if everything found
int checkprograms()
{
  int rc, ok = 0;
  int i;

  for (i = 0; i < numPrograms; i++)
  {
    rc = checkprogram(programs[i]);
    ok = ok + rc;

    if (rc != 0)
    {
      printf(notFoundText, programs[i]);
    }
  }

  if (ok != 0)
  {
    printf(pleaseInstallProgramsText);
  }

  return ok;
}

// display a file
void showFile(char *fname)
{
  FILE *f;
  char *content;
  struct stat sb;

  stat(fname, &sb);
  f = fopen(fname, "r");
  if (f == NULL)
  {
    printf("could not open file!\n");

    return;
  }

  content = (char *) malloc(sb.st_size+1);
  fread(content, sb.st_size, 1, f);
  content[sb.st_size] = 0;
  fclose(f);

  printf("%s", content);
  free(content);
}

// get the contents of a file
char *readFile(char *fname)
{
  FILE *f;
  char *content;
  struct stat sb;

  stat(fname, &sb);
  f = fopen(fname, "r");
  if (f == NULL) return strdup("could not open file!\n");

  content = (char *) malloc(sb.st_size+1);
  fread(content, sb.st_size, 1, f);
  content[sb.st_size] = 0;

  fclose(f);

  return content;
}

// create group <groupname> with users <users> in it
int creategroup(char *groupname, char *users)
{
  int id;
  FILE *f;
  struct group *group;

  group = getgrnam(groupname);
  if (group != NULL)
  {
    printf(groupExistsText, groupname, users);
    waitForKey();

    return group->gr_gid;
  }

  // find free GID
  id = 1000;
  group = getgrgid(id);
  while (group != NULL)
  {
    id++;
    group = getgrgid(id);
  }
  Cfg.groupId = id;

  if (fexist("/etc/gshadow") == 0)
  {
    f = fopen("/etc/group", "a");
    fprintf(f, "%s:x:%d:%s\n", groupname, id, users);
    fclose(f);

    f = fopen("/etc/gshadow", "a");
    fprintf(f, "%s:!::%s\n", groupname, users);
    fclose(f);
  }
  else
  {
    f = fopen("/etc/group", "a");
    fprintf(f, "%s::%d:%s\n", groupname, id, users);
    fclose(f);
  }

  printf(createdGroupText, groupname, id, users);

  return id;
}

// create user <username> with primary group <groupid>
int createuser(char *username, int groupid)
{
  char cmdline[256];
  int id;
  FILE *f;
  char homeDir[256];
  struct passwd *pw;
  int rc;

  pw = getpwnam(username);
  if (pw != NULL)
  {
    printf(userExistsText, username);
    waitForKey();
    Cfg.homeDir = strdup(pw->pw_dir);

    return pw->pw_uid;
  }
  
  // find free UID
  id = 1000;
  pw = getpwuid(id);
  while (pw != NULL)
  {
    id++;
    pw = getpwuid(id);
  }

  sprintf(homeDir, "/home/%s", username);
  Cfg.homeDir = ask(questions[homeDirIdx], homeDir);

  if (fexist("/etc/shadow") == 0)
  {
    f = fopen("/etc/passwd", "a");
    fprintf(f, "%s:x:%d:%d:Fido subsystem:%s:/bin/bash\n", username, id,
	    groupid, Cfg.homeDir);
    fclose(f);

    f = fopen("/etc/shadow", "a");
    fprintf(f, "%s::0:0:99999:7:::\n", username);
    fclose(f);
  }
  else
  {
    f = fopen("/etc/passwd", "a");
    fprintf(f, "%s::%d:%d:Fido subsystem:%s:/bin/bash\n", username, id,
	    groupid, Cfg.homeDir);
    fclose(f);
  }

  rc = mkdir(Cfg.homeDir, 0755);
  if ((rc != 0) && (errno != EEXIST))
  {
    printf("Could not create home directory!\n");
    return -1;
  }
  chown(Cfg.homeDir, id, groupid);

  printf(createdUserText, username, id, groupid);
  sprintf(cmdline, "passwd %s", username);
  system(cmdline);

  return id;
}

// returns 0 if successfull
int createusers()
{
  int groupId;

  printf(creatingUserAndGroupText);

  groupId = creategroup(Cfg.groupName, Cfg.users);
  if (createuser(Cfg.fidoName, groupId) == -1) return 1;

  return 0;
}

// set access rights
void setMode(char *fname, int mode)
{
  chmod(fname, mode);
}

// set default permission mask
void setUmask(int mask)
{
  umask(mask);
}

// set user and group
void setOwner(char *path, char *userName, char *groupName)
{
  struct passwd *pw;
  struct group *grp;

  pw = getpwnam(userName);
  grp = getgrnam(groupName);

  chown(path, pw->pw_uid, grp->gr_gid);

}

// returns 0 if successfull
int createUserConfig(char *userName, char *groupName)
{
  char fname[1024], fname2[1024];
  char *oldSysOpName, *oldUserName;
  struct passwd *pw;

  printf(creatingUserCfgText, userName);

  umask(0022);
  oldSysOpName = Cfg.sysOpName;
  oldUserName = Cfg.userName;

  pw = getpwnam(userName);

  if (strcmp(userName, Cfg.fidoName) != 0)
  {
    Cfg.sysOpName = strdup(pw->pw_gecos);
    if (strchr(Cfg.sysOpName, ',') != NULL) *strchr(Cfg.sysOpName, ',') = 0;
  }

  sprintf(fname, "%s/.msged", pw->pw_dir);
  sprintf(fname2, "%s/msged.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;

  sprintf(fname, "%s/.msged.scheme", pw->pw_dir);
  sprintf(fname2, "%s/msged.scheme.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;

  sprintf(fname, "%s/.msged.tpl", pw->pw_dir);
  sprintf(fname2, "%s/msged.tpl", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;

  if (strcmp(userName, Cfg.fidoName) != 0) free(Cfg.sysOpName);
  Cfg.sysOpName = oldSysOpName;
  Cfg.userName = oldUserName;

  printf(createdUserCfgText, userName);

  return 0;
}

// configure ld.so, returns 0 if successfull
int ldsocfg()
{
  FILE *f;
  char line[1024];
  int found = 1;

  printf(configLdsoText);

  f = fopen("/etc/ld.so.conf", "r+");
  if (f == NULL)
  {
    printf("Could not open /etc/ld.so.conf!\n");

    return 1;
  }

  // a bit dirty because it won't find libDir at a 1024-byte boundary,
  // but who has lines with over 1024 chars in /etc/ld.so.conf?
  while ((feof(f) == 0) && (found != 0))
  {
    fgets(line, 1024, f);
    while ((strlen(line) > 0) && (line[strlen(line) - 1] == '\n'))
      line[strlen(line) - 1] = 0;
    found = strcmp(line, Cfg.libDir);
  }

  if (found != 0)
  {
    sprintf(line, "%s\n", Cfg.libDir);
    fputs(line, f);
    printf(LdsoEntryMadeText);
  }

  fclose(f);

  printf(configLdsoDoneText);

  return 0;
}

// returns 0 if successfull
int profilecfg()
{
  FILE *f;
  char line[1024];
  int foundPath = 1, foundFidoconfig = 1, pathOk = 1;

  printf(configProfileText);

  f = fopen("/etc/profile", "r+");
  if (f == NULL)
  {
    printf("Could not open /etc/profile!\n");

    return 1;
  }

  // a bit dirty because it won't find path/fidoconfig at a 1024-byte boundary,
  // but who has lines with over 1024 chars in /etc/profile?
  while (feof(f) == 0)
  {
    fgets(line, 1024, f);
    while ((strlen(line) > 0) && (line[strlen(line) - 1] == '\n'))
      line[strlen(line) - 1] = 0;
    if (strstr(line, "PATH=") != NULL)
    {
      foundPath = 0;
      if ((strstr(line, Cfg.binDir) != NULL) &&
	  (strstr(line, Cfg.scriptDir) != NULL))
	pathOk = 0;
      else pathOk = 1;
    }
    else if (strstr(line, "FIDOCONFIG=") != NULL)
    {
      foundFidoconfig = 0;
    }
  }

  if (foundFidoconfig != 0)
  {
    fprintf(f, "export FIDOCONFIG=%s/config\n", Cfg.cfgDir);
  }

  if (foundPath != 0)
  {
    fprintf(f, "export PATH=%s:%s:%s\n", getenv("PATH"), Cfg.binDir,
	    Cfg.scriptDir);
  }
  else if (pathOk != 0)
  {
    fprintf(f, "PATH=${PATH}:%s:%s\n", Cfg.binDir, Cfg.scriptDir);
  }

  fclose(f);

  printf(configProfileDoneText);

  return 0;
}

int createSystemConfigPrecompile()
{
  return ldsocfg();
}

int createSystemConfigAfterInstall()
{
  return profilecfg();
}

// returns 0 if user has root privileges
int checkRootAccess()
{

  return geteuid();
}

int callAsUser(char *userName, char *groupName,
	       int func(char *userName, char *groupName) )
{
  int pid;
  int childstat;
  struct passwd *pw;
  struct group *grp;

  pw = getpwnam(userName);
  grp = getgrnam(groupName);

  pid = fork();
  if (pid == 0)
  {
    setuid(pw->pw_uid);
    setgid(grp->gr_gid);

    exit(func(userName, groupName));
  }
  do { usleep(100000); waitpid(pid, &childstat, 0); }
  while (WIFEXITED(childstat) == 0);

  return WEXITSTATUS(childstat);
}


