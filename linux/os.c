/* platform-specific functions, linux version */
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

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

