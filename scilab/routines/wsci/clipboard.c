
/* Allan CORNET INRIA 2004 */
#include "clipboard.h"
/*-----------------------------------------------------------------------------------*/
static BOOL ThreadPasteRunning=FALSE;
static HANDLE hThreadPaste;
static char *PasteForThread=NULL; /* Chaine contenant le texte pour la thread Coller */




static BOOL ReadyForAnewLign=FALSE; /* Indique si on peut continuer à coller */
/* utiliser par les fonctions 
BOOL IsReadyForAnewLign();
void SetReadyOrNotForAnewLign(BOOL Ready);
*/

static BOOL SpecialPaste=FALSE;

/*-----------------------------------------------------------------------------------*/
void CreateThreadPaste(char *Text)
{
	DWORD IdThreadPaste;
	PasteForThread=(char*)malloc( (strlen(Text)+1)*sizeof(char));
	strcpy(PasteForThread,Text);
	PasteForThread[strlen(PasteForThread)]='\0';
	/* PasteForThread défini en global car passage via CreateThread plante sous Win98 */
	hThreadPaste=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendInputText,NULL,0,(LPDWORD)&IdThreadPaste);
	
}
/*-----------------------------------------------------------------------------------*/	
BOOL IsReadyForAnewLign(void)
{
	return ReadyForAnewLign;
}
/*-----------------------------------------------------------------------------------*/	
void SetReadyOrNotForAnewLign(BOOL Ready)
{
	ReadyForAnewLign=Ready;
}
/*-----------------------------------------------------------------------------------*/	
DWORD WINAPI SendInputText(LPVOID lpParam )

{
	#define TEMPOTOUCHE  1
	int lg=0;
	int i=0;
	
	char *TextToSend=NULL;
	
	LPTW lptw;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
	
	ThreadPasteRunning=TRUE;
	
	lg=strlen((char*)PasteForThread)+1;
	TextToSend=(char*)malloc(lg*sizeof(char));
	strcpy(TextToSend,(char*)PasteForThread);
	TextToSend[strlen(TextToSend)]='\0';

	free(PasteForThread);
	PasteForThread=NULL;

	//if (SpecialPaste==TRUE)CleanPromptFromText(TextToSend); /* Desactiver pour le moment */
		
	while ( C2F (ismenu) () == 1 ) {Sleep(TEMPOTOUCHE);}
	/* Il n'y a plus rien dans la queue des commandes */
	while ( lptw->bGetCh == FALSE ) {Sleep(TEMPOTOUCHE);}
	/* Nous sommes au prompt */
	
	
	lg=strlen(TextToSend);
	i=0;
	while(i<lg)
	{
		long count;
		
		
		count = lptw->KeyBufIn - lptw->KeyBufOut;
			
		if (count < 0) count = count+lptw->KeyBufSize;
		if (count < (long) (lptw->KeyBufSize-1)) 
			{
				if (TextToSend[i] == '\t') *lptw->KeyBufIn++ = ' ';
				else *lptw->KeyBufIn++ = TextToSend[i];
				if (lptw->KeyBufIn - lptw->KeyBuf >= (signed)lptw->KeyBufSize)
				lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
			}
	
		if (TextToSend[i]==10) i++;
		if (TextToSend[i]==13) 
			{
				SetReadyOrNotForAnewLign(FALSE);
				while ( IsReadyForAnewLign() == FALSE ) {Sleep(TEMPOTOUCHE);}
				while ( C2F (ismenu) () == 1 ) {Sleep(TEMPOTOUCHE);}
				while ( lptw->bGetCh == FALSE ) {Sleep(TEMPOTOUCHE);}
				i++;
			}
		i++;	
		
	}

	
	free(TextToSend);
	TextToSend=NULL;
	ThreadPasteRunning=FALSE;
	CloseHandle( hThreadPaste );
	
	return 0;	
}
/*-----------------------------------------------------------------------------------*/
void PasteFunction(LPTW lptw,BOOL special)
/* Modification Allan CORNET */
/* Le 18/08/03 */
/* "Coller" fonctionne correctement */
/* Ne mange plus les premiers caracteres de chaque ligne */
/* Synchronisation avec le traitement des instructions scilab */
{
	HDC hdc;
	HGLOBAL hGMem;
	TEXTMETRIC tm;
	UINT type;
	LPSTR lpMem; /* Pointeur sur la chaine du clipboard */
	
	SpecialPaste=special;	
	/* find out what type to get from clipboard */
	hdc = GetDC (lptw->hWndText);
	SelectObject(hdc, lptw->hfont);
	GetTextMetrics(hdc,(TEXTMETRIC FAR *)&tm);
	if (tm.tmCharSet == OEM_CHARSET) type = CF_OEMTEXT;
	else type = CF_TEXT;
	ReleaseDC (lptw->hWndText, hdc);
	
	/* now get it from clipboard */
	OpenClipboard (lptw->hWndText);
	hGMem = GetClipboardData (type);
	
	if (hGMem)
		{
			lpMem  = GlobalLock( hGMem );
			if (!ThreadPasteRunning) SendMessage(lptw->hWndText, WM_SETTEXT, 0,(LPARAM) lpMem);
			GlobalUnlock (hGMem);
		}
	CloseClipboard ();
}
/*-----------------------------------------------------------------------------------*/
BOOL IsEmptyClipboard(LPTW lptw)
/* Test si le Clipboard est vide */
{
	BOOL Retour=TRUE;
	HDC hdc;
	HGLOBAL hGMem;
	TEXTMETRIC tm;
	UINT type;
	LPSTR lpMem=NULL; /* Pointeur sur la chaine du clipboard */
	

	/* find out what type to get from clipboard */
	hdc = GetDC (lptw->hWndText);
	SelectObject(hdc, lptw->hfont);
	GetTextMetrics(hdc,(TEXTMETRIC FAR *)&tm);
	if (tm.tmCharSet == OEM_CHARSET) type = CF_OEMTEXT;
	else type = CF_TEXT;
	ReleaseDC (lptw->hWndText, hdc);
	
	/* now get it from clipboard */
	OpenClipboard (lptw->hWndText);
	hGMem = GetClipboardData (type);
	
	if (hGMem)
		{
			lpMem  = GlobalLock( hGMem );
			if (lpMem==NULL) Retour=TRUE;
			else Retour=FALSE;
			GlobalUnlock (hGMem);
		}
	CloseClipboard ();
	
	return Retour;
}
/*-----------------------------------------------------------------------------------*/
/***********************************
 *  Copy to Clipboard
 ***********************************/
 /* Modification Allan CORNET */
void TextCopyClip(LPTW lptw)
{
	int size, count;
	HGLOBAL hGMem;
	LPSTR cbuf, cp;
	POINT pt, end;
	TEXTMETRIC tm;
	UINT type;
	HDC hdc;

	if ((lptw->MarkBegin.x == lptw->MarkEnd.x) && 
	    (lptw->MarkBegin.y == lptw->MarkEnd.y) ) {
		/* copy user text */
		return;
	}

	size = (lptw->MarkEnd.y - lptw->MarkBegin.y + 1) 
		* (lptw->ScreenSize.x + 2) + 1;
	hGMem = GlobalAlloc(GMEM_MOVEABLE, (DWORD)size);
	cbuf = cp = (LPSTR)GlobalLock(hGMem);
	if (cp == (LPSTR)NULL)
		return;
	
	pt.x = lptw->MarkBegin.x;
	pt.y = lptw->MarkBegin.y;
	end.x   = lptw->MarkEnd.x;
	end.y   = lptw->MarkEnd.y;

	while (pt.y < end.y) {
		/* copy to global buffer */
		count = lptw->ScreenSize.x - pt.x;
		_fmemcpy(cp, lptw->ScreenBuffer + lptw->ScreenSize.x*pt.y+pt.x, count);
		/* remove trailing spaces */
		for (count=count-1; count>=0; count--) {
			if (cp[count]!=' ')
				break;
			cp[count] = '\0';
		}
		cp[++count] = '\r';
		cp[++count] = '\n';
		cp[++count] = '\0';
		cp += count;
		pt.y++;
		pt.x=0;
	}
	/* partial line */
	count = end.x - pt.x;
	if (end.y != lptw->ScreenSize.y) {
		_fmemcpy(cp, lptw->ScreenBuffer + lptw->ScreenSize.x*pt.y+pt.x, count);
		cp[count] = '\0';
	}
	size = _fstrlen(cbuf) + 1;
	GlobalUnlock(hGMem);
	hGMem = GlobalReAlloc(hGMem, (DWORD)size, GMEM_MOVEABLE);
	/* find out what type to put into clipboard */
	hdc = GetDC(lptw->hWndText);
	SelectObject(hdc, lptw->hfont);
	GetTextMetrics(hdc,(TEXTMETRIC FAR *)&tm);
	if (tm.tmCharSet == OEM_CHARSET)
		type = CF_OEMTEXT;
	else
		type = CF_TEXT;
	ReleaseDC(lptw->hWndText, hdc);
	/* give buffer to clipboard */
	OpenClipboard(lptw->hWndParent);
	EmptyClipboard();
	SetClipboardData(type, hGMem);
	CloseClipboard();
}
/*-----------------------------------------------------------------------------------*/
void CleanClipboard(LPTW lptw)
{
	if (ThreadPasteRunning)
	{
		SuspendThread(hThreadPaste);
		TerminateThread(hThreadPaste,1);
        ThreadPasteRunning=FALSE;
        CloseHandle( hThreadPaste );
	}

	OpenClipboard(lptw->hWndParent);
	EmptyClipboard();
	CloseClipboard();
}
/*-----------------------------------------------------------------------------------*/
HANDLE GetHandleThreadPaste(void)
{
	return hThreadPaste;
}
/*-----------------------------------------------------------------------------------*/
BOOL GetThreadPasteRunning(void)
{
	return ThreadPasteRunning;
}
/*-----------------------------------------------------------------------------------*/
void SetThreadPasteRunning(BOOL Running)
{
	ThreadPasteRunning=Running;
}
/*-----------------------------------------------------------------------------------*/
