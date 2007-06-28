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
/*-----------------------------------------------------------------------------------*/
char save_prompt[10];
/* Fonction Récuperant la ligne à executer par Scilab */

extern void InvalidateCursor( void ); 
extern char input_line[MAX_LINE_LEN + 1];

extern void SetReadyOrNotForAnewLign(BOOL Ready);
extern void GetCurrentPrompt(char *CurrentPrompt);

extern int GetSaveHistoryAfterNcommands(void);
extern char * getfilenamehistory(void);
extern void write_history(char *filename);
extern void SendCTRLandAKey(int code);
extern BOOL IsWindowInterface(void);
extern int C2F (scilines) (int *nl, int *nc);
extern int C2F(sxevents)();
extern void set_is_reading(int);

BOOL CanPutLineInBuffer(void);
void DisablePutLineInBuffer(void);
/*-----------------------------------------------------------------------------------*/
char copycur_line[1024];
BOOL PutLineInBuffer=FALSE;
void ChangeCursorWhenScilabIsReady(void);
extern void CleanCurrentLine(char *line);
/*-----------------------------------------------------------------------------------*/
/***********************************************************************
 * line editor win32 version 
 * Input function for Scilab 
 * zzledt  for scilab 
 **********************************************************************/

/**** Warning here : eof can be true  ***/
void C2F (zzledt) (char *buffer, int *buf_size, int *len_line, int *eof, int* interrupt, int *modex, long int dummy1)
{
  int i;
  extern char copycur_line[1024];

  GetCurrentPrompt(save_prompt);

  ChangeCursorWhenScilabIsReady();
  C2F (sxevents) ();
  if (*modex) SetReadyOrNotForAnewLign(TRUE); /* Pret à recevoir depuis la thread Coller */
  set_is_reading (TRUE);
  if (CanPutLineInBuffer())
  {
	  CleanCurrentLine(copycur_line);
	  DisablePutLineInBuffer();
  }
  
  i = read_line (save_prompt,*interrupt);
  
  if (i==-1) 
  { /* dynamic menu canceled read SS*/
    *len_line = 0;
    *eof = -1;
    //set_is_reading (FALSE);
    return;
  }
  strncpy (buffer, input_line, *buf_size);
  *len_line = (int)strlen (buffer);
  /** fprintf(stderr,"[%s,%d]\n",buffer,*len_line); **/
  *eof = (i == 1) ? TRUE : FALSE;
  set_is_reading (FALSE);
  C2F (sxevents) ();

  /* see savehistory */
  addNumberOfCommands();
  if ( ( GetSaveHistoryAfterNcommands() == getNumberOfCommands() ) && ( GetSaveHistoryAfterNcommands() > 0) )
  {
	  char *filenamehistory=NULL;
	
	  filenamehistory=getfilenamehistory();
	  write_history( filenamehistory );
	  FREE(filenamehistory);

	  resetNumberOfCommands();
  }

  return;
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
void SaveCurrentLine(BOOL RewriteLineAtPrompt)
{
	extern char cur_line[1024];

	if ( (get_is_reading ()) && (CanPutLineInBuffer() == FALSE) )
	{
		strcpy(copycur_line,cur_line);
		if (RewriteLineAtPrompt) DisablePutLineInBuffer();
	}
}
BOOL CanPutLineInBuffer(void)
{
	return PutLineInBuffer;
}
/*-----------------------------------------------------------------------------------*/
void EnablePutLineInBuffer(void)
{
	PutLineInBuffer = TRUE;
}
/*-----------------------------------------------------------------------------------*/
void DisablePutLineInBuffer(void)
{
	PutLineInBuffer = FALSE;
}
/*-----------------------------------------------------------------------------------*/
char *getCopyCurrentLine(void)
{
	char *copy_current_line = MALLOC(sizeof(char)*(strlen(copycur_line)+1));
	strcpy(copy_current_line,copycur_line);
	return copy_current_line;
}
/*-----------------------------------------------------------------------------------*/
