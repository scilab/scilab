#ifndef __WTEXT_H__
#define  __WTEXT_H__

/*******************************************
 * Original source : GNUPLOT - win/wtext.h
 * modified for Scilab 
 *******************************************
 * Copyright (C) 1992   Russell Lang
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose with or without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code.  Modifications are to be distributed 
 * as patches to released version.
 *  
 * This software is provided "as is" without express or implied warranty.
 * 
 *
 * AUTHORS
 *   Russell Lang
 * Modified for Scilab 1997
 *   Jean-Philippe Chancelier 
 *   Allan CORNET (2003)
 */


#include <string.h>		/* use only far items */
#include <stdlib.h>
#include <ctype.h>


#include "wresource.h"
#include "resource.h"
#include "wcommon.h"
#include "..\version.h"


/*-----------------------------------------------------------------------------------*/
/* Les Definitions */
/*-----------------------------------------------------------------------------------*/
/**********************/
/* Code Touches CTRL+Key */
#define CTRLA		1
#define CTRLB		2
#define CTRLC		3
#define CTRLD		4
#define CTRLE		5
#define CTRLF		6
#define CTRLH		8
#define CTRLK		11
#define CTRLL		12
#define CTRLN		14
#define CTRLP		16
#define CTRLU		21
#define CTRLW		23

/**********************/

/* Taille du buffer circulaire des touches*/
#define KeyBufferSize 256



/* limits */
#define MAXSTR 256

#define NOTEXT 0xF0

/*-----------------------------------------------------------------------------------*/
/* Les Fonctions */
/*-----------------------------------------------------------------------------------*/

EXPORT void WINAPI TextMessage (void);
EXPORT int WINAPI TextInit (LPTW lptw);
EXPORT void WINAPI TextClose (LPTW lptw);
EXPORT void WINAPI TextToCursor (LPTW lptw);
EXPORT int WINAPI TextPutCh (LPTW lptw, BYTE ch);
EXPORT int WINAPI TextKBHit (LPTW lptw);
EXPORT int WINAPI TextGetCh (LPTW lptw);
EXPORT int WINAPI TextGetChE (LPTW lptw);
EXPORT LPSTR WINAPI TextGetS (LPTW lptw, LPSTR str, unsigned int size);
EXPORT int WINAPI TextPutS (LPTW lptw, LPSTR str);
EXPORT void WINAPI TextGotoXY (LPTW lptw, int x, int y);
EXPORT int WINAPI TextWhereX (LPTW lptw);
EXPORT int WINAPI TextWhereY (LPTW lptw);
EXPORT void WINAPI TextCursorHeight (LPTW lptw, int height);
EXPORT void WINAPI TextClearEOL (LPTW lptw);
EXPORT void WINAPI TextClearEOS (LPTW lptw);
EXPORT void WINAPI TextInsertLine (LPTW lptw);
EXPORT void WINAPI TextDeleteLine (LPTW lptw);
EXPORT void WINAPI TextScrollReverse (LPTW lptw);
void NewLine (LPTW lptw);
void UpdateText (LPTW lptw, int count);
void TextPutStr (LPTW lptw, LPSTR str);
void ReadRegistryTxt (LPTW lptw);
void LimitMark (LPTW lptw, POINT FAR * lppt);
void ClearMark (LPTW lptw, POINT pt);
void DoLine (LPTW lptw, HDC hdc, int xpos, int ypos, int offset, int count);
void DoMark (LPTW lptw, POINT pt, POINT end, BOOL mark);
void UpdateMark (LPTW lptw, POINT pt);
void TextCopyClip (LPTW lptw);
void TextMakeFont (LPTW lptw);
void TextSelectFont (LPTW lptw);
int CtrlCHit (LPTW lptw);
/*--------------------------*/
void HomeFunction _PARAMS((char *fname, unsigned long fname_len));
void ResizeScreenBuffer(LPTW lptw);
void ReAllocScreenBuffer(LPTW lptw);
void InitScreenBuffer(LPTW lptw);
void ReorganizeScreenBuffer(LPTW lptw);
int ClearScreenConsole _PARAMS((char *fname, unsigned long fname_len));
void ClearCommandWindow(LPTW lptw,BOOL Clearfirstline);
void ClearLinesScreenConsole(int NbrOfLines);
void CleanClipboard(LPTW lptw);
void SelectAll(LPTW lptw,BOOL DoAMark);
void UnSelect(LPTW lptw);
void HelpOn(LPTW lptw);
void OpenSelection(LPTW lptw);
void CutSelection(LPTW lptw);
void EvaluateSelection(LPTW lptw);
void OnRightClickMenu(LPTW lptw);
void InvalidateCursor( void ); 
BOOL HasAZoneTextSelected(LPTW lptw);
DWORD WINAPI SendInputText(LPVOID lpParam );
void CleanPromptFromText(char *Text);
/*--------------------------*/
int ReplacePrompt(char *Text,char *prompt);
/* retourne TRUE si Text a été modifié */
/*--------------------------*/
void ForceToActiveWindowParent(void);
void PrintSelection(LPTW lptw,char *Entete);
void ExitWindow(void);
void write_scilab_synchro(char *line);
DWORD WINAPI WriteTextThread(LPVOID lpParam);
int ShowWindowFunction _PARAMS((char *fname,unsigned long fname_len));
BOOL WriteIntoScilab(LPTW lptw,char *StringCommand);
void WriteIntoKeyBuffer(LPTW lptw,char *StringCommand);
BOOL IsToThePrompt(void);
void EnableNMenus(LPTW lptw,int numbermenus);
void EnableMenus(LPTW lptw);
void DisableMenus(LPTW lptw);
void EnableToolBar(LPTW lptw);
void DisableToolBar(LPTW lptw);
/*--------------------------*/
int GetCurrentLanguage(void);
/*--------------------------*/
#endif /*  __WTEXT_H__ */
