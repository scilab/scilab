/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intclipboard.h"
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
  extern int InterfaceWindowsClipboard _PARAMS((char *fname));
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(intclipboard) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsClipboard(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
