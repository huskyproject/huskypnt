/* rlsubst.c - substitute / wrapper functions for readline */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auto.h"

#ifndef HAVE_READLINE

char *_rlLastLine = NULL;

char *readline(char *prompt)
{
  char *inputBuf;
  char *res;
  char *bufTmp;

  inputBuf = malloc(1024);
  *inputBuf = 0;

  printf(prompt, _rlLastLine ? _rlLastLine : "");
  fgets(inputBuf, 1024, stdin);

  if (*inputBuf)
  {
    // strip trailing CRs and LFs
    bufTmp = inputBuf + strlen(inputBuf) - 1;
    while ((bufTmp > inputBuf) && ((*bufTmp == 10) || (*bufTmp == 13)))
      bufTmp--;
    if ((*bufTmp == 10) || (*bufTmp == 13)) *bufTmp = 0;
    else *(bufTmp + 1) = 0;
  }

  if (*inputBuf) res = strdup(inputBuf);
//  else res = strdup(_rlLastLine ? _rlLastLine : "");
  else res = strdup("");

  free(inputBuf);

  return res;
}

void add_history(char *value)
{
  if (_rlLastLine) free(_rlLastLine);
  if (value) _rlLastLine = strdup(value);
  else _rlLastLine = NULL;
}

void rlDone()
{
  if (_rlLastLine)
  {
    free(_rlLastLine);
    _rlLastLine = NULL;
  }
}

#endif

