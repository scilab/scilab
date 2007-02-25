/**********************************************************************************/
/* Allan CORNET */
/* INRIA 2004 */
/**********************************************************************************/
#ifndef __DDE__
#define __DDE__
#include <windows.h>
/**********************************************************************************/
BOOL DDEExecute(char* szCommand,int TimeOut);
BOOL DDERequest(char* szItem,int TimeOut,char *Result);
BOOL DDEPoke(char* szItem, char* szData,int TimeOut);
BOOL DDEOpenConnection(char *strServeur, char *strTopic);
BOOL DDECloseConnection(void);
BOOL DDEIsOpen(void);
/**********************************************************************************/
#endif /* __DDE__ */

