/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __CONVERTPATHUNIXTOWINDOWS_H__
#define __CONVERTPATHUNIXTOWINDOWS_H__

#include "machine.h"

/**
* convert a unix path to windows path
* replaces '/' by '\'
* @param [in]
* @param [out]
* @return
*/
BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows);

#endif /* __CONVERTPATHUNIXTOWINDOWS_H__ */
/*-----------------------------------------------------------------------------------*/
