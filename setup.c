/* setup-script for HuskyPoint/Lnx
   author: Sascha Silbe
   last change: 12.04.00
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>

#define VERSION "1.3"

#define numZipFiles 9
char *zipFiles[numZipFiles] =
{
  "smapi.zip", "fidoconf.zip", "hpt.zip", "sqpack.zip", "mpost.zip",
  "msged.zip", "ifcico.zip", "huskycom.zip", "scripts.zip"
};

#define numPrograms 2
char *programs[numPrograms] =
{
  "unzip", "passwd"
};

#define numNodelists 2
char *nodelists[numNodelists] =
{
  "nodelist", "points24"
};

#define groupNameIdx                        1
#define fidoNameIdx                         2
#define usersIdx                            3
#define libDirIdx                           4
#define binDirIdx                           5
#define manDirIdx                           6
#define cfgDirIdx                           7
#define logDirIdx                           8
#define incDirIdx                           9
#define debugIdx                           10
#define libcversionIdx                     11
#define locationIdx                        12
#define sysOpNameIdx                       13
#define workDirIdx                         14
#define inboundIdx                         15
#define protInboundIdx                     16
#define localInboundIdx                    17
#define tempInboundIdx                     18
#define tempOutboundIdx                    19
#define outboundIdx                        20
#define msgbaseDirIdx                      21
#define nodelistDirIdx                     22
#define netmailDirIdx                      23
#define scriptDirIdx                       24
#define isdnDevIdx                         25
#define modemDevIdx                        26
#define modemBaudIdx                       27
#define internatPrefixIdx                  28
#define localPrefixIdx                     29
#define voiceNumIdx                        30
#define dataNumIdx                         31
#define amtNumIdx                          32
#define localNumIdx                        33
#define internatNumIdx                     34
#define packerIdx                          35
#define pointNrIdx                         36
#define uplinkAddrIdx                      37
#define uplinkNameIdx                      38
#define uplinkPwdIdx                       39
#define homeDirIdx                         40
#define infoDirIdx                         41
#define htmlDirIdx                         42
#define numIdx                             42

#include "lang.h"
#include "os.h"


typedef struct _tCfg
{
  char *groupName, *fidoName, *users, *userName;
  char *libDir, *binDir, *manDir, *cfgDir, *logDir, *incDir, *infoDir;
  char *htmlDir;
  char *debug;
  char *libcversion;
  char *location, *sysOpName;
  char *workDir, *inbound, *protInbound, *localInbound, *tempInbound;
  char *tempOutbound, *outbound, *msgbaseDir, *nodelistDir, *netmailDir;
  char *scriptDir;
  char *isdnDev, *modemDev;
  char *modemBaud;
  char *internatPrefix, *localPrefix, *voiceNum, *dataNum, *amtNum, *localNum;
  char *internatNum;
  char *packer;

  char *pointNr, *uplinkAddr, *uplinkName, *uplinkPwd;

  int groupId;
  int userId;
  char *homeDir;
} tCfg;

tCfg Cfg;

#define nfree(a) { if (a != NULL) { free(a); a = NULL; } }

char *ask(char *prompt, char *defaultValue)
{
  char inputBuf[1024];

  printf(prompt, defaultValue);
  fgets(inputBuf, 1024, stdin);

  while ((strlen(inputBuf) > 0) && (inputBuf[strlen(inputBuf) - 1] == '\n'))
    inputBuf[strlen(inputBuf) - 1] = 0;

  if (strlen(inputBuf) == 0) return strdup(defaultValue);

  return strdup(inputBuf);
}

// 0 if everything's allright
int askAllright()
{
  int c = ' ';

  while ((c != yesKey) && (c != noKey))
  {
    printf(allrightText, yesKey, noKey);
    c = fgetc(stdin);
    printf("\n");
  }

  // get return
  fgetc(stdin);

  if (c == yesKey) return 0;
  return 1;
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

char *findVar(char *varName)
{
  if (strcmp(varName, "groupname") == 0) return Cfg.groupName;
  else if (strcmp(varName, "fidoname") == 0) return Cfg.fidoName;
  else if (strcmp(varName, "username") == 0) return Cfg.userName;
  else if (strcmp(varName, "users") == 0) return Cfg.users;
  else if (strcmp(varName, "libdir") == 0) return Cfg.libDir;
  else if (strcmp(varName, "bindir") == 0) return Cfg.binDir;
  else if (strcmp(varName, "mandir") == 0) return Cfg.manDir;
  else if (strcmp(varName, "cfgdir") == 0) return Cfg.cfgDir;
  else if (strcmp(varName, "logdir") == 0) return Cfg.logDir;
  else if (strcmp(varName, "incdir") == 0) return Cfg.incDir;
  else if (strcmp(varName, "debug") == 0) return Cfg.debug;
  else if (strcmp(varName, "libcversion") == 0) return Cfg.libcversion;
  else if (strcmp(varName, "location") == 0) return Cfg.location;
  else if (strcmp(varName, "sysopname") == 0) return Cfg.sysOpName;
  else if (strcmp(varName, "workdir") == 0) return Cfg.workDir;
  else if (strcmp(varName, "inbound") == 0) return Cfg.inbound;
  else if (strcmp(varName, "protinbound") == 0) return Cfg.protInbound;
  else if (strcmp(varName, "localinbound") == 0) return Cfg.localInbound;
  else if (strcmp(varName, "tempinbound") == 0) return Cfg.tempInbound;
  else if (strcmp(varName, "outbound") == 0) return Cfg.outbound;
  else if (strcmp(varName, "tempoutbound") == 0) return Cfg.tempOutbound;
  else if (strcmp(varName, "msgbasedir") == 0) return Cfg.msgbaseDir;
  else if (strcmp(varName, "nodelistdir") == 0) return Cfg.nodelistDir;
  else if (strcmp(varName, "netmaildir") == 0) return Cfg.netmailDir;
  else if (strcmp(varName, "scriptdir") == 0) return Cfg.scriptDir;
  else if (strcmp(varName, "isdndev") == 0) return Cfg.isdnDev;
  else if (strcmp(varName, "modemdev") == 0) return Cfg.modemDev;
  else if (strcmp(varName, "modembaud") == 0) return Cfg.modemBaud;
  else if (strcmp(varName, "internatprefix") == 0) return Cfg.internatPrefix;
  else if (strcmp(varName, "localprefix") == 0) return Cfg.localPrefix;
  else if (strcmp(varName, "voicenum") == 0) return Cfg.voiceNum;
  else if (strcmp(varName, "datanum") == 0) return Cfg.dataNum;
  else if (strcmp(varName, "amtnum") == 0) return Cfg.amtNum;
  else if (strcmp(varName, "localnum") == 0) return Cfg.localNum;
  else if (strcmp(varName, "internatnum") == 0) return Cfg.internatNum;
  else if (strcmp(varName, "packer") == 0) return Cfg.packer;
  else if (strcmp(varName, "pointnr") == 0) return Cfg.pointNr;
  else if (strcmp(varName, "uplinkaddr") == 0) return Cfg.uplinkAddr;
  else if (strcmp(varName, "uplinkname") == 0) return Cfg.uplinkName;
  else if (strcmp(varName, "uplinkpwd") == 0) return Cfg.uplinkPwd;
  else if (strcmp(varName, "homedir") == 0) return Cfg.homeDir;
  else if (strcmp(varName, "infodir") == 0) return Cfg.infoDir;
  else if (strcmp(varName, "htmldir") == 0) return Cfg.htmlDir;

  return "";
}

// returns 0 if successfull
int processTemplate(char *templName, char *outName)
{
  FILE *templ;
  FILE *out;
  char lineIn[1024], lineOut[1024];
  char *lineInTmp, *lineOutTmp;

  templ = fopen(templName, "r");

  if (templ == NULL)
  {
    printf("Could not open template-file '%s'!\n", templName);

    return 1;
  }

  out = fopen(outName, "w");

  if (out == NULL)
  {
    printf("Cannot write '%s'!\n", outName);
    fclose(templ);

    return 2;
  }

  while (feof(templ) == 0)
  {
    *lineIn = 0;
    lineInTmp = lineIn;
    lineOutTmp = lineOut;
    fgets(lineIn, 1024, templ);

    while (*lineInTmp != 0)
    {
      if (*lineInTmp == '\\')
      {
	lineInTmp++;
	*lineOutTmp = *lineInTmp;
	lineInTmp++;
	lineOutTmp++;
      }
      else if (*lineInTmp == '$')
      {
	int pos;
	char *varName;
	char *varContent;
	char *varContTmp;
	int i;

	lineInTmp++;
	if (strchr(lineInTmp, '$') != NULL)
	  pos = strchr(lineInTmp, '$') - lineInTmp;
	else pos = 0;
	varName = malloc(pos+1);
	for (i = 0; i < pos; i++) varName[i] = tolower(lineInTmp[i]);
	varName[pos] = 0;
	varContent = findVar(varName);
	free(varName);

	for (varContTmp = varContent ; *varContTmp != 0; varContTmp++)
	{
	  *lineOutTmp = *varContTmp;
          lineOutTmp++;
	}

	lineInTmp = lineInTmp + pos + 1;
      }
      else
      {
	*lineOutTmp = *lineInTmp;
	lineInTmp++;
	lineOutTmp++;
      }
    }

    *lineOutTmp = 0;

    fputs(lineOut, out);
  }

  fclose(templ);
  fclose(out);

  return 0;
}

// checks if all needed programs are installed, returns 0 if everything found
int checkprograms()
{
  int rc, ok = 0;
  int i;
  char cmdline[256];

  for (i = 0; i < numPrograms; i++)
  {
    sprintf(cmdline, "which %s > /dev/null", programs[i]);
    rc = system(cmdline);
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

// checks if all needed files are available, returns 0 if everything found
int checkfiles()
{
  int rc, ok = 0;
  int i;
  DIR *dir;
  struct dirent *dirent;

  for (i = 0; i < numZipFiles; i++)
  {
    rc = fexist(zipFiles[i]);

    if (rc != 0)
    {
      printf(notFoundText, zipFiles[i]);
      ok++;
    }
  }

  for (i = 0; i < numNodelists; i++)
  {
    dir = opendir(".");

    do
    {
      dirent = readdir(dir);
    } while ((dirent != NULL) &&
	     (strstr(dirent->d_name, nodelists[i]) != dirent->d_name));

    if (dirent == NULL)
    {
      char *nlname;

      nlname = malloc(strlen(nodelists[i]) + 3);
      sprintf(nlname, "%s.*", nodelists[i]);
      printf(notFoundText, nlname);
      free(nlname);
      ok++;
    }

    closedir(dir);
  }
  
  if (ok != 0)
  {
    printf(pleaseCopyFilesText);
  }

  return ok;
}

// get config (without uplink)
void getconfig()
{
  int ok;

  ok = 1;
  while (ok != 0)
  {
    Cfg.groupName = ask(questions[groupNameIdx], defaults[groupNameIdx]);
    Cfg.fidoName = ask(questions[fidoNameIdx], defaults[fidoNameIdx]);
    Cfg.userName = Cfg.fidoName;
//    Cfg.users = ask(questions[usersIdx], defaults[usersIdx]);
    Cfg.users = strdup("");
    Cfg.libcversion = ask(questions[libcversionIdx], defaults[libcversionIdx]);
    Cfg.debug = ask(questions[debugIdx], defaults[debugIdx]);
    Cfg.libDir = ask(questions[libDirIdx], defaults[libDirIdx]);
    Cfg.binDir = ask(questions[binDirIdx], defaults[binDirIdx]);
    Cfg.manDir = ask(questions[manDirIdx], defaults[manDirIdx]);
    Cfg.cfgDir = ask(questions[cfgDirIdx], defaults[cfgDirIdx]);
    Cfg.logDir = ask(questions[logDirIdx], defaults[logDirIdx]);
    Cfg.incDir = ask(questions[incDirIdx], defaults[incDirIdx]);
//    Cfg.infoDir = ask(questions[infoDirIdx], defaults[infoDirIdx]);
    Cfg.infoDir = strdup("");
//    Cfg.htmlDir = ask(questions[htmlDirIdx], defaults[htmlDirIdx]);
    Cfg.htmlDir = strdup("");

    ok = askAllright();
  }
  printf("\n");

  ok = 1;
  while (ok != 0)
  {
    Cfg.location = ask(questions[locationIdx], defaults[locationIdx]);
    Cfg.sysOpName = ask(questions[sysOpNameIdx], defaults[sysOpNameIdx]);
    Cfg.packer = ask(questions[packerIdx], defaults[packerIdx]);
    Cfg.workDir = ask(questions[workDirIdx], defaults[workDirIdx]);
    Cfg.inbound = ask(questions[inboundIdx], defaults[inboundIdx]);
    Cfg.protInbound = ask(questions[protInboundIdx], defaults[protInboundIdx]);
    Cfg.localInbound = ask(questions[localInboundIdx], defaults[localInboundIdx]);
    Cfg.tempInbound = ask(questions[tempInboundIdx], defaults[tempInboundIdx]);
    Cfg.tempOutbound = ask(questions[tempOutboundIdx], defaults[tempOutboundIdx]);
    Cfg.outbound = ask(questions[outboundIdx], defaults[outboundIdx]);
    Cfg.msgbaseDir = ask(questions[msgbaseDirIdx], defaults[msgbaseDirIdx]);
    Cfg.nodelistDir = ask(questions[nodelistDirIdx], defaults[nodelistDirIdx]);
    Cfg.netmailDir = ask(questions[netmailDirIdx], defaults[netmailDirIdx]);
    Cfg.scriptDir = ask(questions[scriptDirIdx], defaults[scriptDirIdx]);

    ok = askAllright();
  }
  printf("\n");

  ok = 1;
  while (ok != 0)
  {
    Cfg.isdnDev = ask(questions[isdnDevIdx], defaults[isdnDevIdx]);
    Cfg.modemDev = ask(questions[modemDevIdx], defaults[modemDevIdx]);
    Cfg.modemBaud = ask(questions[modemBaudIdx], defaults[modemBaudIdx]);
    Cfg.internatPrefix = ask(questions[internatPrefixIdx], defaults[internatPrefixIdx]);
    Cfg.localPrefix = ask(questions[localPrefixIdx], defaults[localPrefixIdx]);
    Cfg.voiceNum = ask(questions[voiceNumIdx], defaults[voiceNumIdx]);
    Cfg.dataNum = ask(questions[dataNumIdx], defaults[dataNumIdx]);
    Cfg.amtNum = ask(questions[amtNumIdx], defaults[amtNumIdx]);
    Cfg.localNum = ask(questions[localNumIdx], defaults[localNumIdx]);
    Cfg.internatNum = ask(questions[internatNumIdx], defaults[internatNumIdx]);

    ok = askAllright();
  }
  printf("\n");
}

void chooseboss()
{
  FILE *f;
  char *nodes2435;
  struct stat sb;

  // read list of nodes
  stat("2435node.lst", &sb);
  f = fopen("2435node.lst", "r");
  nodes2435 = (char *) malloc(sb.st_size+1);
  fread(nodes2435, sb.st_size, 1, f);
  nodes2435[sb.st_size] = 0;
  fclose(f);

  printf(chooseBossText, nodes2435);
  free(nodes2435);

  waitForKey();
}

// get config from uplink
void getuplinkconfig()
{
  int ok = 1;

  while (ok != 0)
  {
    Cfg.pointNr = ask(questions[pointNrIdx], defaults[pointNrIdx]);
    Cfg.uplinkAddr = ask(questions[uplinkAddrIdx], defaults[uplinkAddrIdx]);
    Cfg.uplinkName = ask(questions[uplinkNameIdx], defaults[uplinkNameIdx]);
    Cfg.uplinkPwd = ask(questions[uplinkPwdIdx], defaults[uplinkPwdIdx]);

    ok = askAllright();
  }
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
  printf(creatingUserAndGroupText);

  Cfg.groupId = creategroup(Cfg.groupName, Cfg.users);
  Cfg.userId = createuser(Cfg.fidoName, Cfg.groupId);
  if (Cfg.userId == -1) return 1;

  return 0;
}

// returns 0 if successfull
int createdirs()
{
  char dirname[1024];
  int pos;

  printf(creatingDirsText);

  if (direxist(Cfg.libDir) != 0)
    if (mkdirp(Cfg.libDir) != 0) return 1;
  if (direxist(Cfg.binDir) != 0)
    if (mkdirp(Cfg.binDir) != 0) return 1;
  if (direxist(Cfg.manDir) != 0)
    if (mkdirp(Cfg.manDir) != 0) return 1;
  sprintf(dirname, "%s/man1", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/man2", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/man3", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/man4", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/man5", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/man6", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/man7", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/man8", Cfg.manDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(Cfg.incDir) != 0)
    if (mkdirp(Cfg.incDir) != 0) return 1;
  if (direxist(Cfg.cfgDir) != 0)
    if (mkdirp(Cfg.cfgDir) != 0) return 1;
  if (direxist(Cfg.logDir) != 0)
    if (mkdirp(Cfg.logDir) != 0) return 1;
  if (direxist(Cfg.scriptDir) != 0)
    if (mkdirp(Cfg.scriptDir) != 0) return 1;
  if (direxist(Cfg.workDir) != 0)
    if (mkdirp(Cfg.workDir) != 0) return 1;
  sprintf(dirname, "%s/dupes", Cfg.workDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s/src", Cfg.homeDir);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(Cfg.inbound) != 0)
    if (mkdirp(Cfg.inbound) != 0) return 1;
  sprintf(dirname, "%s/tmp", Cfg.inbound);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(Cfg.localInbound) != 0)
    if (mkdirp(Cfg.localInbound) != 0) return 1;
  if (direxist(Cfg.tempInbound) != 0)
    if (mkdirp(Cfg.tempInbound) != 0) return 1;
  if (direxist(Cfg.protInbound) != 0)
    if (mkdirp(Cfg.protInbound) != 0) return 1;
  sprintf(dirname, "%s/tmp", Cfg.protInbound);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(Cfg.outbound) != 0)
    if (mkdirp(Cfg.outbound) != 0) return 1;
  if (direxist(Cfg.tempOutbound) != 0)
    if (mkdirp(Cfg.tempOutbound) != 0) return 1;
  if (direxist(Cfg.msgbaseDir) != 0)
    if (mkdirp(Cfg.msgbaseDir) != 0) return 1;
  if (direxist(Cfg.nodelistDir) != 0)
    if (mkdirp(Cfg.nodelistDir) != 0) return 1;
  if (direxist(Cfg.netmailDir) != 0)
    if (mkdirp(Cfg.netmailDir) != 0) return 1;

  printf(settingRightsText);

  chmod(Cfg.libDir, 493); // dec. 493 = octal 0755
  chmod(Cfg.binDir, 493);
  chmod(Cfg.manDir, 493);
  sprintf(dirname, "%s/man1", Cfg.manDir); chmod(dirname, 493);
  sprintf(dirname, "%s/man2", Cfg.manDir); chmod(dirname, 493);
  sprintf(dirname, "%s/man3", Cfg.manDir); chmod(dirname, 493);
  sprintf(dirname, "%s/man4", Cfg.manDir); chmod(dirname, 493);
  sprintf(dirname, "%s/man5", Cfg.manDir); chmod(dirname, 493);
  sprintf(dirname, "%s/man6", Cfg.manDir); chmod(dirname, 493);
  sprintf(dirname, "%s/man7", Cfg.manDir); chmod(dirname, 493);
  sprintf(dirname, "%s/man8", Cfg.manDir); chmod(dirname, 493);
  chmod(Cfg.incDir, 493);
  chmod(Cfg.cfgDir, 493);
  chmod(Cfg.logDir, 448); // dec. 448 = octal 0700
  chmod(Cfg.scriptDir, 493);
  chmod(Cfg.workDir, 493);
  chmod(Cfg.inbound, 448);
  chmod(Cfg.localInbound, 448);
  chmod(Cfg.tempInbound, 448);
  chmod(Cfg.protInbound, 448);
  chmod(Cfg.outbound, 448);
  chmod(Cfg.tempOutbound, 448);
  chmod(Cfg.msgbaseDir, 493);
  chmod(Cfg.netmailDir, 448);
  chmod(Cfg.nodelistDir, 493);
  sprintf(dirname, "%s/dupes", Cfg.workDir); chmod(dirname, 493);
  sprintf(dirname, "%s/src", Cfg.homeDir); chmod(dirname, 493);
  sprintf(dirname, "%s/tmp", Cfg.inbound); chmod(dirname, 448);
  sprintf(dirname, "%s/tmp", Cfg.protInbound); chmod(dirname, 448);

  // parent directory of outbound (.../out/zone -> .../out)
  pos = strrchr(Cfg.outbound, '/') - Cfg.outbound;
  strncpy(dirname, Cfg.outbound, pos); dirname[pos] = 0;
  chmod(dirname, 448);

  // dec. 483 = octal 0600
  if (strlen(Cfg.isdnDev) > 0) chmod(Cfg.isdnDev, 483); 
  if (strlen(Cfg.modemDev) > 0) chmod(Cfg.isdnDev, 483);

  chown(Cfg.cfgDir, Cfg.userId, Cfg.groupId);
  chown(Cfg.logDir, Cfg.userId, Cfg.groupId);
  chown(Cfg.scriptDir, Cfg.userId, Cfg.groupId);
  chown(Cfg.workDir, Cfg.userId, Cfg.groupId);
  chown(Cfg.inbound, Cfg.userId, Cfg.groupId);
  chown(Cfg.localInbound, Cfg.userId, Cfg.groupId);
  chown(Cfg.protInbound, Cfg.userId, Cfg.groupId);
  chown(Cfg.tempInbound, Cfg.userId, Cfg.groupId);
  chown(Cfg.outbound, Cfg.userId, Cfg.groupId);
  chown(Cfg.tempOutbound, Cfg.userId, Cfg.groupId);
  chown(Cfg.msgbaseDir, Cfg.userId, Cfg.groupId);
  chown(Cfg.nodelistDir, Cfg.userId, Cfg.groupId);
  chown(Cfg.netmailDir, Cfg.userId, Cfg.groupId);
  sprintf(dirname, "%s/dupes", Cfg.workDir);
  chown(dirname, Cfg.userId, Cfg.groupId);
  sprintf(dirname, "%s/src", Cfg.homeDir);
  chown(dirname, Cfg.userId, Cfg.groupId);
  sprintf(dirname, "%s/tmp", Cfg.inbound);
  chown(dirname, Cfg.userId, Cfg.groupId);
  sprintf(dirname, "%s/tmp", Cfg.protInbound);
  chown(dirname, Cfg.userId, Cfg.groupId);

  // parent directory of outbound (.../out/zone -> .../out)
  pos = strrchr(Cfg.outbound, '/') - Cfg.outbound;
  strncpy(dirname, Cfg.outbound, pos); dirname[pos] = 0;
  chown(dirname, Cfg.userId, Cfg.groupId);

  if (Cfg.isdnDev != NULL) chown(Cfg.isdnDev, Cfg.userId, Cfg.groupId);
  if (Cfg.modemDev != NULL) chown(Cfg.modemDev, Cfg.userId, Cfg.groupId);

  printf(createdDirsText);

  return 0;
}

// returns 0 if successfull
int unpacksources()
{
  char dirname[1024];
  char cwd[1024];
  char cmdline[1024];
  int i;
  int rc;

  printf(unzipSourcesText);

  getcwd(cwd, 1024);
  sprintf(dirname, "%s/src", Cfg.homeDir);
  rc = chdir(dirname);

  if (rc != 0)
  {
    printf(chdirErrorText, dirname);

    return 1;
  }

  for (i = 0; i < numZipFiles; i++)
  {
    sprintf(cmdline, "unzip -q %s/%s", cwd, zipFiles[i]);
    rc = system(cmdline);
  }

  chdir(cwd);

  return 0;
}

// returns 0 is successfull
int createmakeconfig()
{
  char *fname;
  FILE *f;

  printf(creatingMakefileCfgsText);

  fname = malloc(strlen(Cfg.homeDir)+18);
  sprintf(fname, "%s/src/huskymak.cfg", Cfg.homeDir);
  if (processTemplate("huskymak.cfg", fname) != 0)
  {
    free(fname);

    return 1;
  }
  free(fname);

  fname = malloc(strlen(Cfg.homeDir)+19);
  sprintf(fname, "%s/src/ifcico/CONFIG", Cfg.homeDir);
  if (processTemplate("ifcicomk.cfg", fname) != 0)
  {
    free(fname);

    return 1;
  }

  f = fopen(fname, "a");
  if (f == NULL)
  {
    printf("Could not open '%s' for appending!\n", fname);
    free(fname);

    return 1;
  }

  if (strcmp(Cfg.libcversion, "libc5") == 0)
  {
    fprintf(f, "SOPTS        = -DHAS_STATFS -DSTATFS_IN_VFS_H -DHAS_SETSID -DHAS_NDBM_H \\\n"
	    "\t\t-DDONT_HAVE_TM_GMTOFF -DHAS_TERMIOS_H -DASCII_LOCKFILES \\\n"
	    "\t\t-DHAS_FSYNC -DHAS_IOCTL_H -DHAS_REGEX_H -DHAS_TCP \\\n"
	    "\t\t-DNEED_UUCPFROM -DNEED_TRAP -DSETPROCTITLE \\\n"
	    "\t\t-DHAS_SELECT -DREGEX_NEED_CARET -DPRE_21_KERNEL\n"
	    "OPTS\t= ${SOPTS} -DFORCEINTL -DNEED_BSY -DSLAVE_SENDS_NAK_TOO \\\n"
	    "-DBELIEVE_ZFIN=1\n");
  }
  else
  {
    fprintf(f, "SOPTS        = -DHAS_STATFS -DSTATFS_IN_VFS_H -DHAS_SETSID -DHAS_NDBM_H \\\n"
	    "\t\t-DDONT_HAVE_TM_GMTOFF -DHAS_TERMIOS_H -DASCII_LOCKFILES \\\n"
	    "\t\t-DHAS_FSYNC -DHAS_IOCTL_H -DHAS_REGEX_H -DHAS_TCP \\\n"
	    "\t\t-DNEED_UUCPFROM -DNEED_TRAP -DSETPROCTITLE \\\n"
	    "\t\t-DHAS_SELECT -DREGEX_NEED_CARET -DBSD_SIGNALS \\\n"
	    "\t\t-DPRE_21_LINUX -DSETPROCTITLE\n"
	    "OPTS\t= ${SOPTS} -DFORCEINTL -DNEED_BSY -DSLAVE_SENDS_NAK_TOO \\\n"
	    "-DBELIEVE_ZFIN=1\n"
	    "LIBRESOLV = -lresolv\t\t# this is needed for glibc 2\n");
  }
  if (strcmp(Cfg.debug, "1") == 0)
  {
    fprintf(f, "CFLAGS += -ggdb\n");
  }

  fclose(f);
  free(fname);

  printf(createdMakefileCfgsText);

  return 0;
}

// returns 0 if successfull
int doCompile(char *name, char *makeCommand)
{
  char cwd[1024];
  char *dirname;
  int rc;

  getcwd(cwd, 1024);

  dirname = malloc(strlen(Cfg.homeDir)+strlen(name)+6);
  sprintf(dirname, "%s/src/%s", Cfg.homeDir, name);
  if (chdir(dirname) != 0)
  {
    printf(chdirErrorText, dirname);
    free(dirname);

    return 1;
  }

  printf(compilingText, name);

  rc = system(makeCommand);

  chdir(cwd);

  if (rc == 0) printf(compiledText, name);

  return rc;
}

// returns 0 if successfull
int doInstall(char *name, char *makeCommand)
{
  char cwd[1024];
  char *dirname;
  int rc;

  getcwd(cwd, 1024);

  dirname = malloc(strlen(Cfg.homeDir)+strlen(name)+6);
  sprintf(dirname, "%s/src/%s", Cfg.homeDir, name);
  if (chdir(dirname) != 0)
  {
    printf(chdirErrorText, dirname);
    free(dirname);

    return 1;
  }

  printf(installingText, name);

  rc = system(makeCommand);

  chdir(cwd);

  if (rc == 0) printf(installedText, name);

  return rc;
}

// returns 0 if successfull
int compilesmapi()
{
  return doCompile("smapi", "make -f makefile.husky");
}

// returns 0 if successfull
int compilefconf()
{
  return doCompile("fidoconfig", "make -f makefile.husky");
}

// returns 0 if successfull
int compileprogs()
{
  int rc;

  rc = doCompile("hpt", "make -f makefile.husky");
  if (rc != 0) return rc;

  rc = doCompile("sqpack", "make -f makefile.husky");
  if (rc != 0) return rc;

  rc = doCompile("mpost", "make -f makefile.husky");
  if (rc != 0) return rc;

  rc = doCompile("msged", "make -f makefile.husky");
  if (rc != 0) return rc;

  rc = doCompile("husky-common", "make -f makefile.husky");
  if (rc != 0) return rc;

  rc = doCompile("ifcico", "make");
  if (rc != 0) return rc;

  return 0;
}

// returns 0 if successfull
int installsmapi()
{
  return doInstall("smapi", "make -f makefile.husky install");
}

// returns 0 if successfull
int installfconf()
{
  return doInstall("fidoconfig", "make -f makefile.husky install");
}

// returns 0 if successfull
int installprogs()
{
  int rc;

  rc = doInstall("hpt", "make -f makefile.husky install");
  if (rc != 0) return rc;

  rc = doInstall("sqpack", "make -f makefile.husky install");
  if (rc != 0) return rc;

  rc = doInstall("mpost", "make -f makefile.husky install");
  if (rc != 0) return rc;

  rc = doInstall("msged", "make -f makefile.husky install");
  if (rc != 0) return rc;

  rc = doInstall("husky-common", "make -f makefile.husky install");
  if (rc != 0) return rc;

  rc = doInstall("ifcico", "make install");
  if (rc != 0) return rc;

  return 0;
}

// returns 0 if successfull
int createconfig()
{
  char fname[1024];
  FILE *f;

  printf(creatingCfgText);

  // set permission mask
  umask(0022);

  sprintf(fname, "%s/config", Cfg.cfgDir);
  if (processTemplate("fconf.cfg", fname) != 0) return 1;
  chmod(fname, 0644);

  sprintf(fname, "%s/links.cfg", Cfg.cfgDir);
  if (processTemplate("links.cfg", fname) != 0) return 1;
  chmod(fname, 0600);

  sprintf(fname, "%s/msgbase.cfg", Cfg.cfgDir);
  if (touchFile(fname) != 0) return 1;
  chmod(fname, 0644);

  sprintf(fname, "%s/huskyui.cfg", Cfg.cfgDir);
  if (processTemplate("huskyui.cfg", fname) != 0) return 1;
  chmod(fname, 0644);

  sprintf(fname, "%s/msged.cfg", Cfg.cfgDir);
  if (processTemplate("msgedg.cfg", fname) != 0) return 1;
  chmod(fname, 0644);

  sprintf(fname, "%s/ifcico.cfg", Cfg.cfgDir);
  if (processTemplate("ifcico.cfg", fname) != 0) return 1;
  chmod(fname, 0644);

  f = fopen(fname, "a");
  if (f == NULL)
  {
    printf("Could not open '%s' for appending!\n", fname);

    return 1;
  }

  if (strlen(Cfg.isdnDev) > 0)
  {
    fprintf(f, "ModemPort\t(X75) %s:L115200\n", Cfg.isdnDev);
  }

  if (strlen(Cfg.modemDev) > 0)
  {
    fprintf(f, "ModemPort\t(V34) %s:L%s\n", Cfg.modemDev, Cfg.modemBaud);
    fprintf(f, "ModemPort\t(V32B) %s:L%s\n", Cfg.modemDev, Cfg.modemBaud);
    fprintf(f, "ModemPort\t(INTERN) %s:L%s\n", Cfg.modemDev, Cfg.modemBaud);
  }

  if ((strlen(Cfg.isdnDev) > 0) && (strlen(Cfg.modemDev) > 0))
  {
    fprintf(f, "Flags\tXA,X75,V34,V42B,U,ENC\n");
  }
  else if (strlen(Cfg.isdnDev) > 0)
  {
    fprintf(f, "Flags\tXA,X75,U,ENC\n");
  }
  else
  {
    fprintf(f, "Flags\tXA,V34,V42B,U,ENC\n");
  }

  fclose(f);

  sprintf(fname, "%s/password.lst", Cfg.cfgDir);
  if (processTemplate("password.lst", fname) != 0) return 1;
  chmod(fname, 0600);

  printf(createdCfgText);

  return 0;
}


// returns 0 if successfull
int createuserconfig(char *username)
{
  char fname[1024];
  char *oldSysOpName, *oldUserName;
  struct passwd *pw;

  printf(creatingUserCfgText, username);

  umask(0022);
  oldSysOpName = Cfg.sysOpName;
  oldUserName = Cfg.userName;

  pw = getpwnam(username);

  if (strcmp(username, Cfg.fidoName) != 0)
  {
    Cfg.sysOpName = strdup(pw->pw_gecos);
    if (strchr(Cfg.sysOpName, ',') != NULL) *strchr(Cfg.sysOpName, ',') = 0;
  }

  sprintf(fname, "%s/.msged", pw->pw_dir);
  if (processTemplate("msged.cfg", fname) != 0) return 1;

  sprintf(fname, "%s/.msged.scheme", pw->pw_dir);
  if (processTemplate("msged.scheme.cfg", fname) != 0) return 1;

  sprintf(fname, "%s/.msged.tpl", pw->pw_dir);
  if (processTemplate("msged.tpl", fname) != 0) return 1;

  if (strcmp(username, Cfg.fidoName) != 0) free(Cfg.sysOpName);
  Cfg.sysOpName = oldSysOpName;
  Cfg.userName = oldUserName;

  printf(createdUserCfgText, username);

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

// returns 0 if successfull
int copyNodelists()
{
  int i;
  DIR *dir;
  struct dirent *dirent;
  int ok = 0;

  printf(copyingNodelistsText);

  for (i = 0; i < numNodelists; i++)
  {
    // find nodelist
    dir = opendir(".");

    do
    {
      dirent = readdir(dir);
    } while ((dirent != NULL) &&
	     (strstr(dirent->d_name, nodelists[i]) != dirent->d_name));

    if (dirent == NULL)
    {
      printf("Nodelist '%s' not found!\n", nodelists[i]);
      ok++;
    }
    else
    {
      char *destname;

      destname = malloc(strlen(dirent->d_name)+strlen(Cfg.nodelistDir)+2);
      sprintf(destname, "%s/%s", Cfg.nodelistDir, dirent->d_name);
      copyFile(dirent->d_name, destname);
      free(destname);
    }

    closedir(dir);
  }

  printf(copiedNodelistsText);

  return ok;
}

// returns 0 if successfull
int copyScripts()
{
  DIR *dir;
  struct dirent *dirent;
  char *dirname;

  printf(copyingScriptsText);

  dirname = malloc(strlen(Cfg.homeDir)+13);
  sprintf(dirname, "%s/src/scripts", Cfg.homeDir);
  dir = opendir(dirname);
  if (dir == NULL)
  {
    printf("Could not open directory '%s'!\n", dirname);
    free(dirname);

    return 1;
  }

  dirent = readdir(dir);
  while (dirent != NULL)
  {
    char *destname;
    char *srcname;

    if (dirent->d_name[0] != '.')
    {
      srcname = malloc(strlen(dirent->d_name)+strlen(dirname)+2);
      destname = malloc(strlen(dirent->d_name)+strlen(Cfg.scriptDir)+2);
      sprintf(srcname, "%s/%s", dirname, dirent->d_name);
      sprintf(destname, "%s/%s", Cfg.scriptDir, dirent->d_name);
      processTemplate(srcname, destname);
      chmod(destname, 0755);
      free(srcname);
      free(destname);
    }

    dirent = readdir(dir);
  }

  closedir(dir);
  free(dirname);

  printf(copiedScriptsText);

  return 0;
}

// returns 0 if successfull
int compileNodelists()
{
  char *cmdline;
  int rc;

  printf(compilingNodelistsText);

  cmdline = malloc(strlen(Cfg.binDir)+9);
  sprintf(cmdline, "%s/ifindex", Cfg.binDir);
  rc = system(cmdline);

  if (rc == 0) printf(compiledNodelistsText);

  return rc;
}

void disposeCfg()
{
  nfree(Cfg.groupName);
  nfree(Cfg.fidoName);
  nfree(Cfg.users);
  nfree(Cfg.libDir);
  nfree(Cfg.binDir);
  nfree(Cfg.manDir);
  nfree(Cfg.cfgDir);
  nfree(Cfg.logDir);
  nfree(Cfg.incDir);
  nfree(Cfg.debug);
  nfree(Cfg.libcversion);
  nfree(Cfg.location);
  nfree(Cfg.sysOpName);
  nfree(Cfg.workDir);
  nfree(Cfg.inbound);
  nfree(Cfg.protInbound);
  nfree(Cfg.localInbound);
  nfree(Cfg.tempInbound);
  nfree(Cfg.tempOutbound);
  nfree(Cfg.outbound);
  nfree(Cfg.msgbaseDir);
  nfree(Cfg.nodelistDir);
  nfree(Cfg.netmailDir);
  nfree(Cfg.scriptDir);
  nfree(Cfg.isdnDev);
  nfree(Cfg.modemDev);
  nfree(Cfg.modemBaud);
  nfree(Cfg.internatPrefix);
  nfree(Cfg.localPrefix);
  nfree(Cfg.voiceNum);
  nfree(Cfg.dataNum);
  nfree(Cfg.amtNum);
  nfree(Cfg.localNum);
  nfree(Cfg.internatNum);
  nfree(Cfg.packer);
  nfree(Cfg.pointNr);
  nfree(Cfg.uplinkAddr);
  nfree(Cfg.uplinkName);
  nfree(Cfg.uplinkPwd);
  nfree(Cfg.homeDir);
}

int main(int argc, char *argv[])
{
  char *UserTmp;
  int pid;
  int childstat;

  printf("\n\n\nHuskyPoint V%s/%s\n", VERSION, OS);
  printf("Fido-Point-Packet\n");
  printf("Setup-Script by Sascha Silbe <Sascha.Silbe@ldknet.org>\n\n");

  if (geteuid() != 0)
  {
    printf(pleaseExecuteAsRootText);

    return 1;
  }

  if (checkprograms() != 0) return 2;
  if (checkfiles() != 0) return 3;

  getconfig();
  chooseboss();

  if (createusers() != 0) return 4;
  if (createdirs() != 0) return 5;
  if (ldsocfg() != 0) return 6;
  if (profilecfg() != 0) return 7;

  pid = fork();
  if (pid == 0)
  {
    setuid(Cfg.userId);
    setgid(Cfg.groupId);

    if (unpacksources() != 0) return 8;
    if (createmakeconfig() != 0) return 9;
    if (compilesmapi() != 0) return 10;

    return 0;
  }
  do { usleep(100000); waitpid(pid, &childstat, 0); }
  while (WIFEXITED(childstat) == 0);
  if (WEXITSTATUS(childstat) != 0) return WEXITSTATUS(childstat);
  if (installsmapi() != 0) return 11;

  pid = fork();
  if (pid == 0)
  {
    setuid(Cfg.userId);
    setgid(Cfg.groupId);

    if (compilefconf() != 0) return 12;

    return 0;
  }
  do { usleep(100000); waitpid(pid, &childstat, 0); }
  while (WIFEXITED(childstat) == 0);
  if (WEXITSTATUS(childstat) != 0) return WEXITSTATUS(childstat);
  if (installfconf() != 0) return 13;

  pid = fork();
  if (pid == 0)
  {
    setuid(Cfg.userId);
    setgid(Cfg.groupId);

    if (compileprogs() != 0) return 14;

    return 0;
  }
  do { usleep(100000); waitpid(pid, &childstat, 0); }
  while (WIFEXITED(childstat) == 0);
  if (WEXITSTATUS(childstat) != 0) return WEXITSTATUS(childstat);
  if (installprogs() != 0) return 15;
  if (copyScripts() != 0) return 16;

  getuplinkconfig();

  pid = fork();
  if (pid == 0)
  {
    setuid(Cfg.userId);
    setgid(Cfg.groupId);

    if (createconfig() != 0) return 17;
    if (createuserconfig(Cfg.fidoName) != 0) return 18;
    if (copyNodelists() != 0) return 19;
    if (compileNodelists() != 0) return 20;

    return 0;
  }
  do { usleep(100000); waitpid(pid, &childstat, 0); }
  while (WIFEXITED(childstat) == 0);
  if (WEXITSTATUS(childstat) != 0) return WEXITSTATUS(childstat);

  UserTmp = Cfg.users;
  while (strlen(UserTmp) > 0)
  {
    char *UName;
    int pos;

    if (strchr(UserTmp, ',') == NULL) pos = strlen(UserTmp);
    else pos = strchr(UserTmp, ',') - UserTmp;
    UName = (char *) malloc(pos+1);
    strncpy(UName, UserTmp, pos);
    UName[pos] = 0;

    pid = fork();
    if (pid == 0)
    {
      struct passwd *pw;

      pw = getpwnam(UName);
      setuid(pw->pw_uid);
      setgid(pw->pw_gid);

      createuserconfig(UName);

      return 0;
    }
    do { usleep(100000); waitpid(pid, &childstat, 0); }
    while (WIFEXITED(childstat) == 0);
    free(UName);

    UserTmp = UserTmp + pos + 1;
  }

  printf(installDoneText, Cfg.fidoName);

  disposeCfg();

  return 0;
}

