/**********************************************************************************/
/* Allan CORNET */
/* INRIA 2004 */
/**********************************************************************************/
#include "DDE.h"
/**********************************************************************************/
static DWORD idInst=0;
static HCONV  hConv=NULL;
static BOOL DDEInUse=FALSE;
/**********************************************************************************/
HDDEDATA CALLBACK DdeCallback(
    UINT uType,     // Transaction type.
    UINT uFmt,      // Clipboard data format.
    HCONV hconv,    // Handle to the conversation.
    HSZ hsz1,       // Handle to a string.
    HSZ hsz2,       // Handle to a string.
    HDDEDATA hdata, // Handle to a global memory object.
    DWORD dwData1,  // Transaction-specific data.
    DWORD dwData2)  // Transaction-specific data.
{
    return 0;
}
/**********************************************************************************/
BOOL DDEExecute(char* szCommand)
{
	BOOL bOK=TRUE;
    HDDEDATA hData = DdeCreateDataHandle(idInst, (LPBYTE)szCommand,
                               lstrlen(szCommand)+1, 0, NULL, CF_TEXT, 0);
    if (hData==NULL)
	{
		bOK=FALSE;        
    }
    else    
	{
        DdeClientTransaction((LPBYTE)hData, 0xFFFFFFFF, hConv, 0L, 0,
                             XTYP_EXECUTE, TIMEOUT_ASYNC, NULL);
		bOK=TRUE;
    }
	return bOK;
}
/**********************************************************************************/
BOOL DDERequest(char* szItem,char *Result)
{
	BOOL bOK=FALSE;
    HSZ hszItem = DdeCreateStringHandle(idInst, szItem, 0);
    HDDEDATA hData = DdeClientTransaction(NULL,0,hConv,hszItem,CF_TEXT, 
                                 XTYP_REQUEST,5000 , NULL);
    if (hData==NULL)
    {
        bOK=FALSE;
    }
    else
    {
        DdeGetData(hData, (unsigned char *)Result, (DWORD)strlen(Result), 0);
		bOK=TRUE;
    }
	return bOK;
}
/**********************************************************************************/
void DDEPoke(char* szItem, char* szData)
{
    HSZ hszItem = DdeCreateStringHandle(idInst, szItem, 0);
	DdeClientTransaction((LPBYTE)szData, (DWORD)(lstrlen(szData)+1),
                          hConv, hszItem, CF_TEXT,
                          XTYP_POKE, 3000, NULL);
    DdeFreeStringHandle(idInst, hszItem);
}
/**********************************************************************************/
BOOL DDEOpenConnection(char *strServeur, char *strTopic)
{
	UINT iReturn;
	BOOL bOK=TRUE;
	
	if (DDEInUse)
	{
		bOK=FALSE;
	}
	else
	{
		iReturn = DdeInitialize(&idInst, (PFNCALLBACK)DdeCallback, 
                            APPCLASS_STANDARD | APPCMD_CLIENTONLY, 0 );
		if (iReturn!=DMLERR_NO_ERROR)
		{
			bOK=FALSE;
		}
		else
		{
			HSZ hszApp, hszTopic;
			hszApp = DdeCreateStringHandle(idInst, strServeur, 0);
			hszTopic = DdeCreateStringHandle(idInst, strTopic, 0);
			hConv = DdeConnect(idInst, hszApp, hszTopic, NULL);
			DdeFreeStringHandle(idInst, hszApp);
			DdeFreeStringHandle(idInst, hszTopic);
			if (hConv == NULL)
			{
				DdeUninitialize(idInst);
				bOK=FALSE;
			}
			DDEInUse=TRUE;
		}
	}
    return bOK;
}
/**********************************************************************************/
BOOL DDECloseConnection(void)
{
	BOOL bOK=FALSE;

	if (DDEInUse)
	{
		DdeDisconnect(hConv);
		DdeUninitialize(idInst);
		DDEInUse=FALSE;
		idInst=0;
		hConv=NULL;
		bOK=TRUE;
	}
	else
	{
		bOK=FALSE;
	}
	return bOK;
}
/**********************************************************************************/