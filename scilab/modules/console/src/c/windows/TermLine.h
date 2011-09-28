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

#ifndef __TERMLINE_H__
#define __TERMLINE_H__

#include "BOOL.h"

/**
* returns current line
* @return current line
*/ 
char *getCurrentLine(void);

/**
* returns line before caret
* return line
*/
char *getLineBeforeCaret(void);

/**
* returns line after caret
* return line
*/
char *getLineAfterCaret(void);

/**
* moves to the beginning of the line 
*/ 
void moveBeginningLine(void);

/**
* moves to the end of the line 
*/ 
void moveEndLine(void);

/**
* moves back a single character 
*/ 
void moveBackSingleChar(void);

/**
* moves forward a single character 
*/ 
void moveForwardSingleChar(void);

/**
* moves back a single word
*/ 
void moveBackSingleWord(void);

/**
* moves forward a single word 
*/ 
void moveForwardSingleWord(void);

/**
* kills from current position to the end of line 
*/ 
void killCurrentPositionToEndLine(void);

/**
* delete the previous character 
*/ 
void deletePreviousChar(void);

/**
* deletes the current character
*/ 
void deleteCurrentChar(void);

/**
* moves back through history 
*/ 
void moveBackHistory(void);

/**
* moves forward through history  
*/ 
void moveForwardHistory(void);

/**
* redraw line
*/ 
void redrawLine(void);

/**
* kills last word 
*/ 
void killLastWord(void);

/**
* initialize new line
*/ 
void newLine(void);

/**
* clear current line
*/ 
void clearCurrentLine(void);

/**
* set prompt used by terminal
*/ 
void setCurrentPrompt(char *prompt);

/**
* display prompt
*/ 
void displayPrompt(void);

/**
* refresh line
*/ 
void refreshLine(void);

/**
* copy line on terminal
*/ 
void copyLine(char *line);

/**
* line is a history search
* @return TRUE or FALSE;
*/ 
BOOL isHistorySearch(void);

/**
* add character to current line
* @param[in] character to add
*/ 
void addCharacterCurrentLine(unsigned char ch);

/**
* put line searched in history
*/
void putLineSearchedHistory(void);

/**
* Paste current clipboard on line
*/
void pasteClipBoard(void);

#endif /* __TERMLINE_H__ */