/*-----------------------------------------------------------------------------------*/ 
/* CORNET Allan */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <Windows.h>
/*-----------------------------------------------------------------------------------*/ 
#define MSG_ERROR20 "ERROR"
#define MSG_ERROR36 "Error : GetDC fails"
#define MSG_ERROR37 "Error : GetDC Input don't handle NULL"
/*-----------------------------------------------------------------------------------*/ 
HDC TryToGetDC(HWND hWnd)
{
	HDC hDCRet=NULL;

	if (hWnd)
	{
		hDCRet=GetDC(hWnd);
		if (hDCRet == NULL)
		{
#ifdef _DEBUG
			MessageBox(NULL,MSG_ERROR36,MSG_ERROR20,MB_ICONWARNING);
#endif
		}
	}
	else
	{
#ifdef _DEBUG
		MessageBox(NULL,MSG_ERROR37,MSG_ERROR20,MB_ICONWARNING);
#endif
	}

	return (HDC)hDCRet;
}
/*-----------------------------------------------------------------------------------*/ 