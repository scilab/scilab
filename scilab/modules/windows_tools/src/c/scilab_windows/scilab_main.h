/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCILAB_MAIN__H__
#define __SCILAB_MAIN__H__

#include "realmain.h" /* InitScriptType */

/**
* common main for windows
* @param[in] no startup
* @param[in] path script
* @param[in] path type
* @param[in] size path
* @param[in] stacksize
*/
void sci_windows_main ( int *nos, char *path, InitScriptType pathtype, int *lpath, int memory);

#endif /* __SCILAB_MAIN__H__ */
/*-----------------------------------------------------------------------------------*/ 
