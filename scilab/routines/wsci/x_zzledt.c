/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */
/* INRIA 2003 Allan CORNET */

/************************************
 * reading functions for scilab 
 ************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef STRICT
#define STRICT
#endif
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#include "wcommon.h"
#include "wtext.h"
/*-----------------------------------------------------------------------------------*/
char save_prompt[10];
/* Fonction Récuperant la ligne à executer par Scilab */

extern void InvalidateCursor( void ); 
extern char input_line[MAX_LINE_LEN + 1];
extern BOOL ScilabIsStarting;
extern void SetReadyOrNotForAnewLign(BOOL Ready);
extern void GetCurrentPrompt(char *CurrentPrompt);

extern int GetSaveHistoryAfterNcommands(void);
extern char * getfilenamehistory(void);
extern void write_history(char *filename);
extern LPTW GetTextWinScilab(void);
extern void SendCTRLandAKey(int code);
extern BOOL IsWindowInterface(void);
extern int C2F (scilines) (int *nl, int *nc);
/*-----------------------------------------------------------------------------------*/
char copycur_line[1024];
BOOL PutLineInBuffer=FALSE;
void ChangeCursorWhenScilabIsReady(void);
int NumberOfCommands=0;
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
  C2F (sxevents) ();
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
  C2F (sxevents) ();

  /* see savehistory */
  NumberOfCommands++;
  if ( ( GetSaveHistoryAfterNcommands() == NumberOfCommands ) && ( GetSaveHistoryAfterNcommands() > 0) )
  {
	  char *filenamehistory=NULL;
	
	  filenamehistory=getfilenamehistory();
	  write_history( filenamehistory );
	  FREE(filenamehistory);

	  NumberOfCommands=0;
  }

  return;
}
/*-----------------------------------------------------------------------------------*/
void ChangeCursorWhenScilabIsReady(void)
{
  if ( (IsWindowInterface()) && (ScilabIsStarting) )
  {
	HCURSOR hCursor;
	LPTW lptw=GetTextWinScilab();
	int NumsMenu = 0;
	int nl=0, nc=0;
	HMENU hMenu=NULL;
		
	hCursor=LoadCursor(  lptw->hInstance,IDC_ARROW);
	SetClassLong(lptw->hWndParent, GCL_HCURSOR,	(LONG) hCursor); 
	SetClassLong(lptw->hWndText,GCL_HCURSOR,(LONG) hCursor); 
	InvalidateCursor(); 
	hMenu=GetMenu(lptw->hWndParent);
	NumsMenu=GetMenuItemCount (hMenu);
	EnableNMenus(lptw,NumsMenu);
	EnableToolBar(lptw);

	ScilabIsStarting=FALSE;

	ShowWindow (lptw->hWndParent,SW_SHOWDEFAULT);
	BringWindowToTop (lptw->hWndParent);
	SetFocus(lptw->hWndParent);
	SetFocus(lptw->hWndText);
	
	/* Initialize 'lines' */
	nc = lptw->ClientSize.x / lptw->CharSize.x;
	nl = lptw->ClientSize.y / lptw->CharSize.y;
	/** send number of lines info to scilab **/
	if ((lptw->ClientSize.x>0) && (lptw->ClientSize.y>0))	C2F (scilines) (&nl, &nc);

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
