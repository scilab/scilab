/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> 
#include <readline/readline.h>
#include <readline/history.h>
#include <setjmp.h>

#include "../machine.h" 

extern void C2F(zzledt)(char *buffer,int * buf_size,int * len_line,
			int * eof,long int  dummy1);


#define TRUE 1 
#define FALSE 0
static int fd=0;              /* file number for standard in */
static char Sci_Prompt[24];
static int  use_prompt=1;
static int hist = 1; /* flag to add to history */
/***********************************************************************
 * line editor
 **********************************************************************/


/* my_getc just try to get one interactive character typed 
 * by scilab user while dealing with gtk/tcltk events 
 * if a soft menu is activated current edited line is aborted and we 
 * jump to quit readline 
 */

static jmp_buf my_env;

static int my_getc (FILE *dummy) 
{ 
  int i= Xorgetchar();
  if ( C2F (ismenu) () == 1)
    { 
      /* abort current line aquisition*/
      longjmp(my_env,1);
      return 0;
    }
  return i;
}

int using_readline() { return 1;}

static void initialize_readline();

static char * dupstr (char *s)
{
  char *r;
  r = malloc (strlen (s) + 1);
  strcpy (r, s);
  return (r);
}


int get_one_char(char *prompt) {
  static char lp[24];
  char buffer[2];
  int buf_size=2, len_line, eof;
  rl_num_chars_to_read = 1;  
  hist = 0; /* not to be added to history */
  strcpy(lp,Sci_Prompt);
  strcpy(Sci_Prompt,prompt); 
  C2F(zzledt)(buffer,&buf_size,&len_line,&eof,2);
  strcpy(Sci_Prompt,lp);
  rl_num_chars_to_read = 0;
  hist = 1;
  return buffer[0];
}


extern void C2F(zzledt)(char *buffer,int * buf_size,int * len_line,
			int * eof,long int  dummy1)
{
  static int init_flag = TRUE;
  char * line ; 
  static int tty =0;

   if(init_flag) {
     initialize_readline();
     /* the next line is useful for cut and paste 
      * ehrlich juin 2001 
      */
     setvbuf(stdin, NULL, _IONBF, 0); 
     fd=fileno(stdin);
     tty = isatty(fileno(stdin));
     init_flag = FALSE;
   }

   set_is_reading(TRUE);

   if(!tty) {
     /* if not an interactive terminal 
      * use fgets 
      */ 
     fputs("-->",stdout);
     *eof = (fgets(buffer, *buf_size, stdin) == NULL);
     *len_line = strlen(buffer);
     /* remove newline character if there */
     if(buffer[*len_line - 1] == '\n')
       (*len_line)--;
     return;
   }

   rl_getc_function = my_getc;

   if ( setjmp(my_env)) {
     /** return from longjmp **/
     *eof = -1;
     use_prompt=0;
     return;
   } else {
     line = readline((use_prompt) ? Sci_Prompt : "" );
     use_prompt=1;
   }

   if (hist && line && *line != '\0') 
     add_history (line);

   if ( line == NULL) 
     {
       *len_line= 1;
       strncpy(buffer,"\n",1);
       *eof = FALSE;
     }
   else 
     {
       *len_line= strlen(line);
       strncpy(buffer,line,*buf_size);
       free(line);
       *eof = FALSE;
     }
   if(get_echo_mode()==0)  set_echo_mode(TRUE);
   set_is_reading(FALSE);
   return;
}

/*----------------------------------------------------------------------
 * initialise the io sequences
 *----------------------------------------------------------------------*/

void sci_get_screen_size (int *rows,int *cols)
{
  rl_get_screen_size(rows,cols);
}

/*----------------------------------------------------------------------
 * changing the prompt 
 *----------------------------------------------------------------------*/

void C2F(setprlev)(int *pause)
{
  if ( *pause == 0 ) 
    sprintf(Sci_Prompt,"-->");
  else if ( *pause > 0 )
    sprintf(Sci_Prompt,"-%d->",*pause);
  else
    sprintf(Sci_Prompt,">>");
}

     


/* **************************************************************** */
/*                                                                  */
/*                  Interface to Readline Completion                */
/*                                                                  */
/* **************************************************************** */

/* A structure which contains information on the commands this program
   can understand. */

typedef char *hchar ; 
hchar commands[] = {
#include "zzledt.txt"
   (char *)NULL,
};

static char *command_generator (const char *, int);
static char **scilab_completion (const char *, int, int);

/* Tell the GNU Readline library how to complete. */ 

static void initialize_readline ()
{
  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = scilab_completion;
}

static char **scilab_completion (const char * text,int start,int end)
{
  char **matches ; 
  matches = rl_completion_matches (text, command_generator);
  return (matches);
}

static char *command_generator (const char *text, int state)
{
  static int list_index, len;
  char *name;

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state)
    {
      list_index = 0;
      len = strlen (text);
    }
  /* Return the next name which partially matches from the command list. */
  while ( (name = commands[list_index]))
    {
      list_index++;
      if (strncmp (name, text, len) == 0)
        return (dupstr(name));
    }
  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}
