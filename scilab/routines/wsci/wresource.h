#ifndef __WRESOURCE_H__
#define __WRESOURCE_H__

/*******************************************
 * Original source : GNUPLOT - win/wresourc.h 
 * modified for Scilab 
 *******************************************
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
 * AUTHORS
 * 
 *   Maurice Castro
 *   Russell Lang
 * Modifications for Scilab 
 *   Jean-Philipe Chancelier 
 *   Bugs and mail : Scilab@inria.fr 
 */

/* This contains items internal to wgnuplot.dll
   that are used by the Resource Compiler */

/* wmenu.c */
#define ID_PROMPT 300
#define ID_ANSWER 302
#define NUMMENU   256

/* wpause.c */

/* wtext.c */
#define AB_ICON  250
#define AB_TEXT1 251
#define AB_TEXT2 252
#define AB_TEXT3 253

/* wgraph.c */
#define M_GRAPH_TO_TOP		NUMMENU+1
#define M_CHOOSE_FONT		NUMMENU+2
#define M_BACKGROUND		NUMMENU+3
#define M_COLOR				NUMMENU+4
#define M_COPY_CLIP			NUMMENU+5
#define M_LINESTYLE			NUMMENU+6
#define M_PRINT				NUMMENU+7
#define M_WRITEINI			NUMMENU+8
#define M_PASTE				NUMMENU+9
#define M_ABOUT				NUMMENU+10
#define M_REBUILDTOOLS		NUMMENU+11
/* wtext.c */
#define M_SYSCOLORS			NUMMENU+12
#define M_HELPON			NUMMENU+13
#define M_CONSOLE			NUMMENU+14
#define M_PRINTSELECTION	NUMMENU+15
#define M_SPECIALPASTE		NUMMENU+16
#define M_OPENSELECTION		NUMMENU+17
#define M_CUT				NUMMENU+18
#define M_EVALSELECTION		NUMMENU+19
#define M_SELECT_ALL		NUMMENU+20

/* wprinter.c */
#define PSIZE_SBOX 100
#define PSIZE_DEF 101
#define PSIZE_OTHER 102
#define PSIZE_DEFX 103
#define PSIZE_DEFY 104
#define PSIZE_X 105
#define PSIZE_Y 106
#define PSIZE_OFFBOX 107
#define PSIZE_OFFX 108
#define PSIZE_OFFY 109
#define CANCEL_PCDONE 120

/* gvwprn.c */

#define SPOOL_PORT      515

/* wgraph.c */
/* line style dialog box */
/** 
#define LS_LINENUM 200
#define LS_MONOBOX 201
#define LS_COLORBOX 202
#define LS_MONOSTYLE 203
#define LS_MONOWIDTH 204
#define LS_CHOOSECOLOR 205
#define LS_COLORSAMPLE 206
#define LS_COLORSTYLE 207
#define LS_COLORWIDTH 208
#define LS_DEFAULT 209
**/

#define PS_COLOR 200
#define PS_LAND  201
#define PS_TYPE  202

/* test choose menu */
#define CHOOSE_LINENUM 210
#define CHOOSE_TEXT    211

/* mdialog  */
#define IDD_INFO1                       230
#define IDD_INFO2                       231
#define IDD_INFO3                       232

#define IDD_MCHOICE1                    233
#define IDD_MCHOICE2                    234
#define IDD_MCHOICE3                    235


#define IDE_MD1                         240
#define IDE_MD2                         241
#define IDE_MD3                         242

#define IDE_MDTEXT1                     245
#define IDE_MDTEXT2                     246
#define IDE_MDTEXT3                     247

#define IDE_MDTIT                       248
#define IDE_MDICON                      249

#define ID_HELPDIALOG               400
#define ID_HelpLIST                 401
#define ID_HelpLIST1                402
#define ID_RESETHelp                403
#define ID_DONE                     404
#define ID_HELPEDIT                 405
#define ID_HELPSHOW                 406
#define ID_HELPINFO                 407

#define DI_TEXT                     408
#define DI_TIT                      409

//#define IDC_LICENCE		410

#define IDC_STATIC                      -1

#endif /*  __WRESOURCE_H__ */
