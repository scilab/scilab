/* Allan CORNET */
/* Scilab, INRIA 2004 */

#ifndef __REGISTRY__
#define __REGISTRY__

#include <windows.h>
#include "stack-c.h"
#include "version.h"
#include "bcg.h"
/*-----------------------------------------------------------------------------------*/
void ReadRegistryTxt (LPTW lptw);
void WriteRegistryTxt (LPTW lptw);
/*-----------------------------------------------------------------------------------*/
void ReadRegistryGraph (struct BCG *ScilabGC);
void WriteRegistryGraph (struct BCG *ScilabGC);
/*-----------------------------------------------------------------------------------*/
#endif /* __REGISTRY__ */