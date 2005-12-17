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


/* wgraph.c */
#define TOOLBAR_ZOOM					0x8801
#define TOOLBAR_UNZOOM				0x8802
#define TOOLBAR_ROTATE3D			0x8803
#define TOOLBAR_GED						0x8804
#define TOOLBAR_LINE					0x8805
#define TOOLBAR_POLYLINE			0x8806
#define TOOLBAR_ARROW					0x8807
#define TOOLBAR_DOUBLEARROW		0x8808
#define TOOLBAR_TEXT					0x8809
#define TOOLBAR_RECTANGLE			0x8810
#define TOOLBAR_CIRCLE				0x8811
#define TOOLBAR_PICKER				0x8812


#define M_ABOUT				NUMMENU+1
#define M_COPY_CLIP			NUMMENU+2
#define M_CHOOSE_FONT		NUMMENU+3
#define M_WRITEINI			NUMMENU+4
#define M_PASTE				NUMMENU+5

/* wtext.c */

/* font stuff */
#define TEXTFONTSIZE 11
#define TEXTFONTNAME "Lucida Console"

#define M_HELPON			NUMMENU+6
#define M_CONSOLE			NUMMENU+7
#define M_PRINTSELECTION	NUMMENU+8
#define M_SPECIALPASTE		NUMMENU+9
#define M_OPENSELECTION		NUMMENU+10
#define M_CUT				NUMMENU+11
#define M_EVALSELECTION		NUMMENU+12
#define M_SELECT_ALL		NUMMENU+13

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

#define IDC_STATIC                      -1

#endif /*  __WRESOURCE_H__ */
