/* autocmp.c - execute command if a specific compilation option is available */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auto.h"

int run(int argc, char **argv)
{
  unsigned int argNum;
  char *cmdline;
  unsigned long int cmdLen;
  int rc;

  cmdLen = 0;
  for (argNum = 2; argNum < argc; argNum++)
  {
    cmdLen += strlen(argv[argNum]) + 1;
  }

  cmdline = malloc(cmdLen + 1);
  *cmdline = 0;

  for (argNum = 3; argNum < argc; argNum++)
  {
    strcat(cmdline, argv[argNum]);
    if ((argNum + 1) < argc) strcat(cmdline, " ");
  }

  printf("Executing '%s'\n", cmdline);
  rc = system(cmdline);
  free(cmdline);

  return rc;
}

int main (int argc, char **argv)
{
  if (argc < 3)
  {
    printf("Usage: autocmp <option> <0|1> <command>\n");

    return 100;
  }

  if (!strcmp(argv[1], "rl"))
  {
#ifdef HAVE_READLINE
    if (*argv[2] == '1') return run(argc, argv);
#else
    if (*argv[2] == '0') return run(argc, argv);
#endif
  }
  else
  {
    printf("Unknown option: '%s'\n", argv[1]);
    return 101;
  }

  return 0;
}
