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
#include "All-extern.h"

extern void C2F(zzledt)(char *buffer,int * buf_size,int * len_line,
			int * eof, int *menusflags, int * modex,long int  dummy1);
#define TRUE 1 
#define FALSE 0
static int fd=0;              /* file number for standard in */
static char Sci_Prompt[24];
static int  use_prompt=1;
static int hist = 1; /* flag to add to history */
static int interrupt=1; 


int NumberOfCommands=0; /* for compatibility with console/dynamic_menus.c */

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
  int i= Xorgetchar(interrupt);
  if (interrupt&&( C2F (ismenu) () == 1))
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


int get_one_char(char *prompt) 
{
  static char lp[24];
  char buffer[2];
  int buf_size=2, len_line, eof,inter=0,modex=0;
  rl_num_chars_to_read = 1;  
  hist = 0; /* not to be added to history */
  strcpy(lp,Sci_Prompt);
  strcpy(Sci_Prompt,prompt); 
  C2F(zzledt)(buffer,&buf_size,&len_line,&eof,&inter,&modex,2);
  strcpy(Sci_Prompt,lp);
  rl_num_chars_to_read = 0;
  hist = 1;
  return buffer[0];
}


void C2F(zzledt)(char *buffer,int * buf_size,int * len_line,
		 int * eof, int *inter_flag, int * modex,long int  dummy1)
{
  static char saved_buf[512];/* use to save data when zzledt is interupted 
				rl_stuff is limited to 512 */
  static int saved_buf_flag = 0;
  static int init_flag = TRUE;
  char * line ; 
  static int tty =0;
  interrupt=*inter_flag;

   if(init_flag) {
     initialize_readline();
     /* the next line is useful for cut and paste 
      * ehrlich juin 2001 
      */
     setvbuf(stdin, NULL, _IONBF, 0); 
     fd=fileno(stdin);
     tty = isatty(fileno(stdin));
     init_flag = FALSE;
#if defined(__CYGWIN32__)
     tty=1 ; /* FIXME cygwin */
#endif 
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
     use_prompt=1;
     buffer[0]=0;
     strncpy(saved_buf,rl_line_buffer,512);
     saved_buf_flag=1;
     sciprint("\r");  /* this is a hack and fixes the redraw of short lines upon
                       tk events (bug #593, #1363)*/
     return;
   } else {
     int i; 
     if ( saved_buf_flag == 1 ) 
       {
	 for ( i = 0 ; i < strlen(saved_buf); i++) 
	   rl_stuff_char(saved_buf[i]);
	 saved_buf_flag=0;
       }
     line=readline((use_prompt) ? Sci_Prompt : "" );
     use_prompt=1;
   }

   if (hist && line && *line != '\0' ) 
     add_history (line);

   if ( line == NULL) 
     {
       *len_line= 0;
       buffer[0]=buffer[1]='\0';
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
hchar rlcommands[] = {
#include "zzledt.txt"
   (char *)NULL,
};


static char * void_command_generator ( const char *text,  int state)
{
  return ((char *)NULL);
}

static char *command_generator (const char *, int);
static char **scilab_completion (const char *, int, int);

/* Tell the GNU Readline library how to complete. */ 

static void initialize_readline ()
{
  /* Tell the completer that we want a crack first. */
  /* I remove the completion since it troubles the cut and paste 
   * of data with tabs jpc 2003 
   */
  rl_attempted_completion_function = scilab_completion;
  rl_completion_entry_function = void_command_generator;
}

static char **scilab_completion (const char * text,int start,int end)
{
  char **matches = NULL ; 

  if (end != 0)
    {
      matches = rl_completion_matches (text, command_generator);
      if (matches == NULL) 
	matches = rl_completion_matches (text, rl_filename_completion_function );
    }
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
  while ( (name = rlcommands[list_index]))
    {
      list_index++;
      if (strncmp (name, text, len) == 0)
        return (dupstr(name));
    }
  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

/*----------------------------------------------------------------------
 * clear the screen / move to home 
 *----------------------------------------------------------------------*/

void HomeFunction(void)
{
  sciprint("Not yet implemented. \r\n");
}

void ClearScreenConsole (char *fname)
{
  rl_clear_screen(0,0);
  /*sciprint("Not yet implemented. \r\n");*/
}


