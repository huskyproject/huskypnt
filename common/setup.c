/* setup-script for HuskyPoint/Lnx
   author: Sascha Silbe
   last change: 12.04.00
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>

#include "common.h"
#include "lang.h"
#include "os.h"


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
    Cfg.libcVersion = ask(questions[libcVersionIdx], defaults[libcVersionIdx]);
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
  char *nodes;
  char *fname;

  fname = malloc(strlen(langDir) + 11);
  sprintf(fname, "%s" dirSepS "nodes.lst", langDir);
  nodes = readFile(fname);
  printf(chooseBossText, nodes);
  free(fname);
  free(nodes);

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
    Cfg.packer = ask(questions[packerIdx], defaults[packerIdx]);

    ok = askAllright();
  }
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

  setMode(Cfg.libDir, 493); // dec. 493 = octal 0755
  setMode(Cfg.binDir, 493);
  setMode(Cfg.manDir, 493);
  sprintf(dirname, "%s/man1", Cfg.manDir); setMode(dirname, 493);
  sprintf(dirname, "%s/man2", Cfg.manDir); setMode(dirname, 493);
  sprintf(dirname, "%s/man3", Cfg.manDir); setMode(dirname, 493);
  sprintf(dirname, "%s/man4", Cfg.manDir); setMode(dirname, 493);
  sprintf(dirname, "%s/man5", Cfg.manDir); setMode(dirname, 493);
  sprintf(dirname, "%s/man6", Cfg.manDir); setMode(dirname, 493);
  sprintf(dirname, "%s/man7", Cfg.manDir); setMode(dirname, 493);
  sprintf(dirname, "%s/man8", Cfg.manDir); setMode(dirname, 493);
  setMode(Cfg.incDir, 493);
  setMode(Cfg.cfgDir, 493);
  setMode(Cfg.logDir, 448); // dec. 448 = octal 0700
  setMode(Cfg.scriptDir, 493);
  setMode(Cfg.workDir, 493);
  setMode(Cfg.inbound, 448);
  setMode(Cfg.localInbound, 448);
  setMode(Cfg.tempInbound, 448);
  setMode(Cfg.protInbound, 448);
  setMode(Cfg.outbound, 448);
  setMode(Cfg.tempOutbound, 448);
  setMode(Cfg.msgbaseDir, 493);
  setMode(Cfg.netmailDir, 448);
  setMode(Cfg.nodelistDir, 493);
  sprintf(dirname, "%s/dupes", Cfg.workDir); setMode(dirname, 493);
  sprintf(dirname, "%s/src", Cfg.homeDir); setMode(dirname, 493);
  sprintf(dirname, "%s/tmp", Cfg.inbound); setMode(dirname, 448);
  sprintf(dirname, "%s/tmp", Cfg.protInbound); setMode(dirname, 448);

  // parent directory of outbound (.../out/zone -> .../out)
  pos = strrchr(Cfg.outbound, '/') - Cfg.outbound;
  strncpy(dirname, Cfg.outbound, pos); dirname[pos] = 0;
  setMode(dirname, 448);

  // dec. 483 = octal 0600
  if (strlen(Cfg.isdnDev) > 0) setMode(Cfg.isdnDev, 483); 
  if (strlen(Cfg.modemDev) > 0) setMode(Cfg.isdnDev, 483);

  setOwner(Cfg.cfgDir, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.logDir, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.scriptDir, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.workDir, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.inbound, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.localInbound, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.protInbound, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.tempInbound, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.outbound, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.tempOutbound, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.msgbaseDir, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.nodelistDir, Cfg.fidoName, Cfg.groupName);
  setOwner(Cfg.netmailDir, Cfg.fidoName, Cfg.groupName);
  sprintf(dirname, "%s/dupes", Cfg.workDir);
  setOwner(dirname, Cfg.fidoName, Cfg.groupName);
  sprintf(dirname, "%s/src", Cfg.homeDir);
  setOwner(dirname, Cfg.fidoName, Cfg.groupName);
  sprintf(dirname, "%s/tmp", Cfg.inbound);
  setOwner(dirname, Cfg.fidoName, Cfg.groupName);
  sprintf(dirname, "%s/tmp", Cfg.protInbound);
  setOwner(dirname, Cfg.fidoName, Cfg.groupName);

  // parent directory of outbound (.../out/zone -> .../out)
  pos = strrchr(Cfg.outbound, '/') - Cfg.outbound;
  strncpy(dirname, Cfg.outbound, pos); dirname[pos] = 0;
  setOwner(dirname, Cfg.fidoName, Cfg.groupName);

  if (Cfg.isdnDev != NULL)
    setOwner(Cfg.isdnDev, Cfg.fidoName, Cfg.groupName);
  if (Cfg.modemDev != NULL)
    setOwner(Cfg.modemDev, Cfg.fidoName, Cfg.groupName);

  printf(createdDirsText);

  return 0;
}

// returns 0 if successfull
int unpacksources(char *userName, char *groupName)
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
int createmakeconfig(char *userName, char *groupName)
{
  char *fname, *fname2;

  printf(creatingMakefileCfgsText);

  fname = malloc(strlen(Cfg.homeDir)+18);
  fname2 = malloc(strlen(langDir)+14);
  sprintf(fname, "%s" dirSepS "src" dirSepS "huskymak.cfg", Cfg.homeDir);
  sprintf(fname2, "%s" dirSepS "huskymak.cfg", langDir);
  if (processTemplate(fname2, fname) != 0)
  {
    free(fname);
    free(fname2);

    return 1;
  }
  free(fname);
  free(fname2);

  fname = malloc(strlen(Cfg.homeDir)+19);
  fname2 = malloc(strlen(langDir)+14);
  sprintf(fname, "%s" dirSepS "src" dirSepS "ifcico" dirSepS "CONFIG",
	  Cfg.homeDir);
  sprintf(fname2, "%s" dirSepS "ifcicomk.cfg", langDir);
  if (processTemplate(fname2, fname) != 0)
  {
    free(fname);
    free(fname2);

    return 1;
  }
  free(fname);
  free(fname2);

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
int compilesmapi(char *userName, char *groupName)
{
  return doCompile("smapi", "make");
}

// returns 0 if successfull
int compilefconf(char *userName, char *groupName)
{
  return doCompile("fidoconf", "make");
}

// returns 0 if successfull
int compileprogs(char *userName, char *groupName)
{
  int rc;

  rc = doCompile("hpt", "make");
  if (rc != 0) return rc;

  rc = doCompile("hptutil", "make");
  if (rc != 0) return rc;

  rc = doCompile("sqpack", "make");
  if (rc != 0) return rc;

  rc = doCompile("mpost", "make");
  if (rc != 0) return rc;

  rc = doCompile("msged", "make");
  if (rc != 0) return rc;

  rc = doCompile("husky-common", "make");
  if (rc != 0) return rc;

  rc = doCompile("ifcico", "make");
  if (rc != 0) return rc;

  return 0;
}

// returns 0 if successfull
int installsmapi()
{
  return doInstall("smapi", "make install");
}

// returns 0 if successfull
int installfconf()
{
  return doInstall("fidoconf", "make install");
}

// returns 0 if successfull
int installprogs()
{
  int rc;

  rc = doInstall("hpt", "make install");
  if (rc != 0) return rc;

  rc = doInstall("hptutil", "make install");
  if (rc != 0) return rc;

  rc = doInstall("sqpack", "make install");
  if (rc != 0) return rc;

  rc = doInstall("mpost", "make install");
  if (rc != 0) return rc;

  rc = doInstall("msged", "make install");
  if (rc != 0) return rc;

  rc = doInstall("husky-common", "make install");
  if (rc != 0) return rc;

  rc = doInstall("ifcico", "make install");
  if (rc != 0) return rc;

  return 0;
}

// returns 0 if successfull
int createconfig(char *userName, char *groupName)
{
  char fname[1024], fname2[1024];

  printf(creatingCfgText);

  // set permission mask
  setUmask(0022);

  sprintf(fname, "%s" dirSepS "config", Cfg.cfgDir);
  sprintf(fname2, "%s" dirSepS "fconf.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setMode(fname, 0644);

  sprintf(fname, "%s" dirSepS "links.cfg", Cfg.cfgDir);
  sprintf(fname2, "%s" dirSepS "links.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setMode(fname, 0600);

  sprintf(fname, "%s" dirSepS "msgbase.cfg", Cfg.cfgDir);
  if (touchFile(fname) != 0) return 1;
  setMode(fname, 0644);

  sprintf(fname, "%s" dirSepS "huskyui.cfg", Cfg.cfgDir);
  sprintf(fname2, "%s" dirSepS "huskyui.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setMode(fname, 0644);

  sprintf(fname, "%s" dirSepS "msged.cfg", Cfg.cfgDir);
  sprintf(fname2, "%s" dirSepS "msgedg.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setMode(fname, 0644);

  sprintf(fname, "%s" dirSepS "ifcico.cfg", Cfg.cfgDir);
  sprintf(fname2, "%s" dirSepS "ifcico.cfg", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setMode(fname, 0644);

  sprintf(fname, "%s" dirSepS "password.lst", Cfg.cfgDir);
  sprintf(fname2, "%s" dirSepS "password.lst", langDir);
  if (processTemplate(fname2, fname) != 0) return 1;
  setMode(fname, 0600);

  printf(createdCfgText);

  return 0;
}


// returns 0 if successfull
int copyNodelists(char *userName, char *groupName)
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
      setMode(destname, 0755);
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
int compileNodelists(char *userName, char *groupName)
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
  nfree(Cfg.libcVersion);
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

int doMain()
{
  char *UserTmp;
  int rc;

  if (checkRootAccess() != 0)
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
  if (createSystemConfigPrecompile() != 0) return 6;

  rc = callAsUser(Cfg.fidoName, Cfg.groupName, unpacksources);
  if (rc != 0) return rc;
  rc = callAsUser(Cfg.fidoName, Cfg.groupName, createmakeconfig);
  if (rc != 0) return rc;
  rc = callAsUser(Cfg.fidoName, Cfg.groupName, compilesmapi);
  if (rc != 0) return rc;

  if (installsmapi() != 0) return 11;

  rc = callAsUser(Cfg.fidoName, Cfg.groupName, compilefconf);
  if (rc != 0) return rc;

  if (installfconf() != 0) return 13;

  rc = callAsUser(Cfg.fidoName, Cfg.groupName, compileprogs);
  if (rc != 0) return rc;

  if (installprogs() != 0) return 15;
  if (copyScripts() != 0) return 16;

  getuplinkconfig();
  createSystemConfigAfterInstall();

  rc = callAsUser(Cfg.fidoName, Cfg.groupName, createconfig);
  if (rc != 0) return rc;
  rc = callAsUser(Cfg.fidoName, Cfg.groupName, createUserConfig);
  if (rc != 0) return rc;
  rc = callAsUser(Cfg.fidoName, Cfg.groupName, copyNodelists);
  if (rc != 0) return rc;
  rc = callAsUser(Cfg.fidoName, Cfg.groupName, compileNodelists);
  if (rc != 0) return rc;

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

    callAsUser(UName, Cfg.groupName, createUserConfig);
    free(UName);

    UserTmp = UserTmp + pos + 1;
  }

  printf(installDoneText, Cfg.fidoName);

  return 0;
}

int main(int argc, char *argv[])
{
  char *oldDir;
  int rc;

  printf("\n\n\nHuskyPoint V%s/%s\n", VERSION, OS);
  printf("Fido-Point-Packet\n");
  printf("Setup-Script by Sascha Silbe <Sascha.Silbe@ldknet.org>\n\n");

  memset(&Cfg, 0, sizeof(tCfg));

  oldDir = malloc(1024);
  getcwd(oldDir, 1024);
  chdir("..");

  rc = doMain();

  chdir(oldDir);
  free(oldDir);
  disposeCfg();

  return rc;
}

