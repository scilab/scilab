/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <windows.h>
#include <wincon.h>
#include <stdio.h>
#include <string.h>
#include "stack-def.h"
#include "TermLine.h"
#include "HistoryManager.h"
#include "TermConsole.h"
#include "localization.h"
#include "MALLOC.h"
#include "strdup_windows.h"
#include "TermPosition.h"
#include "../../../windows_tools/src/c/scilab_windows/console.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
static int CURRENT_MAX_LINE_SIZE = bsiz;
static char *cur_line = NULL;	/* current contents of the line */	
static char *currentPrompt = NULL;
static int cur_pos = 0;		/* current position of the cursor */
static int max_pos = 0;
/*--------------------------------------------------------------------------*/
/* do backspace on line */
static void backSpace(void);
static void initializeLineBuffer(void);
static void reallocLineBuffer(void);
/*--------------------------------------------------------------------------*/
static void initializeLineBuffer(void)
{
	int i = 0;
	if (cur_line)
	{
		FREE(cur_line);
		cur_line = NULL;
	}
	cur_line = (char*) MALLOC(sizeof(char)*CURRENT_MAX_LINE_SIZE);
	if (cur_line)
	{
		for (i = 0; i < CURRENT_MAX_LINE_SIZE; i++) cur_line[i] = '\0';
	}
	else
	{
		fprintf(stderr, "Error: Buffer line allocation.\n");
		exit(1);
	}
}
/*--------------------------------------------------------------------------*/
static void reallocLineBuffer(void)
{
	if (cur_line)
	{
		if ( max_pos > (CURRENT_MAX_LINE_SIZE - 1) )
		{
			char *ptrBackup = cur_line;
			int newCURRENT_MAX_LINE_SIZE = CURRENT_MAX_LINE_SIZE * 2;
			cur_line = (char*)REALLOC(cur_line, sizeof(char)*(newCURRENT_MAX_LINE_SIZE));
			if (cur_line == NULL)
			{
				cur_line = ptrBackup;
			}
			else
			{
				CURRENT_MAX_LINE_SIZE = newCURRENT_MAX_LINE_SIZE;
			}
		}
	}
	else
	{
		initializeLineBuffer();
	}
}
/*--------------------------------------------------------------------------*/
void moveBeginningLine(void)
{
	reallocLineBuffer();
	while (cur_pos > 0)
	{
		cur_pos -= 1;
		backSpace();
	}
}
/*--------------------------------------------------------------------------*/
void moveEndLine(void)
{
	reallocLineBuffer();
	while (cur_pos < max_pos)
	{
		TerminalPutc (cur_line[cur_pos]);
		cur_pos += 1;
	}
}
/*--------------------------------------------------------------------------*/
void moveBackSingleChar(void)
{
	reallocLineBuffer();
	if (cur_pos > 0)
	{
		cur_pos -= 1;
		backSpace();
	}
}
/*--------------------------------------------------------------------------*/
void moveForwardSingleChar(void)
{
	reallocLineBuffer();
	if (cur_pos < max_pos)
	{
		TerminalPutc(cur_line[cur_pos]);
		cur_pos += 1;
	}
}
/*--------------------------------------------------------------------------*/
void moveBackSingleWord(void)
{
	reallocLineBuffer();
	while ((cur_pos > 0) && (isspace(cur_line[cur_pos - 1]) ))
	{
		cur_pos -= 1;
		backSpace ();
	}
	while ((cur_pos > 0) && ( !isspace(cur_line[cur_pos - 1]) ))
	{
		cur_pos -= 1;
		backSpace ();
	}
	refreshLine();
}
/*--------------------------------------------------------------------------*/
void moveForwardSingleWord(void)
{
	reallocLineBuffer();
	while ( !isspace(cur_line[cur_pos]) && (cur_pos < max_pos) ) 
	{
		TerminalPutc(cur_line[cur_pos]);
		cur_pos++;
	}
	while ( isspace(cur_line[cur_pos]) && (cur_pos < max_pos) ) 
	{
		TerminalPutc(cur_line[cur_pos]);
		cur_pos++;
	}
	refreshLine();
}
/*--------------------------------------------------------------------------*/
void killCurrentPositionToEndLine(void)
{
	int i = 0;
	reallocLineBuffer();
	for (i = cur_pos; i < max_pos; i++)  cur_line[i] = '\0';
	for (i = cur_pos; i < max_pos; i++) TerminalPutc(VK_SPACE);
	for (i = cur_pos; i < max_pos; i++) backSpace ();
	max_pos = cur_pos;
}
/*--------------------------------------------------------------------------*/
void deletePreviousChar(void)
{
	reallocLineBuffer();
	if (cur_pos > 0)
	{
		int i = 0;
		cur_pos -= 1;
		backSpace ();
		for (i = cur_pos; i < max_pos; i++) cur_line[i] = cur_line[i + 1];
		max_pos -= 1;
		refreshLine();
	}
	else TerminalBeep();
}
/*--------------------------------------------------------------------------*/
void deleteCurrentChar(void)
{
	reallocLineBuffer();
	if (max_pos == 0) 
	{
		TerminalBeep();
	}
	else
	{
		if (cur_pos < max_pos)
		{
			int i =0;
			for (i = cur_pos; i < max_pos; i++) cur_line[i] = cur_line[i + 1];
			max_pos -= 1;
			refreshLine();
		}
	}
}
/*--------------------------------------------------------------------------*/
void moveBackHistory(void)
{
	char *newline = NULL;

	reallocLineBuffer();

	cur_line[max_pos + 1] = '\0';
	if (cur_line[0]== '\0')
	{
		resetSearchedTokenInScilabHistory();
		setSearchedTokenInScilabHistory(NULL);
	}

	newline = getPreviousLineInScilabHistory();

	if (newline)
	{
		clearCurrentLine();
		copyLine(newline);
		FREE(newline);
		newline = NULL;
	}
}
/*--------------------------------------------------------------------------*/
void moveForwardHistory(void)
{
	char *newline = NULL;

	reallocLineBuffer();

	cur_line[max_pos + 1] = '\0';
	if (cur_line[0]== '\0')
	{
		resetSearchedTokenInScilabHistory();
		setSearchedTokenInScilabHistory(NULL);
	}

	newline = getNextLineInScilabHistory();

	if (newline)
	{
		clearCurrentLine();
		copyLine(newline);
		FREE(newline);
		newline = NULL;
	}
}
/*--------------------------------------------------------------------------*/
void redrawLine(void)
{
	int i =0;
	char *line = getCurrentLine();

	displayPrompt();
	for (i = max_pos; i > cur_pos; i--)  backSpace ();
	if (line)
	{
		copyLine(line);
		FREE(line);
		line = NULL;
	}
}
/*--------------------------------------------------------------------------*/
void copyLine(char *line)
{
	reallocLineBuffer();

	if (line)
	{
		TerminalPrintf(line);
		CharToOem(line,cur_line);
		cur_pos = max_pos = (int)strlen (cur_line);
	}
}
/*--------------------------------------------------------------------------*/
void killLastWord(void)
{
	reallocLineBuffer();

	while ((cur_pos > 0) && (cur_line[cur_pos - 1] == VK_SPACE))
	{
		cur_pos -= 1;
		backSpace ();
	}
	while ((cur_pos > 0) && (cur_line[cur_pos - 1] != VK_SPACE))
	{
		cur_pos -= 1;
		backSpace ();
	}

	killCurrentPositionToEndLine();
}
/*--------------------------------------------------------------------------*/
void newLine(void)
{
	reallocLineBuffer();

	cur_line[0] = '\0';
	cur_pos = 0;
	max_pos = 0;
}
/*--------------------------------------------------------------------------*/
void refreshLine(void)
{
	int i = 0;

	reallocLineBuffer();

	/* write tail of string */
	for (i = cur_pos; i < max_pos; i++) TerminalPutc(cur_line[i]);

	/* write a space at the end of the line in case we deleted one */
	TerminalPutc(VK_SPACE);

	/* backup to original position */
	for (i = max_pos + 1; i > cur_pos; i--) backSpace ();

}
/*--------------------------------------------------------------------------*/
void clearCurrentLine(void)
{
	int i = 0;

	reallocLineBuffer();

	for (i = 0; i < max_pos; i++) cur_line[i] = '\0';

	moveBeginningLine();
	
	for (i = 0; i < max_pos; i++) TerminalPutc(VK_SPACE);

	TerminalPutc('\r');
	displayPrompt();

	newLine();
}
/*--------------------------------------------------------------------------*/
static void backSpace(void)
{
	int X = 0, Y = 0;
	reallocLineBuffer();

	TermGetPosition(&X, &Y);
	if ( (X - 1) < 0 )
	{
		X = getXConsoleScreenSize();
		Y = Y - 1;
		TermSetPosition(X, Y);
	}
	else TerminalPutc(VK_BACK);
}
/*--------------------------------------------------------------------------*/
static char *getCurrentPrompt(void)
{
	return currentPrompt;
}
/*--------------------------------------------------------------------------*/
void setCurrentPrompt(char *prompt)
{
	currentPrompt = prompt;
}
/*--------------------------------------------------------------------------*/
void displayPrompt(void)
{
	int X = 0, Y = 0;

	/* check position */
	TermGetPosition(&X, &Y);
	if (X) TerminalPrintf("\n");

	TerminalPrintf(getCurrentPrompt());
}
/*--------------------------------------------------------------------------*/
char *getCurrentLine(void)
{
	char *line = NULL;

	reallocLineBuffer();

	cur_line[max_pos + 1] = '\0';
	line = strdup_windows(cur_line);
	if (line) OemToChar(cur_line, line);
	return line;
}
/*--------------------------------------------------------------------------*/
char *getLineBeforeCaret(void)
{
	char *line = NULL;

	reallocLineBuffer();
	line = strdup_windows(cur_line);
	line[cur_pos] = '\0';
	return line;
}
/*--------------------------------------------------------------------------*/
char *getLineAfterCaret(void)
{
	char *line = NULL;

	reallocLineBuffer();
	if (cur_pos != max_pos)
	{
		line = strdup_windows(&cur_line[cur_pos]);
		line[(max_pos - cur_pos) + 1] = '\0';
	}
	else
	{
		line = strdup_windows("");
	}
	return line;
}
/*--------------------------------------------------------------------------*/
void addCharacterCurrentLine(unsigned char ch)
{
	int i = 0;

	reallocLineBuffer();

	for (i = max_pos; i > cur_pos; i--) cur_line[i] = cur_line[i - 1];

	cur_line[cur_pos] = ch;
	cur_pos += 1;
	max_pos += 1;
	cur_line[max_pos] = '\0';

	setSearchedTokenInScilabHistory(cur_line);

	if (cur_pos < max_pos) refreshLine();
}
/*--------------------------------------------------------------------------*/
BOOL isHistorySearch(void)
{
	return (cur_line[0] == '!');
}
/*--------------------------------------------------------------------------*/
void putLineSearchedHistory(void)
{
	char *line = NULL;
	char *token = getCurrentLine();

	if (token)
	{
		if ( (int)strlen(token) > 1 )
		{
			setSearchedTokenInScilabHistory(&token[1]);
			line = getNextLineInScilabHistory();
		}
		FREE(token);
		token = NULL;
	}

	clearCurrentLine();

	if (line)
	{
		copyLine(line);
		FREE(line);
		line = NULL;
	}
}
/*--------------------------------------------------------------------------*/
void pasteClipBoard(void)
{
	HGLOBAL hGMem = NULL;
	LPSTR lpMem; /* Ptr on clipboard */

	int typeClipboard = CF_TEXT;

	OpenClipboard(NULL);

	hGMem = GetClipboardData (typeClipboard);
	if (hGMem)
	{
		
		char *CurrentLine = getCurrentLine();

		lpMem  = GlobalLock( hGMem );
		if (lpMem)
		{
			char *newline = (char*)MALLOC(sizeof(char)*(strlen(CurrentLine)+ strlen(lpMem)));
			strncpy(newline, CurrentLine, cur_pos);
			strcat(newline, lpMem);
			strcat(newline, &CurrentLine[cur_pos]);

			clearCurrentLine();
			copyLine(newline);
			FREE(newline);
		}
		GlobalUnlock (hGMem);
		FREE(CurrentLine);
	}

	CloseClipboard ();
}
/*--------------------------------------------------------------------------*/
