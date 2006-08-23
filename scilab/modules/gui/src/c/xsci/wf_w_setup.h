/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1985 by Supoj Sutanthavibul
 *
 * "Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both the copyright
 * notice and this permission notice appear in supporting documentation. 
 * No representations are made about the suitability of this software for 
 * any purpose.  It is provided "as is" without express or implied warranty."
 */

#define		PIX_PER_INCH		80
#define		PIX_PER_CM		30	/* ((int)(PIX_PER_INCH / 2.54
						 * + (5.0/2))) */

/* Portrait dimensions */
#define		DEF_CANVAS_HT_PORT	9*PIX_PER_INCH
#define		DEF_CANVAS_WD_PORT	8*PIX_PER_INCH

/* Landscape dimensions */
#define		DEF_CANVAS_HT_LAND	8*PIX_PER_INCH
#define		DEF_CANVAS_WD_LAND	10*PIX_PER_INCH

#define		RULER_WD		24
#ifndef MAX_TOPRULER_WD
#define		MAX_TOPRULER_WD		1020
#endif
#ifndef MAX_SIDERULER_HT
#define		MAX_SIDERULER_HT	860
#endif
#define		MIN_MOUSEFUN_WD		240

#define		MAXDEPTH		999

#define		SW_PER_ROW_PORT 2	/* switches/row in mode panel */
#define		SW_PER_ROW_LAND 2	/* same for landscape mode */
#define		SW_PER_COL_PORT 17
#define		SW_PER_COL_LAND 17

#define		MODE_SW_HT	32	/* height of a mode switch icon */
#define		MODE_SW_WD	36	/* width of a mode switch icon */

#define		DEF_INTERNAL_BW		1
#define		POPUP_BW		2

extern int	CMDPANEL_WD;
extern int	MODEPANEL_WD;
extern int	MODEPANEL_SPACE;
extern int	MSGFORM_WD;
extern int	MSGPANEL_WD;
extern int	MOUSEFUN_WD;
extern int	INDPANEL_WD;
extern int	CANVAS_WD, CANVAS_HT;
extern int	INTERNAL_BW;
extern int	TOPRULER_WD, TOPRULER_HT;
extern int	SIDERULER_WD, SIDERULER_HT;
extern int	SW_PER_ROW, SW_PER_COL;
