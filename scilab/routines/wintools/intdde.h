/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTDDE__
#define __INTDDE__

#ifdef WIN32
  #include <windows.h>
#endif

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

int	InterfaceWindowsDDEopen _PARAMS((char *fname));
int	InterfaceWindowsDDEclose _PARAMS((char *fname));
int	InterfaceWindowsDDEexec _PARAMS((char *fname));
int	InterfaceWindowsDDEpoke _PARAMS((char *fname));
int	InterfaceWindowsDDEreq _PARAMS((char *fname));
int	InterfaceWindowsDDEIsOpen _PARAMS((char *fname));

#endif /* __INTDDE__ */
/*-----------------------------------------------------------------------------------*/ 
