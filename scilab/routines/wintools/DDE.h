/**********************************************************************************/
/* Allan CORNET */
/* INRIA 2004 */
/**********************************************************************************/
#ifndef __DDE__

#define __DDE__
#include "windows.h"
/**********************************************************************************/
HDDEDATA CALLBACK DdeCallback(UINT uType,UINT uFmt,HCONV hconv,HSZ hsz1,HSZ hsz2,HDDEDATA hdata,DWORD dwData1,DWORD dwData2);
BOOL DDEExecute(char* szCommand,int TimeOut);
BOOL DDERequest(char* szItem,int TimeOut,char *Result);
BOOL DDEPoke(char* szItem, char* szData,int TimeOut);
BOOL DDEOpenConnection(char *strServeur, char *strTopic);
BOOL DDECloseConnection(void);
BOOL DDEIsOpen(void);
/**********************************************************************************/
#endif /* __DDE__ */

