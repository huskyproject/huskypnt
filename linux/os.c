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

#include "auto.h"
#include "common.h"
#include "os.h"
#include "lang.h"

// numXXX are defined in common.h

char *zipFiles[numZipFiles] =
{
  "smapi.zip", "fidoconf.zip", "hpt.zip", "hptutil.zip", "sqpack.zip",
  "mpost.zip", "msged.zip", "ifcico.zip", "huskybse.zip", "huskmisc.zip",
  "nltools.zip", "scripts.zip"
};

char *programs[numPrograms] =
{
  "hpt", "hptutil", "sqpack", "mpost", "msged", "ifcico", "huskmisc",
  "nltools"
};

tCfgFileMap makeCfgFiles[numMakeCfgFiles] =
{
  { "huskymak.cfg", "src/huskymak.cfg", 0644 },
  { "ifcicomk.cfg", "src/ifcico/CONFIG", 0644 }
};

tCfgFileMap globalCfgFiles[numGlobalCfgFiles] =
{
  { "fconf.cfg", "config", 0644 },
  { "links.cfg", "links.cfg", 0600 },
  { NULL, "msgbase.cfg", 0644 },
  { "huskyui.cfg", "huskyui.cfg", 0644 },
  { "msgedg.cfg", "msged.cfg", 0644 },
  { "password.lst", "password.lst", 0600 },
  { "ifcico.cfg", "ifcico.cfg", 0644 }
};

char *neededPrograms[numNeededPrograms] =
{
  "unzip", "passwd"
};

char *defaults[numIdx] =
{
  "fido",
  "fido",
  "",
  "/usr/local/lib",
  "/usr/local/bin",
  "/usr/local/man",
  "/etc/fido",
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
  "/usr/local/info",
  "",
  "",
  "fidoadm"
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

// unpack archive into current directory
// returns 0 if successfull
int unpackFile(char *fname)
{
  char *cmdline;
  int rc;

  cmdline = malloc(strlen(fname)+10);
  sprintf(cmdline, "unzip -q %s", fname);
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

// checks if a program is installed, returns 0 if found
int checkNeededProgram(char *name)
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
int checkNeededPrograms()
{
  int rc, ok = 0;
  int i;

  for (i = 0; i < numNeededPrograms; i++)
  {
    rc = checkNeededProgram(neededPrograms[i]);
    ok = ok + rc;

    if (rc != 0)
    {
      printf(notFoundText, neededPrograms[i]);
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
    nfree(cfg[homeDirIdx]);
    cfg[homeDirIdx] = strdup(pw->pw_dir);

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
  nfree(cfg[homeDirIdx]);
  cfg[homeDirIdx] = ask(homeDirQuestion, homeDir);

  if (fexist("/etc/shadow") == 0)
  {
    f = fopen("/etc/passwd", "a");
    fprintf(f, "%s:x:%d:%d:Fido subsystem:%s:/bin/bash\n", username, id,
	    groupid, cfg[homeDirIdx]);
    fclose(f);

    f = fopen("/etc/shadow", "a");
    fprintf(f, "%s::0:0:99999:7:::\n", username);
    fclose(f);
  }
  else
  {
    f = fopen("/etc/passwd", "a");
    fprintf(f, "%s::%d:%d:Fido subsystem:%s:/bin/bash\n", username, id,
	    groupid, cfg[homeDirIdx]);
    fclose(f);
  }

  rc = mkdir(cfg[homeDirIdx], 0755);
  if ((rc != 0) && (errno != EEXIST))
  {
    printf("Could not create home directory!\n");
    return -1;
  }
  chown(cfg[homeDirIdx], id, groupid);

  printf(createdUserText, username, id, groupid);
  sprintf(cmdline, "passwd %s", username);
  system(cmdline);

  return id;
}

// returns 0 if successfull
int createusers()
{
  int fidoGroupId, admGroupId;

  printf(creatingUserAndGroupText);

  fidoGroupId = creategroup(cfg[groupNameIdx], cfg[usersIdx]);
  if (createuser(cfg[fidoNameIdx], fidoGroupId) == -1) return 1;
  admGroupId = creategroup(cfg[admGroupNameIdx], cfg[fidoNameIdx]);

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
  struct group *gr;

  printf(creatingUserCfgText, userName);

  oldSysOpName = cfg[sysOpNameIdx];
  oldUserName = cfg[userNameIdx];

  pw = getpwnam(userName);
  gr = getgrgid(pw->pw_gid);

  if (strcmp(userName, cfg[fidoNameIdx]) != 0)
  {
    cfg[sysOpNameIdx] = strdup(pw->pw_gecos);
    if (strchr(cfg[sysOpNameIdx], ',') != NULL)
      *strchr(cfg[sysOpNameIdx], ',') = 0;
  }

  sprintf(fname, "%s/.fido", pw->pw_dir);
  mkdirp(fname);
  setOwner(fname, userName, cfg[admGroupNameIdx]);
  setMode(fname, 488); // Octal 750

  sprintf(fname, "%s/.fido/msged.cfg", pw->pw_dir);
  sprintf(fname2, "%s/msged.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setOwner(fname, userName, gr->gr_name);
  setMode(fname, 384); // Octal 600

  sprintf(fname, "%s/.fido/msged.colors.cfg", pw->pw_dir);
  sprintf(fname2, "%s/msged.colors.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setOwner(fname, userName, gr->gr_name);
  setMode(fname, 384); // Octal 600

  sprintf(fname, "%s/.fido/msged.tpl", pw->pw_dir);
  sprintf(fname2, "%s/msged.tpl", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setOwner(fname, userName, gr->gr_name);
  setMode(fname, 384); // Octal 600

  if (strcmp(userName, cfg[fidoNameIdx]) != 0) free(cfg[sysOpNameIdx]);
  cfg[sysOpNameIdx] = oldSysOpName;
  cfg[userNameIdx] = oldUserName;

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
    found = strcmp(line, cfg[libDirIdx]);
  }

  if (found != 0)
  {
    sprintf(line, "%s\n", cfg[libDirIdx]);
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
  int foundBin = 1, foundScripts = 1, foundFidoconfig = 1, foundPath = 1;

  printf(configProfileText);

  f = fopen("/etc/profile", "r+");
  if (f == NULL)
  {
    printf("Could not open /etc/profile!\n");

    return 1;
  }

  while (feof(f) == 0)
  {
    fgets(line, 1024, f);
    while ((strlen(line) > 0) && (line[strlen(line) - 1] == '\n'))
      line[strlen(line) - 1] = 0;
    if (strstr(line, "PATH=") != NULL)
    {
      foundPath = 0;
      if (strstr(line, cfg[binDirIdx]) != NULL) foundBin = 0;
      if (strstr(line, cfg[scriptDirIdx]) != NULL) foundScripts = 0;
    }
    else if (strstr(line, "FIDOCONFIG=") != NULL)
    {
      foundFidoconfig = 0;
    }
  }

  if (foundFidoconfig != 0)
  {
    fprintf(f, "export FIDOCONFIG=%s/config\n", cfg[cfgDirIdx]);
  }

  if (foundPath != 0)
  {
    fprintf(f, "export PATH=%s:%s:%s\n", getenv("PATH"), cfg[binDirIdx],
	    cfg[scriptDirIdx]);
  }
  else if ((foundBin != 0) && (foundScripts != 0))
  {
    fprintf(f, "PATH=${PATH}:%s:%s\n", cfg[binDirIdx], cfg[scriptDirIdx]);
  }
  else if ((foundBin != 0) && (foundScripts == 0))
  {
    fprintf(f, "PATH=${PATH}:%s\n", cfg[binDirIdx]);
  }
  else if ((foundBin == 0) && (foundScripts != 0))
  {
    fprintf(f, "PATH=${PATH}:%s\n", cfg[scriptDirIdx]);
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
    setgid(grp->gr_gid);
    setuid(pw->pw_uid);

    exit(func(userName, groupName));
  }

  pid = waitpid(pid, &childstat, 0);
  while ((WIFEXITED(childstat) == 0) && (WIFSIGNALED(childstat) == 0) &&
	 (pid != -1))
  {
    pid = waitpid(pid, &childstat, 0);
  }

  if (WIFEXITED(childstat) != 0) return WEXITSTATUS(childstat);
  if (WIFSIGNALED(childstat) != 0) return WTERMSIG(childstat);

  // child exited faster than waitpid was issued, assume it was all right
  return 0;
}

// returns 0 on success
int setVar(char *varName, char *content)
{
  return setenv(varName, content, 1);
}

char *getVar(char *varName)
{
  char *content;

  content = getenv(varName);
  if (content != NULL) return content;

  return "";
}

// returns 0 if successfull
int compileNodelists(char *userName, char *groupName)
{
  char *cmdline;
  int rc;

  printf(compilingNodelistsText);

  printf("nlupdate...\n");
  cmdline = malloc(strlen(cfg[binDirIdx])+10);
  sprintf(cmdline, "%s" dirSepS "nlupdate", cfg[binDirIdx]);
  rc = system(cmdline);
  free(cmdline);
  printf("nlupdate finished\n");

  printf("ifindex...\n");
  cmdline = malloc(strlen(cfg[binDirIdx])+9);
  sprintf(cmdline, "%s" dirSepS "ifindex", cfg[binDirIdx]);
  rc = system(cmdline);
  free(cmdline);
  printf("ifindex finished\n");

  printf("ulc...\n");
  cmdline = malloc(strlen(cfg[binDirIdx])+5);
  sprintf(cmdline, "%s" dirSepS "ulc", cfg[binDirIdx]);
  rc = system(cmdline);
  free(cmdline);
  printf("ulc finished\n");

  if (rc == 0) printf(compiledNodelistsText);

  return rc;
}

void clrscr()
{
  // ANSI sequence - should work on every terminal
  printf("\033[H\033[J");
}



// init os-dependent variables, do os-dep. checks
int osInit()
{
  int rc;

  rc = checkNeededPrograms();
  if (rc != 0) return rcCheckNeededPrograms;

  rc = setVar("linux", "1");
  rc += setVar("os", OS);
  rc += setVar("cp", "cp");
  rc += setVar("mv", "mv");
  rc += setVar("ln", "ln");
  rc += setVar("lnopt", "-sf");
  rc += setVar("rm", "rm");
  rc += setVar("cc", "gcc");
  rc += setVar("gpp", "g++");
  rc += setVar("ar", "ar");
  rc += setVar("ar_r", "r");
  rc += setVar("install", "install");
  rc += setVar("mkdir", "mkdir");
  rc += setVar("mkdiropt", "-p");
  rc += setVar("ldconfig", "/sbin/ldconfig");
  rc += setVar("ranlib", "ar s");
  rc += setVar("h2pas", "h2pas");
  rc += setVar("ostype", "UNIX");
  rc += setVar("uname", "LNX");
  rc += setVar("dynlibs", "1");
  rc += setVar("addcdefs", "-DHAS_POPEN");
  rc += setVar("exe", "");
  rc += setVar("obj", ".o");
  rc += setVar("lib", ".a");
  rc += setVar("tpu", ".ppu");

  return rc;
}

// free os-dependent variables
void osDone()
{
}

