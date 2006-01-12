/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intclipboard.h"
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
  extern int InterfaceWindowsClipboard _PARAMS((char *fname,unsigned long l));
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(intclipboard) _PARAMS((char *fname,unsigned long l))
{
#ifdef WIN32
	InterfaceWindowsClipboard(fname,l);
#else
	Scierror(999,"Only for Windows\r\n");
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
