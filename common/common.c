/* some variables and functions common to all platforms and languages */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

