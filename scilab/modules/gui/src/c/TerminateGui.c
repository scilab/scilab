/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "TerminateGui.h"
/*-----------------------------------------------------------------------------------*/ 
#include "xscion.h"
#include "fromjava.h" /* IsFromJava */
/*-----------------------------------------------------------------------------------*/ 
static int CloseConsoleGUI(void);
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include "wsci/wtext.h"
extern LPTW GetTextWinScilab(void);
extern BOOL IsWindowInterface(void);
extern void RestoreConsoleColors(void);
extern int IsFromC(void);
#endif
/*-----------------------------------------------------------------------------------*/ 
BOOL TerminateGUI(void)
{
#ifdef _MSC_VER
	if ( IsWindowInterface() ) 
	{
		CloseConsoleGUI();
	}
	else
	{
		if ( !IsFromC() && !IsFromJava() )  RestoreConsoleColors();
	}
#else
	{
		int i = 0 ;
		C2F (xscion) (&i);
		if (i != 0) CloseConsoleGUI();
	}
#endif
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/ 
static int CloseConsoleGUI(void)
{
#ifdef _MSC_VER
	LPTW lptw=GetTextWinScilab();
	TextClose (lptw);
	TextMessage ();		/* process messages */
#else

#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
