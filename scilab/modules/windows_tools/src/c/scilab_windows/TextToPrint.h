/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
void PrintString(char *lines,char *Entete);
void PrintFile(char *filename);
void PageHeader(HDC hdc,LPSTR Entete);
void Footer(HDC hdc,int number);
HDC GetPrinterDC(void);
BOOL ConfigurePrinterDialogBox(void);
/*--------------------------------------------------------------------------*/ 
extern HDC TryToGetDC(HWND hWnd);
#endif /* __TEXTTOPRINT__ */
/*--------------------------------------------------------------------------*/ 
