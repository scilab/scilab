/* Allan CORNET */
/* INRIA 2004 */
#ifndef __INTERFDDE__

#define __INTERFDDE__
/*-----------------------------------------------------------------------------------*/
#include <Windows.h>
#include "wcommon.h"
#include "../stack-c.h"
#include "DDE.h"
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEopen _PARAMS((char *fname));
int	InterfaceWindowsDDEclose _PARAMS((char *fname));
int	InterfaceWindowsDDEexec _PARAMS((char *fname));
int	InterfaceWindowsDDEpoke _PARAMS((char *fname));
int	InterfaceWindowsDDEreq _PARAMS((char *fname));
int	InterfaceWindowsDDEIsOpen _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/
#endif /* __INTERFDDE__ */
