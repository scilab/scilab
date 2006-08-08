/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intdde.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  extern int InterfaceWindowsDDEopen _PARAMS((char *fname));
  extern int InterfaceWindowsDDEclose _PARAMS((char *fname));
  extern int InterfaceWindowsDDEexec _PARAMS((char *fname));
  extern int InterfaceWindowsDDEpoke _PARAMS((char *fname));
  extern int InterfaceWindowsDDEreq _PARAMS((char *fname));
  extern int InterfaceWindowsDDEIsOpen _PARAMS((char *fname));
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(intddeopen) _PARAMS((char *fname))
{
#ifdef _MSC_VER
	InterfaceWindowsDDEopen(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intddeclose) _PARAMS((char *fname))
{
#ifdef _MSC_VER
	InterfaceWindowsDDEclose(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intddeexec) _PARAMS((char *fname))
{
#ifdef _MSC_VER
	InterfaceWindowsDDEexec(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intddepoke) _PARAMS((char *fname))
{
#ifdef _MSC_VER
	InterfaceWindowsDDEpoke(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intddereq) _PARAMS((char *fname))
{
#ifdef _MSC_VER
	InterfaceWindowsDDEreq(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intddeisopen) _PARAMS((char *fname))
{
#ifdef _MSC_VER
	InterfaceWindowsDDEIsOpen(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
