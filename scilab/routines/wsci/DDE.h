/**********************************************************************************/
/* Allan CORNET */
/* INRIA 2004 */
/**********************************************************************************/
#ifndef __DDE__

#define __DDE__
#include "windows.h"
#include "ddeml.h"
/**********************************************************************************/
HDDEDATA CALLBACK DdeCallback(UINT uType,UINT uFmt,HCONV hconv,HSZ hsz1,HSZ hsz2,HDDEDATA hdata,DWORD dwData1,DWORD dwData2);
BOOL DDEExecute(char* szCommand);
BOOL DDERequest(char* szItem,char *Result);
void DDEPoke(char* szItem, char* szData);
BOOL DDEOpenConnection(char *strServeur, char *strTopic);
BOOL DDECloseConnection(void);
/**********************************************************************************/
#endif /* __DDE__ */

