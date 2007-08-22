/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __WMCOPYDATA_H__
#define __WMCOPYDATA_H__

#include <windows.h>
#include "machine.h"

typedef struct tagMYREC
{
   char  CommandFromAnotherScilab[PATH_MAX];
} 
MYREC;

/**
* Send a command to another Scilab
* @param[in] Title Window source
* @param[in] Title Window destination
* @param[in] command to send
* @return TRUE or FALSE
*/
BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource,char *ScilabWindowNameDestination,char *CommandLine);

/**
* Get a command from another scilab
* @param[in] Title
* @param[in] command to send
* @return TRUE or FALSE
*/
BOOL GetCommandFromAnotherScilab(char *TitleWindowSend,char *CommandLine);

/**
* Receinve data from another scilab
* @param[in] Handle on Window
* @param[in] COPYDATASTRUCT
* @return TRUE or FALSE
*/
BOOL ReceiveFromAnotherScilab(HWND hWndSend,PCOPYDATASTRUCT MyCopyDataStruct); 

#endif /*__WMCOPYDATA_H__ */
/*-----------------------------------------------------------------------------------*/ 
