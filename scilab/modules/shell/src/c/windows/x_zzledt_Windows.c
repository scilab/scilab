/************************************
 * reading functions for scilab 
 ************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h"
#include "command.h"
#include "../../core/src/c/flags.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "scilabmode.h"
#include "ShellRead.h"
/*-----------------------------------------------------------------------------------*/
char save_prompt[10];
/* Fonction Récuperant la ligne à executer par Scilab */
extern char input_line[MAX_LINE_LEN + 1];
extern void GetCurrentPrompt(char *CurrentPrompt);

extern int C2F (scilines) (int *nl, int *nc);
extern void set_is_reading(int);
/*-----------------------------------------------------------------------------------*/
void C2F (zzledt) (char *buffer, int *buf_size, int *len_line, int *eof, int* interrupt, int *modex, long int dummy1)
{
  int i = 0;
  char *line = NULL;

  GetCurrentPrompt(save_prompt);

  set_is_reading (TRUE);
  if (getScilabMode() == SCILAB_STD)
  {
	  i = 0;
	  line = ShellRead();
	  if (line)
	  {
		  strcpy(input_line,line);
		  FREE(line);
		  *len_line = (int)strlen(input_line);
	  }
  }
  else
  {
	i = read_line (save_prompt,*interrupt);
  }

  if (i==-1) 
  { 
	/* dynamic menu canceled read SS*/
    *len_line = 0;
    *eof = -1;
    return;
  }
  strncpy (buffer, input_line, *buf_size);
  *len_line = (int)strlen (buffer);

  *eof = (i == 1) ? TRUE : FALSE;
  set_is_reading (FALSE);
  return;
}
/*-----------------------------------------------------------------------------------*/
