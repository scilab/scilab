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
#define TEXTFONTSIZE 9
#define TEXTFONTNAME "Terminal"
/*-----------------------------------------------------------------------------------*/
void ReadTextIni (LPTW lptw);
void WriteTextIni (LPTW lptw);
/*-----------------------------------------------------------------------------------*/
void ReadGraphIni (struct BCG *ScilabGC);
void WriteGraphIni (struct BCG *ScilabGC);
/*-----------------------------------------------------------------------------------*/
#endif /* __REGISTRY__ */