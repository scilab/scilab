/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */
/************************************
 * reading functions for scilab 
 * Scilab 1997
 *   Jean-Philippe Chancelier 
 ************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef STRICT
#define STRICT
#endif
/*#include <windows.h>
  #include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"
#include "plot.h"
/*-----------------------------------------------------------------------------------*/
char save_prompt[10];
/* Fonction Récuperant la ligne à executer par Scilab */

extern void InvalidateCursor( void ); 
extern char input_line[MAX_LINE_LEN + 1];
extern BOOL ScilabIsStarting;
extern void SetReadyOrNotForAnewLign(BOOL Ready);
extern void GetCurrentPrompt(char *CurrentPrompt);


char copycur_line[1024];
BOOL PutLineInBuffer=FALSE;
void ChangeCursorWhenScilabIsReady(void);
/*-----------------------------------------------------------------------------------*/
/***********************************************************************
 * line editor win32 version 
 * Input function for Scilab 
 * zzledt1 for scilab 
 * zzledt  for scilab -nw 
 **********************************************************************/

/**** Warning here : eof can be true  ***/
#ifdef __STDC__
void C2F (zzledt) (char *buffer, int *buf_size, int *len_line, int *eof, int* interrupt, int *modex, long int dummy1)
#else
void C2F (zzledt) (buffer, buf_size, len_line, eof, interrupt, modex, dummy1)
     char *buffer;
     int *buf_size;
     int *len_line;
     int *eof;
     int *interrupt;
     int *modex;
     long int dummy1;		/* added by FORTRAN to give buffer length */
#endif
{
  int i;
  extern BOOL PutLineInBuffer;
  extern char copycur_line[1024];

  GetCurrentPrompt(save_prompt);

  ChangeCursorWhenScilabIsReady();
  TextMessage();
  if (*modex) SetReadyOrNotForAnewLign(TRUE); /* Pret à recevoir depuis la thread Coller */
  set_is_reading (TRUE);
  if (PutLineInBuffer)
  {
	  SendCTRLandAKey(CTRLU);
	  write_scilab(copycur_line);
	  PutLineInBuffer=FALSE;
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
  *len_line = strlen (buffer);
  /** fprintf(stderr,"[%s,%d]\n",buffer,*len_line); **/
  *eof = (i == 1) ? TRUE : FALSE;
  set_is_reading (FALSE);
  TextMessage();


  return;
}
/*-----------------------------------------------------------------------------------*/
void ChangeCursorWhenScilabIsReady(void)
{
  if ( (IsWindowInterface()) && (ScilabIsStarting) )
  {
	extern char ScilexWindowName[MAX_PATH];
	HCURSOR hCursor;
	LPTW lptw;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
	hCursor=LoadCursor(  lptw->hInstance,IDC_ARROW);
	SetClassLong(lptw->hWndParent, GCL_HCURSOR,	(LONG) hCursor); 
	SetClassLong(lptw->hWndText,GCL_HCURSOR,(LONG) hCursor); 
	InvalidateCursor(); 
	ScilabIsStarting=FALSE;
  }

}
/*-----------------------------------------------------------------------------------*/
void SaveCurrentLine(BOOL RewriteLineAtPrompt)
{
	extern char cur_line[1024];

	if ( (get_is_reading ()) && (PutLineInBuffer == FALSE) )
	{
		strcpy(copycur_line,cur_line);
		if (RewriteLineAtPrompt) PutLineInBuffer=TRUE;
	}

}
/*-----------------------------------------------------------------------------------*/
