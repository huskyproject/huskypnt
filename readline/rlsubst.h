/* rlsubst.h - substitute functions for readline */

#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#define rlDone()
#else

char *readline(char *prompt);
void add_history(char *value);
void rlDone();

#endif

