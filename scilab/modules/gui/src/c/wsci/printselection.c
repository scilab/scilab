/*-----------------------------------------------------------------------------------*/ 
#include "../../windows_tools/src/c/TextToPrint.h"
#include "printselection.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
void PrintSelection(HWND hWND,char *Entete,HFONT hfont)
{
	HDC hdc;
	HGLOBAL hGMem;

	TEXTMETRIC tm;
	UINT type;
	LPSTR lpMem; /* Pointeur sur la chaine du clipboard */
	char *MessagePaste=NULL;

	/*Récupere ce qu'il y a dans le presse papier*/
	hdc = (HDC)TryToGetDC (hWND);
	SelectFont (hdc,  hfont);
	GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
	if (tm.tmCharSet == OEM_CHARSET) type = CF_OEMTEXT;
	else type = CF_TEXT;
	ReleaseDC (hWND, hdc);
	/* now get it from clipboard */
	OpenClipboard (hWND);
	hGMem = GetClipboardData (type);
	if (hGMem)
	{
		int l=0;

		lpMem= GlobalLock (hGMem);
		l=(int)strlen(lpMem);
		MessagePaste=(char*)MALLOC( (l+1)*sizeof(char));
		strcpy(MessagePaste,lpMem);
		MessagePaste[l]='\0';
		GlobalUnlock (hGMem);


	}
	CloseClipboard ();
	PrintString(MessagePaste,Entete);
}
/*-----------------------------------------------------------------------------------*/ 
