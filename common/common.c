/* some variables and functions common to all platforms and languages */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "auto.h"
#include "common.h"
#include "lang.h"
#include "os.h"


char *cfg[numIdx];


// numXXX are defined in common.h

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

// returns 0 on success
int setTemplateVars()
{
  int rc;

  rc = 0;

  rc += setVar("amtnum", cfg[amtNumIdx]);
  rc += setVar("bindir", cfg[binDirIdx]);
  rc += setVar("cfgdir", cfg[cfgDirIdx]);
  rc += setVar("datanum", cfg[dataNumIdx]);
  rc += setVar("debug", cfg[debugIdx]);
  rc += setVar("dirsep", dirSepS);
  rc += setVar("fidoname", cfg[fidoNameIdx]);
  rc += setVar("groupname", cfg[groupNameIdx]);
  rc += setVar("homedir", cfg[homeDirIdx]);
  rc += setVar("htmldir", cfg[htmlDirIdx]);
  rc += setVar("inbound", cfg[inboundIdx]);
  rc += setVar("incdir", cfg[incDirIdx]);
  rc += setVar("infodir", cfg[infoDirIdx]);
  rc += setVar("internatnum", cfg[internatNumIdx]);
  rc += setVar("internatprefix", cfg[internatPrefixIdx]);
  rc += setVar("isdndev", cfg[isdnDevIdx]);
  rc += setVar("langdir", langDir);
  rc += setVar("libcversion", cfg[libcVersionIdx]);
  rc += setVar("libdir", cfg[libDirIdx]);
  rc += setVar("localinbound", cfg[localInboundIdx]);
  rc += setVar("localnum", cfg[localNumIdx]);
  rc += setVar("localprefix", cfg[localPrefixIdx]);
  rc += setVar("location", cfg[locationIdx]);
  rc += setVar("logdir", cfg[logDirIdx]);
  rc += setVar("mandir", cfg[manDirIdx]);
  rc += setVar("modembaud", cfg[modemBaudIdx]);
  rc += setVar("modemdev", cfg[modemDevIdx]);
  rc += setVar("msgbasedir", cfg[msgbaseDirIdx]);
  rc += setVar("netmaildir", cfg[netmailDirIdx]);
  rc += setVar("nodelistdir", cfg[nodelistDirIdx]);
  rc += setVar("outbound", cfg[outboundIdx]);
  rc += setVar("packer", cfg[packerIdx]);
  rc += setVar("pointnr", cfg[pointNrIdx]);
  rc += setVar("protinbound", cfg[protInboundIdx]);
  rc += setVar("scriptdir", cfg[scriptDirIdx]);
  rc += setVar("sysopname", cfg[sysOpNameIdx]);
  rc += setVar("tempinbound", cfg[tempInboundIdx]);
  rc += setVar("tempoutbound", cfg[tempOutboundIdx]);
  rc += setVar("uplinkaddr", cfg[uplinkAddrIdx]);
  rc += setVar("uplinkname", cfg[uplinkNameIdx]);
  rc += setVar("uplinkpwd", cfg[uplinkPwdIdx]);
  rc += setVar("username", cfg[userNameIdx]);
  rc += setVar("users", cfg[usersIdx]);
  rc += setVar("voicenum", cfg[voiceNumIdx]);
  rc += setVar("workdir", cfg[workDirIdx]);

  return rc;
}

// returns 0 if successfull
int processTemplate(char *templName, char *outName)
{
  FILE *templ;
  FILE *out;
  char lineIn[1024], lineOut[1024];
  char *lineInTmp, *lineOutTmp;
  int rc;
  char *cmdLine;
  char *tmpName;

  setTemplateVars();

  cmdLine = malloc(strlen(templName) + strlen(osDir) + strlen(osTmpDir) + 18);
  sprintf(cmdLine, "%s" dirSepS "lwpp %s > %slwpp.tmp", osDir, templName,
	  osTmpDir);
  rc = system(cmdLine);
  free(cmdLine);
  if (rc != 0)
  {
    printf("lwpp returned error code #%d!\n", rc);

    return rc;
  }

  tmpName = malloc(strlen(osTmpDir)+9);
  sprintf(tmpName, "%slwpp.tmp", osTmpDir);
  templ = fopen(tmpName, "r");

  if (templ == NULL)
  {
    printf("Could not open temporary file '%s'!\n", tmpName);
    free(tmpName);

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
	varContent = getVar(varName);
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
  unlink(tmpName);
  free(tmpName);

  return 0;
}

