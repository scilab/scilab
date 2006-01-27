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
/*-----------------------------------------------------------------------------------*/
#ifndef __WGRAPH__
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
void scig_replay_hdc (char c, integer win_num, HDC hdc, int width, int height, int scale);
void set_delete_win_mode();
int C2F (deletewin) (integer * number);
void CopyToClipboardEMF (struct BCG *ScilabGC);
void CopyToClipboardBitmap (struct BCG *ScilabGC);
void SciViewportMove (ScilabGC, x, y);
void SciViewportGet (ScilabXgc, x, y);
void GPopupResize (struct BCG * ScilabXgc,int * width,int * height);
static void ScilabPaintWithBitmap(HWND hwnd,HDC hdc , struct BCG *ScilabGC);
static void sci_extra_margin(HDC hdc_c , struct BCG *ScilabGC);
static void ScilabPaintWithBitmap(HWND hwnd,HDC hdc , struct BCG *ScilabGC);
static void sci_extra_margin(HDC hdc_c , struct BCG *ScilabGC);
void ScilabPaint (HWND hwnd, struct BCG *ScilabGC);
static void ScilabNoPaint (HWND hwnd, struct BCG *ScilabGC);
int ScilabGResize (HWND hwnd, struct BCG *ScilabGC, WPARAM wParam);
void scig_replay_hdc (char c, integer win_num, HDC hdc, int width,int height,  int scale);
/*-----------------------------------------------------------------------------------*/


/* if thid flag is set to one then a pixmap is used 
 * when painting for windows with CurPixmapStatus==0.
 */ 
static int emulate_backing_store = 1; /* to use  ScilabPaintWithBitmap*/
/* static COLORREF DefaultBackground = RGB(255,255,255); define in periwin.c */

void HideGraphToolBar(struct BCG * ScilabGC);
void ShowGraphToolBar(struct BCG * ScilabGC);

int GetScreenProperty(char *prop, char *value);

integer GetCurrentFigureWindows(void);
void SetCurrentFigureWindows(integer win);

HDC TryToGetDC(HWND hWnd);

int Interface_XS2BMP(int figurenum,char *filename);
int Interface_XS2EMF(int figurenum,char *filename);

extern HDC GetPrinterDC(void);

#endif /* __WGRAPH__ */
/*-----------------------------------------------------------------------------------*/