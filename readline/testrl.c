/* testrl.c - test if GNU readline is available */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
  char *temp, *prompt;
  FILE *f;

  f = fopen("auto.h", "a");
  if (f)
  {
    // tell the substitution functions that we have GNU readline
    fprintf(f, "#define HAVE_READLINE\n");
    fclose(f);
  }
  else if (f)
  {
    // never executed, just to test if they are available
    prompt = "readline$ ";
    temp = readline (prompt);
    if (temp)
    {
      add_history (temp);
      free(temp);
    }
  }

  return 0;
}
