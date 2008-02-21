/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
/*--------------------------------------------------------------------------*/
Tcl_Interp *TCLinterp = NULL;
Tk_Window TKmainWindow = NULL;
/*--------------------------------------------------------------------------*/
char *UTF8toANSI(Tcl_Interp *TCLinterpl,char *StringUTF8)
{
	#define AddCharacters 4
	char *ReturnANSIString=NULL;
	if (StringUTF8)
	{
		ReturnANSIString=MALLOC(sizeof(char)*(strlen(StringUTF8)+AddCharacters));
		/* UTF to ANSI */
		Tcl_UtfToExternal(TCLinterpl, NULL, StringUTF8, (int)strlen(StringUTF8), 0, NULL, ReturnANSIString, (int)(strlen(StringUTF8)+AddCharacters), NULL, NULL,NULL);
	}
	return ReturnANSIString;
}
/*--------------------------------------------------------------------------*/
