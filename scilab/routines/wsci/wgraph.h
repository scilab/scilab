/*-----------------------------------------------------------------------------------*/
/*******************************************
 * Original source : GNUPLOT - win/wgraph.c
 * modified for Scilab 
 *******************************************
 *
 * Copyright (C) 1992   Maurice Castro, Russell Lang
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
 * AUTHORS (GNUPLOT) 
 *   Maurice Castro
 *   Russell Lang
 *
 * Modifications for Scilab 
 *   Jean-Philipe Chancelier 
 *   CORNET Allan 2004
 *   Bugs and mail : Scilab@inria.fr 
 */
/*-----------------------------------------------------------------------------------*/#ifndef __WGRAPH__
#define __WGRAPH__

#ifndef STRICT
#define STRICT
#endif


#define MAXPRINTF 512

/*-----------------------------------------------------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <shellapi.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#include "../version.h"
#include "wresource.h"
#include "wcommon.h"
#include "../graphics/scigraphic.h"
#include "../graphics/Graphics.h"

#include "../graphics/DestroyObjects.h"
#include "../graphics/GetProperty.h"
#include "../graphics/DrawObjects.h"
#include "Events.h"

extern int version_flag();

/*-----------------------------------------------------------------------------------*/

typedef struct
{
  MSG msg;
  integer flag;
}
SCISEND;
SCISEND sciSend;
/*-----------------------------------------------------------------------------------*/
extern TW textwin;
extern GW graphwin;
extern void SetGHdc __PARAMS ((HDC lhdc, int width, int height));
extern void sci_pixmapclear(HDC hdc_c, struct BCG *ScilabGC );
extern void sci_pixmapclear_rect(HDC hdc_c, struct BCG *ScilabGC,int w,int h); 
extern void sci_pixmap_resize(struct BCG * ScilabGC, int x, int y) ;
extern void   set_no_delete_win_mode() ;
extern void DebugGW (char *fmt,...);
extern void DebugGW1 (char *fmt,...);
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));
extern void delete_sgwin_entities(int win_num,int v_flag);
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,long int nams_len));  

static void scig_replay_hdc (char c, integer win_num, HDC hdc, int width, int height, int scale);
void set_delete_win_mode();

int C2F (deletewin) (integer * number);
EXPORT void WINAPI GraphPrint (struct BCG *ScilabGC);
EXPORT void WINAPI GraphRedraw (struct BCG *ScilabGC);
void NewCopyClip (struct BCG *ScilabGC);
void CopyClip (struct BCG *ScilabGC);
int CopyPrint (struct BCG *ScilabGC);
void SciViewportMove (ScilabGC, x, y);
void SciViewportGet (ScilabXgc, x, y);
void GPopupResize (struct BCG * ScilabXgc,int * width,int * height);

void sciSendMessage (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int sciPeekMessage (MSG * msg);
static void ScilabPaintWithBitmap(HWND hwnd,HDC hdc , struct BCG *ScilabGC);
static void sci_extra_margin(HDC hdc_c , struct BCG *ScilabGC);
static void ScilabPaintWithBitmap(HWND hwnd,HDC hdc , struct BCG *ScilabGC);
static void sci_extra_margin(HDC hdc_c , struct BCG *ScilabGC);
static void ScilabPaint (HWND hwnd, struct BCG *ScilabGC);
static void ScilabNoPaint (HWND hwnd, struct BCG *ScilabGC);
static int ScilabGResize (HWND hwnd, struct BCG *ScilabGC, WPARAM wParam);
static void scig_replay_hdc (char c, integer win_num, HDC hdc, int width,int height,  int scale);
EXPORT LRESULT CALLBACK WndGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
EXPORT LRESULT CALLBACK WndParentGraphProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
extern Scig_deletegwin_handler set_scig_deletegwin_handler (Scig_deletegwin_handler f);
/*-----------------------------------------------------------------------------------*/
static void sci_extra_margin(HDC hdc , struct BCG *ScilabGC);
/*-----------------------------------------------------------------------------------*/


/* if thid flag is set to one then a pixmap is used 
 * when painting for windows with CurPixmapStatus==0.
 */ 
static int emulate_backing_store = 1; /* to use  ScilabPaintWithBitmap*/
/* static COLORREF DefaultBackground = RGB(255,255,255); define in periwin.c */

extern void CreateMyTooltip (HWND hwnd,char ToolTipString[30]);


#define ToolBarHeight 24
#define ButtonToolBarWeight 24
static BOOL DefaultShowToolBar=TRUE;
void SetDefaultShowToolBar(BOOL valShowToolBar);
void CreateGraphToolBar(struct BCG * ScilabGC); 
void HideGraphToolBar(struct BCG * ScilabGC);
void ShowGraphToolBar(struct BCG * ScilabGC);
void RefreshGraphToolBar(struct BCG * ScilabGC) ;


BOOL HdcToBmpFile(HDC hdc, char *pszflname);
BOOL HwndToBmpFile(HWND hwnd, char *pszflname);
void ExportBMP(struct BCG *ScilabGC,char *pszflname);
void ExportEMF(struct BCG *ScilabGC,char *pszflname);
int GetScreenProperty(char *prop, char *value);

integer GetCurrentFigureWindows(void);
void SetCurrentFigureWindows(integer win);

HDC TryToGetDC(HWND hWnd);

#endif /* __WGRAPH__ */
/*-----------------------------------------------------------------------------------*/