/* lwpp.c: A lightweight text preprocessor.
 * created 1999-Mar-12 16:42 jmk
 * autodate: 1999-Aug-15 18:46
 * 
 * by Jim Knoble <http://www.pobox.com/~jmknoble/>
 * Copyright 1999 Jim Knoble
 *
 * Adapted for use with HuskyPnt by Sascha Silbe <Sascha.Silbe@ldknet.org>
 * 
 * Disclaimer:
 * 
 * The software is provided "as is", without warranty of any kind,
 * express or implied, including but not limited to the warranties of
 * merchantability, fitness for a particular purpose and
 * noninfringement. In no event shall the author(s) be liable for any
 * claim, damages or other liability, whether in an action of
 * contract, tort or otherwise, arising from, out of or in connection
 * with the software or the use or other dealings in the software.
 */

#define VERSION "1.0hp"
#define DATE "2000-Apr-22"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN		1024

#define KEYWORD_NONE		0
#define KEYWORD_DEFINE		1
#define KEYWORD_UNDEF		2
#define KEYWORD_IFDEF		3
#define KEYWORD_ELSE		4
#define KEYWORD_ENDIF		5
#define KEYWORD_IFNDEF		6
#define KEYWORD_IFEQ		7
#define KEYWORD_IFNEQ		8

#define STATUS_OK		1
#define STATUS_UNKNOWN_KEYWORD	0
#define STATUS_DEFINE_ERROR	-1
#define STATUS_UNDEF_ERROR	-2
#define STATUS_MISSING_TOKEN	-3
#define STATUS_MISSING_VALUE	-4
#define STATUS_ELSE_WITHOUT_IF	-5
#define STATUS_ENDIF_WITHOUT_IF	-6
#define STATUS_MISSING_ENDIF	-7
#define STATUS_STACK_UNDERFLOW	-10

typedef struct KeywordStruct 
{
   char *token;
   int id;
   char *text;
   char *help_text;
} Keyword;

static Keyword keyword_table[] = 
{
   { "define",	KEYWORD_DEFINE, "=",
"    define <token> [<value>]  Define <token> to have value <value>.\n"
"                              If <value> is omitted, defaults to `1'.\n"
"\n"
   },
   { "undef",	KEYWORD_UNDEF, NULL,
"    undef <token>             Undefine <token>.\n"
"\n"
   },	 
   { "ifdef",	KEYWORD_IFDEF, "defined",
"    ifdef <token>             If <token> is defined, print the following\n"
"                              lines on the output, until the next `else'\n"
"                              or `endif' directive.\n"
"\n"
   },
   { "else",	KEYWORD_ELSE, NULL,
"    else                      If the prior `if' directive evaluated to\n"
"                              false, print the following lines on the\n"
"                              output.\n"
"\n"
   },
   { "endif",	KEYWORD_ENDIF, NULL,
"    endif                     Conclude the preceding `if ... [else ...]'.\n"
"\n"
   },
   { "ifndef",	KEYWORD_IFNDEF, "!defined",
"    ifndef <token>            If <token> is *not* defined, print the\n"
"                              following lines on the output, until the\n"
"                              next `else' or `endif' directive.\n"
"\n"
   },
   { "ifeq",	KEYWORD_IFEQ, "==",
"    ifeq <token> <value>      If <token> is lexically equal to <value>,\n"
"                              print the following lines on the output,\n"
"                              until the next `else' or `endif' directive.\n"
"\n"
   },
   { "ifneq",	KEYWORD_IFNEQ, "!=",
"    ifneq <token> <value>     If <token> is *not* lexically equal to\n"
"                              <value>, print the following lines on the\n"
"                              output, until the next `else' or `endif'\n"
"                              directive.\n"
"\n"
   },
   { NULL,	KEYWORD_NONE, NULL, NULL }
};

typedef struct FileStateStruct
{
   int in_if;
   int in_else;
   int condition;
   int parent_condition;
   char *text;
} FileState;

#define STACK_SIZE_INCREMENT	64
typedef struct StackStateStruct
{
   FileState *bottom;
   int size;
   int count;
} StackState;

#define DEF_PREFIX	"@"
#define DEF_VALUE	"1"
static char *prefix = NULL;
static int prefix_len = 0;

char *prog_name = NULL;
int verbose = 0;

char version_info[] = ""
"lwpp version " VERSION " (" DATE ")\n"
"by Jim Knoble <http://www.pobox.com/~jmknoble/>\n"
"Provided as is with no warranty of any kind, etc., etc.\n"
;

char usage_info[] =
"\n"
"Usage: %s [-p <text>] [-D<token>[=<val>]] [-U<token>] [-v] [<filename>]\n"
"\n"
"Preprocess <filename>, sending the result to the standard output.\n"
"If <filename> is not specified, read from the standard input.\n"
"\n"
"Options:\n"
"    -p<text>                  Preprocessor directives are prefixed with\n"
"    --prefix <text>           <text>.  Defaults to `" DEF_PREFIX "'.\n"
"\n"
"    -D<token>[=<val>]         Define <token> to have value <val>.  If\n"
"    --define <token>[=<val>]  <val> is omitted, defaults to `" DEF_VALUE "'.\n"
"\n"
"    -U<token>                 Undefine <token>.\n"
"    --undef <token>\n"
"\n"
"    -v                        Be verbose.\n"
"    --verbose\n"
"\n"
"    -V                        Show version information.\n"
"    --version\n"
"\n"
"Preprocessing Directives:\n"
;

/**********************************************************************/
void out_of_memory(int line_num)
{
   fprintf(stderr, "%s:line %d: Out of memory\n", prog_name, line_num);
   exit(1);
}

void init_stack(StackState *stack)
{
   stack->bottom = NULL;
   stack->size = 0;
   stack->count = 0;
}

void increase_stack_size(StackState *stack)
{
   stack->size += STACK_SIZE_INCREMENT;
   stack->bottom = realloc(stack->bottom, stack->size * sizeof(FileState));
   if (NULL == stack->bottom)
    {
       out_of_memory(__LINE__);
    }
}

void init_file_state(FileState *state)
{
   state->in_if = 0;
   state->in_else = 0;
   state->condition = 1;
   state->parent_condition = 1;
   state->text = NULL;
}

void copy_file_state(FileState *dst, FileState *src)
{
   dst->in_if = src->in_if;
   dst->in_else = src->in_else;
   dst->condition = src->condition;
   dst->parent_condition = src->parent_condition;
   dst->text = src->text;
}

void push_file_state(StackState *stack, FileState *state)
{
   FileState *top;

   if (stack->count == stack->size)
    {
       increase_stack_size(stack);
    }
   top = (stack->bottom + stack->count);
   (stack->count)++;
   copy_file_state(top, state);
}

int pop_file_state(StackState *stack, FileState *state)
{
   int status = STATUS_STACK_UNDERFLOW;
   FileState *top;
   
   if (0 < stack->count)
    {
       status = STATUS_OK;
       (stack->count)--;
       top = (stack->bottom + stack->count);
       copy_file_state(state, top);
    }
   return(status);
}

Keyword *find_keyword(char *token)
{
   Keyword *kw = keyword_table;

   while (NULL != kw->token)
    {
       if (0 == strcmp(token, kw->token))
	{
	   break;
	}
       kw++;
    }
   return(kw);
}

char *lookup_definition(char *token)
{
   char *value = NULL;
   
   if (NULL != token)
    {
       value = getenv(token);
    }
   return(value);
}

#ifndef USE_PUTENV
int create_definition(char *token, char *value)
{
   int status = 0;
   
   if ((NULL != token) && (0 < strlen(token)) && (NULL != value))
    {
       status = (0 == setenv(token, value, 1));
    }
   return(status);
}

int remove_definition(char *token)
{
   unsetenv(token);
   return(1);
}
#else /* USE_PUTENV */
int create_definition(char *token, char *value)
{
   int status = 0;
   char *definition;
   int n;
   
   if (NULL != token)
    {
       n = strlen(token);
       if (0 < n)
	{
	   if (NULL == value)
	    {
	       status = (0 == putenv(token));
	    }
	   else
	    {
	       n += strlen(value);
	       definition = malloc(n + 2);
	       if (NULL == definition)
		{
		   out_of_memory(__LINE__);
		}
	       strcpy(definition, token);
	       strcat(definition, "=");
	       if (NULL != value)
		{
		   strcat(definition, value);
		}
	       status = (0 == putenv(definition));
	       free(definition);
	    }
	}
    }
   return(status);
}

int remove_definition(char *token)
{
   int status = create_definition(token, NULL);
   return(status);
}
#endif /* !USE_PUTENV */

int is_defined(char *token)
{
   int status = 0;
   char *value;
   
   if (NULL != token)
    {
      value = getenv(token);
      return ((value != NULL) && (strlen(value) > 0));
    }
   return(status);
}

int is_prefixed_line(char *line)
{
   int status = 0;
   
   if (NULL != line)
    {
       status = (0 == strncmp(line, prefix, prefix_len));
    }
   return(status);
}

char *skip_white(char *line)
{
   char *c = line;
   
   if (NULL != c)
    {
       while (isspace(*c))
	{
	   c++;
	}
    }
   return(c);
}

char *skip_prefix(char *line)
{
   char *new_line = line;
   int i;
   
   for (i = 0; i < prefix_len; i++)
    {
       if ('\0' == *new_line)
	{
	   break;
	}
       new_line++;
    }
   new_line = skip_white(new_line);
      
   return(new_line);
}

char *skip_quoted_string(char **line, int *n)
{
   char *c = *line;
   char quote;
   
   if (NULL != c)
    {
       quote = *c;
       (*line)++;
       c++;
       while (('\0' != *c) && (quote != *c))
	{
	   (*n)++;
	   c++;
	}
    }
   return(c);
}

char *extract_next_token(char **line)
{
   int n = 0;
   char *c = *line;
   char *token = NULL;
   
   if (NULL != c)
    {
       if (('"' == *c) || ('\'' == *c))
	{
	   c = skip_quoted_string(line, &n);
	}
       else
	{
	   while (('\0' != *c) && !isspace(*c))
	    {
	       n++;
	       c++;
	    }
	}
       if (0 < n)
	{
	   token = malloc(n + 1);
	   if (NULL == token)
	    {
	       out_of_memory(__LINE__);
	    }
	   strncpy(token, *line, n);
	   token[n] = '\0';
	   if (('"' == *c) || ('\'' == *c))
	    {
	       c++;
	    }
	   *line = skip_white(c);
	}
    }
   return(token);
}

void print_warning(int status, char *filename, char *keyword, char *token,
		   int line_num)
{
   char *msg;
   int need_keyword = 0;
   int need_token = 0;
   
   switch (status)
    {
     case STATUS_OK:
       msg = "Oops ... no error.  This shouldn't happen.";
       break;
     case STATUS_UNKNOWN_KEYWORD:
       msg = "Unknown keyword";
       need_token = 1;
       break;
     case STATUS_DEFINE_ERROR:
       msg = "Error defining token";
       need_token = 1;
       break;
     case STATUS_UNDEF_ERROR:
       msg = "Error undefining token";
       need_token = 1;
       break;
     case STATUS_MISSING_TOKEN:
       msg = "Missing token for keyword";
       need_keyword = 1;
       break;
     case STATUS_MISSING_VALUE:
       msg = "Missing comparison value for expression";
       need_keyword = 1;
       break;
     case STATUS_ELSE_WITHOUT_IF:
       msg = "else without if";
       break;
     case STATUS_ENDIF_WITHOUT_IF:
       msg = "endif without if";
       break;
     case STATUS_MISSING_ENDIF:
       msg = "missing endif";
       need_keyword = 1;
       break;
     case STATUS_STACK_UNDERFLOW:
       msg = "File state stack underflow";
       break;
     default:
       msg = "Unknown error";
       break;
    }
   if (need_keyword && need_token)
    {
       fprintf(stderr, "%s:%d: %s: `%s %s'\n", filename, line_num, msg,
	       keyword, token);
    }
   else if (need_keyword)
    {
       fprintf(stderr, "%s:%d: %s: `%s'\n", filename, line_num, msg, keyword);
    }
   else if (need_token)
    {
       fprintf(stderr, "%s:%d: %s: `%s'\n", filename, line_num, msg, token);
    }
   else
    {
       fprintf(stderr, "%s:%d: %s\n", filename, line_num, msg);
    }
}

int process_line(char *infilename, FILE *outfile,
		 StackState *stack, FileState *state,
		 char *buf, int line_num)
{
   int status = STATUS_OK;
   char *line = buf;
   char *keyword = NULL;
   Keyword *kw = NULL;
   char *token = NULL;
   char *value = NULL;
   char *default_value = DEF_VALUE;
   char *txt = NULL;
   
   if (!is_prefixed_line(line))
    {
       if ((state->condition) && (state->parent_condition))
	{
	   fputs(line, outfile);
	}
    }
   else
    {
       line = skip_prefix(line);
       keyword = extract_next_token(&line);
       kw = find_keyword(keyword);
       switch (kw->id)
	{
	 case KEYWORD_DEFINE:
	   token = extract_next_token(&line);
	   value = extract_next_token(&line);
	   if (NULL == value)
	    {
	       status = create_definition(token, default_value);
	       txt = default_value;
	    }
	   else
	    {
	       status = create_definition(token, value);
	       txt = value;
	    }
	   if (verbose)
	    {
	       fprintf(stderr, "%s:%d: define (\"%s\" %s \"%s\")\n",
		       infilename, line_num, token, kw->text, txt);
	    }
	   if (!status)
	    {
	       status = STATUS_DEFINE_ERROR;
	    }
	   break;
	 case KEYWORD_UNDEF:
	   token = extract_next_token(&line);
	   status = remove_definition(token);
	   if (verbose)
	    {
	       fprintf(stderr, "%s:%d: undef  (\"%s\")\n",
		       infilename, line_num, token);
	    }
	   if (!status)
	    {
	       status = STATUS_UNDEF_ERROR;
	    }
	   break;
	 case KEYWORD_IFDEF:
	   /* Fall through */
	 case KEYWORD_IFNDEF:
	   token = extract_next_token(&line);
	   if (NULL == token)
	    {
	       status = STATUS_MISSING_TOKEN;
	    }
	   else
	    {
	       int n;
	       
	       push_file_state(stack, state);
	       state->in_if = 1;
	       state->in_else = 0;
	       state->parent_condition = state->condition;
	       if (state->parent_condition)
		{
		   if (KEYWORD_IFDEF == kw->id)
		    {
		       state->condition = is_defined(token);
		    }
		   else
		    {
		       state->condition = !is_defined(token);
		    }
		}
	       n = strlen(kw->text) + 1 + strlen(token) + 2 + 1;
	       state->text = malloc(n);
	       if (NULL == state->text)
		{
		   out_of_memory(__LINE__);
		}
	       sprintf(state->text, "%s \"%s\"", kw->text, token);
	       if (verbose)
		{
		   fprintf(stderr, "%s:%d: if     (%s): %s\n",
			   infilename, line_num, state->text,
			   (state->condition ? "true" : "false"));
		}
	    }
	   break;
	 case KEYWORD_ELSE:
	   if (!state->in_if)
	    {
	       status = STATUS_ELSE_WITHOUT_IF;
	    }
	   else
	    {
	       state->in_if = 0;
	       state->in_else = 1;
	       state->condition = !(state->condition);
	       if (verbose)
		{
		   fprintf(stderr, "%s:%d: else   (%s): %s\n",
			   infilename, line_num, state->text,
			   (state->condition ? "true" : "false"));
		}
	    }
	   break;
	 case KEYWORD_ENDIF:
	   if (!(state->in_if) && !(state->in_else))
	    {
	       status = STATUS_ENDIF_WITHOUT_IF;
	    }
	   else
	    {
	       if (verbose)
		{
		   fprintf(stderr, "%s:%d: endif  (%s)\n",
			   infilename, line_num, state->text);
		}
	       if (NULL != state->text)
		{
		   free(state->text);
		}
	       status = pop_file_state(stack, state);
	    }
	   break;
	 case KEYWORD_IFEQ:
	   /* Fall through */
	 case KEYWORD_IFNEQ:
	   token = extract_next_token(&line);
	   if (NULL == token)
	    {
	       status = STATUS_MISSING_TOKEN;
	    }
	   else
	    {
	       value = extract_next_token(&line);
	       if (NULL == value)
		{
		   status = STATUS_MISSING_VALUE;
		}
	       else
		{
		   int n;
		   
		   push_file_state(stack, state);
		   state->in_if = 1;
		   state->in_else = 0;
		   state->parent_condition = state->condition;
		   if (state->parent_condition) 
		    {
		       char *def = lookup_definition(token);
		       if (KEYWORD_IFEQ == kw->id)
			{
			   state->condition = ((NULL != def) &&
					       (0 == strcmp(def, value)));
			}
		       else
			{
			   state->condition = ((NULL != def) &&
					       (0 != strcmp(def, value)));
			}
		    }
		   n = strlen(token) + 2 + 1 + strlen(kw->text) + 1 +
		      strlen(value) + 2 + 1;
		   state->text = malloc(n);
		   if (NULL == state->text)
		    {
		       out_of_memory(__LINE__);
		    }
		   sprintf(state->text, "\"%s\" %s \"%s\"",
			   token, kw->text, value);
		   if (verbose)
		    {
		       fprintf(stderr, "%s:%d: if     (%s): %s\n",
			       infilename, line_num, state->text,
			       (state->condition ? "true" : "false"));
		    }
		}
	    }
	   break;
	 default:
	   status = STATUS_UNKNOWN_KEYWORD;
	   break;
	}
       if (status < STATUS_OK)
	{
	   print_warning(status, infilename, keyword, token, line_num);
	}
       if (NULL != keyword)
	{
	   free(keyword);
	}
       if (NULL != token)
	{
	   free(token);
	}
       if (NULL != value)
	{
	   free(value);
	}
    }
   return(status);
}

int process_input_file(char *infilename, char *outfilename)
{
   int status = 1;
   FILE *infile = stdin;
   FILE *outfile = stdout;
   int using_stdin = 1;
   int using_stdout = 1;
   char *munged_infilename = "stdin";
   char *munged_outfilename = "stdout";
   char buffer[MAX_LINE_LEN];
   char *line;
   int line_num = 0;
   StackState stack;
   FileState state;

   if (0 != strcmp(infilename, "-"))
    {
       using_stdin = 0;
       munged_infilename = infilename;
       infile = fopen(infilename, "r");
       if (NULL == infile)
	{
	   perror(infilename);
	   return(0);
	}
    }
   if (0 != strcmp(outfilename, "-"))
    {
       using_stdout = 0;
       munged_outfilename = outfilename;
       outfile = fopen(outfilename, "w");
       if (NULL == outfile)
	{
	   perror(outfilename);
	   if (!using_stdin)
	    {
	       fclose(infile);
	    }
	   return(0);
	}
    }
   if (verbose)
    {
       fprintf(stderr, "Processing %s ...\n", munged_infilename);
    }
   
   init_stack(&stack);
   init_file_state(&state);
   while(NULL != fgets(buffer, sizeof(buffer), infile))
    {
       line = buffer;
       line_num++;
       status = process_line(munged_infilename, outfile,
			     &stack, &state, line, line_num);
    }
   if (stack.count > 0)
    {
       print_warning(STATUS_MISSING_ENDIF, munged_infilename, state.text,
		     NULL, line_num);
    }
   putc('\n', outfile);
   if (!using_stdin)
    {
       fclose(infile);
    }
   if (!using_stdout)
    {
       fclose(outfile);
    }
   return(status);
}

void usage(void)
{
   Keyword *kw;
   
   fprintf(stderr, usage_info, prog_name);
   kw = keyword_table;
   while (NULL != kw->token)
    {
       fprintf(stderr, kw->help_text);
       kw++;
    }
   exit(1);
}

void show_version(void)
{
   fprintf(stderr, version_info);
}

int process_args(int argc, char **argv)
{
   int i;
   char *t;
   char *v;
   
   for (i = 1; i < argc; i++)
    {
       if ('-' != argv[i][0])
	{
	   break;
	}
       else if ((0 == strcmp(argv[i], "-h")) ||
		(0 == strcmp(argv[i], "--help")))
	{
	   show_version();
	   usage();
	}
       else if ((0 == strncmp(argv[i], "-p", 2)) ||
		(0 == strcmp(argv[i], "--prefix")))
	{
	   if (2 <= strlen(argv[i]))
	    {
	       t = (argv[i] + 2);
	    }
	   else if (i >= argc)
	    {
	       break;
	    }
	   else
	    {
	       t = argv[++i];
	    }
	   prefix = t;
	}
       else if ((0 == strcmp(argv[i], "-v")) ||
		(0 == strcmp(argv[i], "--verbose")))
	{
	   verbose = 1;
	}
       else if ((0 == strcmp(argv[i], "-V")) ||
		(0 == strcmp(argv[i], "--version")))
	{
	   show_version();
	   exit(1);
	}
       else if ((0 == strncmp(argv[i], "-D", 2)) ||
		(0 == strcmp(argv[i], "--define")))
	{
	   if (2 <= strlen(argv[i]))
	    {
	       t = (argv[i] + 2);
	    }
	   else if (i >= argc)
	    {
	       break;
	    }
	   else
	    {
	       t = argv[++i];
	    }
	   v = strchr(t, '=');
	   if (NULL == v)
	    {
	       create_definition(t, DEF_VALUE);
	    }
	   else
	    {
	       *(v++) = '\0';
	       create_definition(t, v);
	    }
	   if (verbose)
	    {
	       fprintf(stderr, "Defined token `%s' with value `%s'\n",
		       t, lookup_definition(t));
	    }
	}
       else if ((0 == strncmp(argv[i], "-U", 2)) ||
		(0 == strcmp(argv[i], "--undef")))
	{
	   if (2 <= strlen(argv[i]))
	    {
	       t = (argv[i] + 2);
	    }
	   else if (i >= argc)
	    {
	       break;
	    }
	   else
	    {
	       t = argv[++i];
	    }
	   remove_definition(t);
	   if (verbose)
	    {
	       fprintf(stderr, "Undefined token `%s'\n", t);
	    }
	}
       else if (0 == strcmp(argv[i], "-"))
	{
	   /* '-' is a filename meaning stdin;
	    * leave it in the list of files to process.
	    */
	   break;
	}
       else if (0 == strcmp(argv[i], "--"))
	{
	   /* End of options. */
	   i++;
	   break;
	}
       else
	{
	   fprintf(stderr, "%s: Unknown option `%s'\n", prog_name, argv[i]);
	   usage();
	}
    }
   return(i);
}

int main(int argc, char **argv)
{
   int i;
   int status;
   char *outfilename = "-";
   
   prog_name = argv[0];
   
   i = process_args(argc, argv);

   if (NULL == prefix)
    {
       prefix = DEF_PREFIX;
    }
   prefix_len = strlen(prefix);

   if (i >= argc)
    {
       /* No files specified on command line; use stdin. */
       status = process_input_file("-", outfilename);
    }
   else
    {
       status = process_input_file(argv[i], outfilename);
    }

   return(status ? 0 : 1);
}

