/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  extern int InterfaceWindowsClipboard _PARAMS((char *fname,unsigned long l));
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(sci_clipboard) _PARAMS((char *fname,unsigned long l))
{
#ifdef _MSC_VER
	InterfaceWindowsClipboard(fname,l);
#else
	Scierror(999,"Only for Windows\r\n");
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
