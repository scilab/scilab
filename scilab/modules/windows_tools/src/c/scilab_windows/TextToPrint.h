/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __TEXTTOPRINT__
#define __TEXTTOPRINT__
/*--------------------------------------------------------------------------*/

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <commdlg.h>
#include <stdio.h>
#include <ShlObj.h>
#include <time.h>
#include <math.h>
#include "BOOL.h"

/*--------------------------------------------------------------------------*/
/* EzCreateFont */
/* Fonts Properties */
#define EZ_ATTR_BOLD 1
#define EZ_ATTR_ITALIC 2
#define EZ_ATTR_UNDERLINE 4
#define EZ_ATTR_STRIKEOUT 8
/*--------------------------------------------------------------------------*/
void PrintString(char *lines, char *Entete);
void PrintFile(char *filename);
void PageHeader(HDC hdc, LPSTR Entete);
void Footer(HDC hdc, int number);
HDC GetPrinterDC(void);
BOOL ConfigurePrinterDialogBox(void);
/*--------------------------------------------------------------------------*/
extern HDC TryToGetDC(HWND hWnd);
#endif /* __TEXTTOPRINT__ */
/*--------------------------------------------------------------------------*/
