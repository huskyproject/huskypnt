/* some variables and functions common to all platforms and languages */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "lang.h"
#include "os.h"


tCfg Cfg;


// numXXX are defined in common.h

char *zipFiles[numZipFiles] =
{
  "smapi.zip", "fidoconf.zip", "hpt.zip", "hptutil.zip", "sqpack.zip",
  "mpost.zip", "msged.zip", "ifcico.zip", "huskycom.zip", "scripts.zip"
};

char *programs[numPrograms] =
{
  "unzip", "passwd"
};

char *nodelists[numNodelists] =
{
  "nodelist", "points24"
};

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

