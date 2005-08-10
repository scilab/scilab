/* Allan CORNET */
/* Scilab, INRIA 2004 */

#ifndef __REGISTRY__
#define __REGISTRY__

#include <windows.h>
#include "../stack-c.h"
#include "../version.h"
#include "../graphics/bcg.h"
/*-----------------------------------------------------------------------------------*/
int InterfaceWindowsQueryRegistry _PARAMS((char *fname));
BOOL WindowsQueryRegistry(char *ParamIn1,char *ParamIn2,char *ParamIn3,char *ParamOut1,int *ParamOut2,BOOL *OuputIsREG_SZ);
BOOL WindowsQueryRegistryList(char *ParamIn1,char *ParamIn2,int dimMax,char **ListKeys);
BOOL WindowsQueryRegistryNumberOfElementsInList(char *ParamIn1,char *ParamIn2,int *Number);
HKEY GetHkeyrootFromString(char *string);
/*-----------------------------------------------------------------------------------*/
void ReadRegistryTxt (LPTW lptw);
void WriteRegistryTxt (LPTW lptw);
/*-----------------------------------------------------------------------------------*/
void ReadRegistryGraph (struct BCG *ScilabGC);
void WriteRegistryGraph (struct BCG *ScilabGC);
/*-----------------------------------------------------------------------------------*/
#endif /* __REGISTRY__ */