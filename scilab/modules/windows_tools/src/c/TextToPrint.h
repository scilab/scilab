/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
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
HFONT EzCreateFont (HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,int iDeciPtWidth, int iAttributes, BOOL fLogRes);
HDC GetPrinterDC(void);
BOOL ConfigurePrinterDialogBox(void);
/*--------------------------------------------------------------------------*/ 
extern HDC TryToGetDC(HWND hWnd);
#endif /* __TEXTTOPRINT__ */
/*--------------------------------------------------------------------------*/ 
