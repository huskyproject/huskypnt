/* HuskyPoint
   author: Sascha Silbe
   last change: see CVS log :)
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>

#include "auto.h"
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

char menuChoice(char *validChoices)
{
  char c;
  char *s;
  int ok = 1;

  s = malloc(5);
  while (ok != 0)
  {
    printf("your choice: ");
    fgets(s, 5, stdin);
    c = *s;
    if (strchr(validChoices, c) != NULL) ok = 0;
    else printf(menuInvalidChoice);
  }
  free(s);

  return c;
}

void editVar(int idx)
{
  char *newVal;

  clrscr();
  printf(descTexts[idx]);
  printf("old value: %s\n"
	 "new value: ", cfg[idx]);
  newVal = malloc(1024);
  *newVal = 0;
  fgets(newVal, 1024, stdin);

  // strip CR/LF
  while ((*newVal != 0) && ((newVal[strlen(newVal) - 1] == 13) ||
			    (newVal[strlen(newVal) - 1] == 10)))
    newVal[strlen(newVal) - 1] = 0;

  free(cfg[idx]);
  cfg[idx] = strdup(newVal);
  free(newVal);
}

void useMenu(char *title, int numEntries, tMenuEntry *entries)
{
  int ok = 1;
  int c;
  int i;
  char *keys;

  keys = malloc(numEntries + 2);
  keys[numEntries] = 'x';
  keys[numEntries + 1] = 0;

  while (ok != 0)
  {
    clrscr();
    printf(title);

    for (i = 0; i < numEntries; i++)
    {
      printf(entries[i].text, cfg[entries[i].idx]);
      keys[i] = entries[i].key;
    }

    printf("\n"
	   " x) return\n"
	   "\n");

    c = menuChoice(keys);
    if (c != 'x')
    {
      editVar(entries[strchr(keys, c) - keys].idx);
    }
    else ok = 0;
  }
}

// get config (without uplink)
void getconfig()
{
  int ok = 1;
  int c;
  int i;

  for (i = 0; i < numIdx; i++) cfg[i] = strdup(defaults[i]);

  while (ok != 0)
  {
    clrscr();
    printf(configMenus);

    c = menuChoice("1234x");
    switch (c)
    {
    case '1':
      useMenu(systemConfigTitle, numSystemConfigEntries, systemConfigEntries);
      break;

    case '2':
      useMenu(personalConfigTitle, numPersonalConfigEntries,
	      personalConfigEntries);
      break;

    case '3':
      useMenu(fidoDirConfigTitle, numFidoDirConfigEntries,
	      fidoDirConfigEntries);
      break;

    case '4':
      useMenu(commConfigTitle, numCommConfigEntries, commConfigEntries);
      break;

    case 'x':
      ok = 0;
      break;
    }
  }

  cfg[userNameIdx] = cfg[fidoNameIdx];
  clrscr();
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
  useMenu(uplinkConfigTitle, numUplinkConfigEntries, uplinkConfigEntries);
}

// returns 0 if successfull
int createdirs()
{
  char dirname[1024];
  int pos;

  printf(creatingDirsText);

  if (direxist(cfg[libDirIdx]) != 0)
    if (mkdirp(cfg[libDirIdx]) != 0) return 1;
  if (direxist(cfg[binDirIdx]) != 0)
    if (mkdirp(cfg[binDirIdx]) != 0) return 1;
  if (direxist(cfg[manDirIdx]) != 0)
    if (mkdirp(cfg[manDirIdx]) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man1", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man2", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man3", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man4", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man5", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man6", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man7", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "man8", cfg[manDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(cfg[incDirIdx]) != 0)
    if (mkdirp(cfg[incDirIdx]) != 0) return 1;
  if (direxist(cfg[cfgDirIdx]) != 0)
    if (mkdirp(cfg[cfgDirIdx]) != 0) return 1;
  if (direxist(cfg[logDirIdx]) != 0)
    if (mkdirp(cfg[logDirIdx]) != 0) return 1;
  if (direxist(cfg[scriptDirIdx]) != 0)
    if (mkdirp(cfg[scriptDirIdx]) != 0) return 1;
  if (direxist(cfg[workDirIdx]) != 0)
    if (mkdirp(cfg[workDirIdx]) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "dupes", cfg[workDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "src", cfg[homeDirIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(cfg[inboundIdx]) != 0)
    if (mkdirp(cfg[inboundIdx]) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "tmp", cfg[inboundIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(cfg[localInboundIdx]) != 0)
    if (mkdirp(cfg[localInboundIdx]) != 0) return 1;
  if (direxist(cfg[tempInboundIdx]) != 0)
    if (mkdirp(cfg[tempInboundIdx]) != 0) return 1;
  if (direxist(cfg[protInboundIdx]) != 0)
    if (mkdirp(cfg[protInboundIdx]) != 0) return 1;
  sprintf(dirname, "%s" dirSepS "tmp", cfg[protInboundIdx]);
  if (direxist(dirname) != 0)
    if (mkdirp(dirname) != 0) return 1;
  if (direxist(cfg[outboundIdx]) != 0)
    if (mkdirp(cfg[outboundIdx]) != 0) return 1;
  if (direxist(cfg[tempOutboundIdx]) != 0)
    if (mkdirp(cfg[tempOutboundIdx]) != 0) return 1;
  if (direxist(cfg[msgbaseDirIdx]) != 0)
    if (mkdirp(cfg[msgbaseDirIdx]) != 0) return 1;
  if (direxist(cfg[nodelistDirIdx]) != 0)
    if (mkdirp(cfg[nodelistDirIdx]) != 0) return 1;
  if (direxist(cfg[netmailDirIdx]) != 0)
    if (mkdirp(cfg[netmailDirIdx]) != 0) return 1;

  printf(settingRightsText);

  setMode(cfg[libDirIdx], 493); // dec. 493 = octal 0755
  setMode(cfg[binDirIdx], 493);
  setMode(cfg[manDirIdx], 493);
  sprintf(dirname, "%s" dirSepS "man1", cfg[manDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "man2", cfg[manDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "man3", cfg[manDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "man4", cfg[manDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "man5", cfg[manDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "man6", cfg[manDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "man7", cfg[manDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "man8", cfg[manDirIdx]); setMode(dirname, 493);
  setMode(cfg[incDirIdx], 493);
  setMode(cfg[cfgDirIdx], 493);
  setMode(cfg[logDirIdx], 448); // dec. 448 = octal 0700
  setMode(cfg[scriptDirIdx], 493);
  setMode(cfg[workDirIdx], 493);
  setMode(cfg[inboundIdx], 448);
  setMode(cfg[localInboundIdx], 448);
  setMode(cfg[tempInboundIdx], 448);
  setMode(cfg[protInboundIdx], 448);
  setMode(cfg[outboundIdx], 448);
  setMode(cfg[tempOutboundIdx], 448);
  setMode(cfg[msgbaseDirIdx], 493);
  setMode(cfg[netmailDirIdx], 448);
  setMode(cfg[nodelistDirIdx], 493);
  sprintf(dirname, "%s" dirSepS "dupes", cfg[workDirIdx]);
  setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "src", cfg[homeDirIdx]); setMode(dirname, 493);
  sprintf(dirname, "%s" dirSepS "tmp", cfg[inboundIdx]); setMode(dirname, 448);
  sprintf(dirname, "%s" dirSepS "tmp", cfg[protInboundIdx]);
  setMode(dirname, 448);

  // parent directory of outbound (.../out/zone -> .../out)
  pos = strrchr(cfg[outboundIdx], '/') - cfg[outboundIdx];
  strncpy(dirname, cfg[outboundIdx], pos); dirname[pos] = 0;
  setMode(dirname, 448);

  // dec. 483 = octal 0600
  if (strlen(cfg[isdnDevIdx]) > 0) setMode(cfg[isdnDevIdx], 483); 
  if (strlen(cfg[modemDevIdx]) > 0) setMode(cfg[isdnDevIdx], 483);

  setOwner(cfg[cfgDirIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[logDirIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[scriptDirIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[workDirIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[inboundIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[localInboundIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[protInboundIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[tempInboundIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[outboundIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[tempOutboundIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[msgbaseDirIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[nodelistDirIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  setOwner(cfg[netmailDirIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  sprintf(dirname, "%s" dirSepS "dupes", cfg[workDirIdx]);
  setOwner(dirname, cfg[fidoNameIdx], cfg[groupNameIdx]);
  sprintf(dirname, "%s" dirSepS "src", cfg[homeDirIdx]);
  setOwner(dirname, cfg[fidoNameIdx], cfg[groupNameIdx]);
  sprintf(dirname, "%s" dirSepS "tmp", cfg[inboundIdx]);
  setOwner(dirname, cfg[fidoNameIdx], cfg[groupNameIdx]);
  sprintf(dirname, "%s" dirSepS "tmp", cfg[protInboundIdx]);
  setOwner(dirname, cfg[fidoNameIdx], cfg[groupNameIdx]);

  // parent directory of outbound (.../out/zone -> .../out)
  pos = strrchr(cfg[outboundIdx], dirSepC) - cfg[outboundIdx];
  strncpy(dirname, cfg[outboundIdx], pos); dirname[pos] = 0;
  setOwner(dirname, cfg[fidoNameIdx], cfg[groupNameIdx]);

  if (cfg[isdnDevIdx] != NULL)
    setOwner(cfg[isdnDevIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);
  if (cfg[modemDevIdx] != NULL)
    setOwner(cfg[modemDevIdx], cfg[fidoNameIdx], cfg[groupNameIdx]);

  printf(createdDirsText);

  return 0;
}

// returns 0 if successfull
int unpacksources(char *userName, char *groupName)
{
  char *dirname;
  char *cwd;
  char *fname;
  int i;
  int rc;

  printf(unzipSourcesText);

  cwd = malloc(1024);
  getcwd(cwd, 1024);
  dirname = malloc(strlen(cfg[homeDirIdx]) + 5);
  sprintf(dirname, "%s" dirSepS "src", cfg[homeDirIdx]);

  rc = chdir(dirname);
  if (rc != 0)
  {
    printf(chdirErrorText, dirname);
    free(dirname);
    free(cwd);

    return rc;
  }

  for (i = 0; i < numZipFiles; i++)
  {
    fname = malloc(strlen(cwd) + strlen(zipFiles[i]) + 2);
    sprintf(fname, "%s" dirSepS "%s", cwd, zipFiles[i]);

    rc = unpackFile(fname);
    if (rc != 0)
    {
      chdir(cwd);
      free(dirname);
      free(cwd);

      return rc;
    }
  }

  chdir(cwd);
  free(dirname);
  free(cwd);

  return 0;
}

// returns 0 if successfull
int doCompile(char *name, char *makeCommand)
{
  char cwd[1024];
  char *dirname;
  int rc;

  getcwd(cwd, 1024);

  dirname = malloc(strlen(cfg[homeDirIdx])+strlen(name)+6);
  sprintf(dirname, "%s" dirSepS "src" dirSepS "%s", cfg[homeDirIdx], name);
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

  dirname = malloc(strlen(cfg[homeDirIdx])+strlen(name)+6);
  sprintf(dirname, "%s" dirSepS "src" dirSepS "%s", cfg[homeDirIdx], name);
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
  int i;

  for (i = 0; i < numPrograms; i++)
  {
    rc = doCompile(programs[i], "make");
    if (rc != 0) return rc;
  }

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
  int i;

  for (i = 0; i < numPrograms; i++)
  {
    rc = doInstall(programs[i], "make install");
    if (rc != 0) return rc;
  }

  return 0;
}

// returns 0 is successfull
int createMakeConfig()
{
  char *src, *dest;
  int i;
  int rc;

  printf(creatingMakefileCfgsText);

  for (i = 0; i < numMakeCfgFiles; i++)
  {
    dest = malloc(strlen(cfg[homeDirIdx])+strlen(makeCfgFiles[i].destFile)+2);
    sprintf(dest, "%s" dirSepS "%s", cfg[homeDirIdx],
	    makeCfgFiles[i].destFile);
    if (makeCfgFiles[i].sourceFile != NULL)
    {
      src = malloc(strlen(langDir)+strlen(makeCfgFiles[i].sourceFile)+2);
      sprintf(src, "%s" dirSepS "%s", langDir, makeCfgFiles[i].sourceFile);
      rc = processTemplate(src, dest);
      free(src);
    }
    else rc = touchFile(dest);

    setMode(dest, makeCfgFiles[i].destMode);
    free(dest);

    if (rc != 0) return rc;
  }

  printf(createdMakefileCfgsText);

  return 0;
}

// returns 0 if successfull
int createGlobalConfig(char *userName, char *groupName)
{
  char *src, *dest;
  int i;
  int rc;

  printf(creatingCfgText);

  // set permission mask
  setUmask(0022);

  for (i = 0; i < numGlobalCfgFiles; i++)
  {
    dest = malloc(strlen(cfg[cfgDirIdx])+strlen(globalCfgFiles[i].destFile)+2);
    sprintf(dest, "%s" dirSepS "%s", cfg[cfgDirIdx],
	    globalCfgFiles[i].destFile);
    if (globalCfgFiles[i].sourceFile != NULL)
    {
      src = malloc(strlen(langDir)+strlen(globalCfgFiles[i].sourceFile)+2);
      sprintf(src, "%s" dirSepS "%s", langDir, globalCfgFiles[i].sourceFile);
      rc = processTemplate(src, dest);
      free(src);
    }
    else rc = touchFile(dest);

    setMode(dest, globalCfgFiles[i].destMode);
    free(dest);

    if (rc != 0) return rc;
  }

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

      destname = malloc(strlen(dirent->d_name)+strlen(cfg[nodelistDirIdx])+2);
      sprintf(destname, "%s" dirSepS "%s", cfg[nodelistDirIdx],
	      dirent->d_name);
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

  dirname = malloc(strlen(cfg[homeDirIdx])+13);
  sprintf(dirname, "%s" dirSepS "src" dirSepS "scripts", cfg[homeDirIdx]);
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
      destname = malloc(strlen(dirent->d_name)+strlen(cfg[scriptDirIdx])+2);
      sprintf(srcname, "%s" dirSepS "%s", dirname, dirent->d_name);
      sprintf(destname, "%s" dirSepS "%s", cfg[scriptDirIdx], dirent->d_name);
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

void disposeCfg()
{
  int i;

  // ignore userNameIdx
  for (i = 0; i < numIdx; i++)
    if (i != userNameIdx) nfree(cfg[i]);
}

int doMain()
{
  char *UserTmp;
  int rc;

  if (checkRootAccess() != 0)
  {
    printf(pleaseExecuteAsRootText);

    return rcNotRoot;
  }

  if (checkfiles() != 0) return rcCheckFiles;

  getconfig();
  chooseboss();

  if (createusers() != 0) return rcCreateUsers;
  if (createdirs() != 0) return rcCreateDirs;
  if (createSystemConfigPrecompile() != 0) return rcCreateSystemConfigPrecompile;

  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], unpacksources);
  if (rc != 0) return rcUnpackSources;
  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], createMakeConfig);
  if (rc != 0) return rcCreateMakeConfig;
  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], compilesmapi);
  if (rc != 0) return rcCompileSmapi;

  if (installsmapi() != 0) return rcInstallSmapi;

  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], compilefconf);
  if (rc != 0) return rcCompileFconf;

  if (installfconf() != 0) return rcInstallFconf;

  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], compileprogs);
  if (rc != 0) return rcCompileProgs;

  if (installprogs() != 0) return rcInstallProgs;
  if (copyScripts() != 0) return rcCopyScripts;

  getuplinkconfig();
  createSystemConfigAfterInstall();

  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], createGlobalConfig);
  if (rc != 0) return rcCreateConfig;
  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], createUserConfig);
  if (rc != 0) return rcCreateUserConfig;
  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], copyNodelists);
  if (rc != 0) return rcCopyNodelists;
  rc = callAsUser(cfg[fidoNameIdx], cfg[groupNameIdx], compileNodelists);
  if (rc != 0) return rcCompileNodelists;

  UserTmp = cfg[usersIdx];
  while (strlen(UserTmp) > 0)
  {
    char *UName;
    int pos;

    if (strchr(UserTmp, ',') == NULL) pos = strlen(UserTmp);
    else pos = strchr(UserTmp, ',') - UserTmp;
    UName = (char *) malloc(pos+1);
    strncpy(UName, UserTmp, pos);
    UName[pos] = 0;

    callAsUser(UName, cfg[groupNameIdx], createUserConfig);
    free(UName);

    UserTmp = UserTmp + pos + 1;
  }

  printf(installDoneText, cfg[fidoNameIdx]);

  return 0;
}

int main(int argc, char *argv[])
{
  char *oldDir;
  int rc;

  printf("\n\n\nHuskyPoint V%s/%s\n", VERSION, OS);
  printf("Fido-Point-Packet\n");
  printf("written by Sascha Silbe <Sascha.Silbe@ldknet.org>\n\n");

  rc = osInit();
  if (rc != 0) return rc;

  memset(&cfg, 0, numIdx * sizeof(char *));

  // we are in the operating system directory => go to top directory
  oldDir = malloc(1024);
  getcwd(oldDir, 1024);
  chdir("..");

  rc = doMain();

  // back to OS dir, free all variables
  chdir(oldDir);
  free(oldDir);
  disposeCfg();
  osDone();

  return rc;
}

