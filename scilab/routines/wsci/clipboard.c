
/* Allan CORNET INRIA 2004 */
#include "clipboard.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "wgnuplib.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */

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
extern HDC TryToGetDC(HWND hWnd);
extern LPTW GetTextWinScilab(void);
extern int IsAScalar(int RhsNumber);
/*-----------------------------------------------------------------------------------*/
void CreateThreadPaste(char *Text)
{
	DWORD IdThreadPaste;
	PasteForThread=(char*)MALLOC( (strlen(Text)+1)*sizeof(char));
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
	
	LPTW lptw=GetTextWinScilab();
	
	ThreadPasteRunning=TRUE;
	
	lg=strlen((char*)PasteForThread)+1;
	TextToSend=(char*)MALLOC(lg*sizeof(char));
	strcpy(TextToSend,(char*)PasteForThread);
	TextToSend[strlen(TextToSend)]='\0';

	FREE(PasteForThread);
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

	
	FREE(TextToSend);
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
	hdc = (HDC)TryToGetDC (lptw->hWndText);
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
	hdc = (HDC)TryToGetDC (lptw->hWndText);
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
	hdc = (HDC)TryToGetDC(lptw->hWndText);
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
int	InterfaceWindowsClipboard(char *fname,unsigned long l)
{
	static int l1,n1,m1;
	char *param1=NULL,*param2=NULL,*param3=NULL;

	Rhs=Max(0,Rhs);
	CheckRhs(0,2);
	CheckLhs(0,1);

	if ( IsWindowInterface() )
	{
		if ( (Rhs == 1) && (GetType(1)==sci_strings) )
		{
			GetRhsVar(1,"c",&m1,&n1,&l1);
			param1=cstk(l1);

			if ( ( strcmp(param1,"paste") == 0 ) || ( strcmp(param1,"pastespecial") == 0 ) )
			{
				char *output=NULL ;
				LPTW lptw=GetTextWinScilab();

				output=GetTextFromClipboard(lptw);

				if (output)
				{
					CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);
					FREE(output);
					LhsVar(1)=1;
				}
				else
				{
					m1=0;
					n1=0;
					l1=0;
					CreateVar(1,"d",  &m1, &n1, &l1);
					LhsVar(1)=1;
				}

				C2F(putlhsvar)();	
				return 0;
			}
			else
			{
				Scierror(999,MSG_ERROR7);
				return 0;
			}
		}

		if (Rhs == 2)
		{
			if ( (GetType(1)==sci_strings) && (GetType(2)==sci_strings) )
			{
				GetRhsVar(1,"c",&m1,&n1,&l1);
				param1=cstk(l1);

				if ( strcmp(param1,"do") == 0 )
				{
					GetRhsVar(2,"c",&m1,&n1,&l1);
					param2=cstk(l1);

					if ( strcmp(param2,"paste") == 0 )
					{
						Callback_PASTE();
					}
					else if ( strcmp(param2,"copy") == 0 )
					{
						Callback_MCOPY();
					}
					else if ( strcmp(param2,"empty") == 0 )
					{
						Callback_EMPTYCLIPBOARD();
					}
					else
					{
						Scierror(999,"Incorrect second parameter.");
						return 0;
					}

					m1=0;
					n1=0;
					l1=0;
					CreateVar(1,"d",  &m1, &n1, &l1);
					LhsVar(1)=1;
					C2F(putlhsvar)();	
					return 0;
				}

				if ( strcmp(param1,"copy") == 0 )
				{
					char *TextToPutInClipboard=NULL;
					char **Str=NULL;
					LPTW lptw=GetTextWinScilab();

					GetRhsVar(2,"S",&m1,&n1,&Str);


					if ( (m1==1) && (n1==1) )
					{
						TextToPutInClipboard=Str[0];
						PutTextInClipboard(lptw,TextToPutInClipboard);
					}
					else
					{
						int i=0;
						int j=0;
						int l=0;
						char *TextToSendInClipboard=NULL;
						int SizeofTextToSendInClipboard=0;
						char **buffer = (char**)MALLOC( (m1*n1)*sizeof(char *) );
					
						for (i=0; i<m1; i++) for (j=0; j<n1; j++) 
						{
							SizeofTextToSendInClipboard=SizeofTextToSendInClipboard+strlen(Str[j*m1+i])+strlen(" \n");
							buffer[i*n1+j]=(char*)MALLOC(strlen(Str[j*m1+i])+1);
							sprintf(buffer[i*n1+j],"%s",Str[j*m1+i]);
						}

						TextToSendInClipboard=(char*)MALLOC( (SizeofTextToSendInClipboard)*sizeof(char) );
						strcpy(TextToSendInClipboard,"");

						for (i=0; i<m1; i++)
						{
								for (j=0; j<n1; j++) 
								{
									strcat(TextToSendInClipboard,buffer[l++]);
									strcat(TextToSendInClipboard," ");
								}
								if ( i != (m1-1) ) strcat(TextToSendInClipboard,"\n");
						}

						PutTextInClipboard(lptw,TextToSendInClipboard);

						if (buffer) {FREE(buffer);buffer=NULL;}

						if(TextToSendInClipboard) {FREE(TextToSendInClipboard);TextToSendInClipboard=NULL;}
					}

					m1=0;
					n1=0;
					l1=0;
					CreateVar(1,"d",  &m1, &n1, &l1);
					LhsVar(1)=1;
					C2F(putlhsvar)();	
					return 0;
				}
				else
				{
					Scierror(999,"Incorrect parameters. See : help clipboard");
					return 0;
				}
			}

			if ( (IsAScalar(1)) && (GetType(2)==sci_strings) )
			{
				GetRhsVar(2,"c",&m1,&n1,&l1);
				param2=cstk(l1);

				if ( ( strcmp(param2,"EMF") == 0 ) || ( strcmp(param2,"DIB") == 0 ) )
				{
					int num_win=-2;
					GetRhsVar(1,"i",&m1,&n1,&l1);
					num_win=*istk(l1);
					if (num_win>=0)
					{
						struct BCG *ScilabGC=NULL;
						ScilabGC = GetWindowXgcNumber (num_win);

						if (ScilabGC)
						{
							if ( strcmp(param2,"EMF") == 0 )
							{
								CopyToClipboardEMF (ScilabGC);
							}
							else
							{
								CopyToClipboardBitmap (ScilabGC);
							}

							m1=0;
							n1=0;
							l1=0;
							CreateVar(1,"d",  &m1, &n1, &l1);
							LhsVar(1)=1;
							C2F(putlhsvar)();	
							return 0;
						}
						else
						{
							Scierror(999,"Invalid Windows number.");
							return 0;
						}
					}
					else
					{
						Scierror(999,"parameter must be >= 0.");
						return 0;
					}
				}
				else
				{
					Scierror(999,"Second parameter must be 'EMF' or 'DIB'.");
					return 0;
				}
			}
		}
		else
		{
			Scierror(999,"Incorrect parameters. See : help clipboard");
			return 0;	
		}
	}
	else
	{
		if ( (Rhs == 2) && (IsAScalar(1)) && (GetType(2)==sci_strings) )
		{
			GetRhsVar(2,"c",&m1,&n1,&l1);
			param2=cstk(l1);
			if ( ( strcmp(param2,"EMF") == 0 ) || ( strcmp(param2,"DIB") == 0 ) )
			{
				int num_win=-2;
				GetRhsVar(1,"i",&m1,&n1,&l1);
				num_win=*istk(l1);
				if (num_win>=0)
				{
					struct BCG *ScilabGC=NULL;
					ScilabGC = GetWindowXgcNumber (num_win);

					if (ScilabGC)
					{
						if ( strcmp(param2,"EMF") == 0 )
						{
							CopyToClipboardEMF (ScilabGC);
						}
						else
						{
							CopyToClipboardBitmap (ScilabGC);
						}

						m1=0;
						n1=0;
						l1=0;
						CreateVar(1,"d",  &m1, &n1, &l1);
						LhsVar(1)=1;
						C2F(putlhsvar)();	
						return 0;
					}
					else
					{
						Scierror(999,"Invalid Windows number.");
						return 0;
					}
				}
				else
				{
					Scierror(999,"parameter must be >= 0.");
					return 0;
				}
			}
			else
			{
				Scierror(999,"Second parameter must be 'EMF' or 'DIB'.");
				return 0;
			}
		}
		else
		{
			Scierror(999,MSG_ERROR10);
			return 0;
		}
		Scierror(999,MSG_ERROR10);
		return 0;
	}
	return 0;

 // static int l1,n1,m1;

 // char *param1=NULL,*param2=NULL,*param3=NULL;

 // Rhs=Max(0,Rhs);
 // CheckRhs(0,2);
 // CheckLhs(0,1);

 // if ( IsWindowInterface() )
 // {
	//if (Rhs == 1)
	//{
	//  GetRhsVar(1,"c",&m1,&n1,&l1);
	//  param1=cstk(l1);

	//  if ( ( strcmp(param1,"paste") == 0 ) || ( strcmp(param1,"pastespecial") == 0 ) )
	//  {
	//	  char *output=NULL ;
	//	  LPTW lptw=GetTextWinScilab();

	//	  output=GetTextFromClipboard(lptw);

	//	  if (output)
	//	  {
	//		  CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);
	//		  FREE(output);
	//		  LhsVar(1)=1;
	//	  }
	//	  else
	//	  {
	//		  m1=0;
	//		  n1=0;
	//		  l1=0;
	//		  CreateVar(1,"d",  &m1, &n1, &l1);
	//		  LhsVar(1)=1;
	//	  }

	//		C2F(putlhsvar)();	
	//		return 0;
	//  }
	//  else
	//  {
	//	  Scierror(999,MSG_ERROR7);
	//	  return 0;
	//  }
	//}
	//else
	//if (Rhs == 2)
	//{
	//	if (IsAScalar(1))
	//	{
	//		if (GetType(2)==sci_strings)
	//		{
	//			GetRhsVar(2,"c",&m1,&n1,&l1);
	//			param2=cstk(l1);

	//			if ( ( strcmp(param2,"EMF") == 0 ) || ( strcmp(param2,"DIB") == 0 ) )
	//			{
	//				int num_win=-2;
	//				GetRhsVar(1,"i",&m1,&n1,&l1);
	//				num_win=*istk(l1);

	//				if (num_win>=0)
	//				{
	//					struct BCG *ScilabGC=NULL;
	//					ScilabGC = GetWindowXgcNumber (num_win);

	//					if (ScilabGC)
	//					{
	//						if ( strcmp(param2,"EMF") == 0 )
	//						{
	//							CopyToClipboardEMF (ScilabGC);
	//						}
	//						else
	//						{
	//							CopyToClipboardBitmap (ScilabGC);
	//						}
	//					}
	//					else
	//					{
	//						Scierror(999,"Invalid Windows number.");
	//						return 0;
	//					}
	//				}
	//				else
	//				{
	//					Scierror(999,"parameter must be >= 0.");
	//					return 0;
	//				}
	//			}
	//			else
	//			{
	//				Scierror(999,"Second parameter must be 'EMF' or 'DIB'.");
	//				return 0;
	//			}
	//		}
	//		else
	//		{
	//			Scierror(999,"Second parameter must be 'EMF' or 'DIB'.");
	//			return 0;
	//		}
	//	}
	//	else
	//	if ( (GetType(1)==sci_strings) && (GetType(2)==sci_strings) )
	//	{
	//		GetRhsVar(1,"c",&m1,&n1,&l1);
	//		param1=cstk(l1);

	//		if ( strcmp(param1,"do") == 0 )
	//		{
	//			GetRhsVar(2,"c",&m1,&n1,&l1);
	//			param2=cstk(l1);

	//			if ( strcmp(param2,"paste") == 0 )
	//			{
	//				Callback_PASTE();
	//			}
	//			else
	//			if ( strcmp(param2,"copy") == 0 )
	//			{
	//				Callback_MCOPY();
	//			}
	//			else
	//			if ( strcmp(param2,"empty") == 0 )
	//			{
	//				Callback_EMPTYCLIPBOARD();
	//			}
	//			else
	//			{
	//				Scierror(999,"Incorrect second parameter.");
	//				return 0;
	//			}
	//		}
	//		else
	//		if ( strcmp(param1,"copy") == 0 )
	//		{
	//			char *TextToPutInClipboard=NULL;
	//			char **Str=NULL;
	//			LPTW lptw=GetTextWinScilab();
	//			
	//			GetRhsVar(2,"S",&m1,&n1,&Str);

	//			if ( (m1==1) && (n1==1) )
	//			{
	//				TextToPutInClipboard=Str[0];
	//				PutTextInClipboard(lptw,TextToPutInClipboard);
	//			}
	//			else
	//			{
	//				int i=0;
	//				int j=0;
	//				int l=0;
	//				char *TextToSendInClipboard=NULL;
	//				int SizeofTextToSendInClipboard=0;
	//				char **buffer = (char**)MALLOC( (m1*n1)*sizeof(char *) );
	//			
	//				for (i=0; i<m1; i++) for (j=0; j<n1; j++) 
	//				{
	//					SizeofTextToSendInClipboard=SizeofTextToSendInClipboard+strlen(Str[j*m1+i])+strlen(" \n");
	//					buffer[i*n1+j]=(char*)MALLOC(strlen(Str[j*m1+i]));
	//					sprintf(buffer[i*n1+j],"%s",Str[j*m1+i]);
	//				}

	//				TextToSendInClipboard=(char*)MALLOC( (SizeofTextToSendInClipboard)*sizeof(char) );
	//				strcpy(TextToSendInClipboard,"");

	//				for (i=0; i<m1; i++)
	//				{
	//					for (j=0; j<n1; j++) 
	//					{
	//						strcat(TextToSendInClipboard,buffer[l++]);
	//						strcat(TextToSendInClipboard," ");
	//					}
	//					if ( i != (m1-1) ) strcat(TextToSendInClipboard,"\n");
	//				}
	//				PutTextInClipboard(lptw,TextToSendInClipboard);

	//				if (buffer)
	//				{
	//					for(i=0;i<m1*n1;i++) if (buffer[i]) {free(buffer[i]);buffer[i]=NULL;}
	//					FREE(buffer);buffer=NULL;
	//				}

	//				if(TextToSendInClipboard) {FREE(TextToSendInClipboard);TextToSendInClipboard=NULL;}
	//			}
	//		}
	//		else
	//		{
	//			Scierror(999,MSG_ERROR9);
	//			return 0;
	//		}
	//	}
	//	else
	//	{
	//		Scierror(999,"Incorrect parameters. See : help clipboard");
	//		return 0;
	//	}
 //   LhsVar(1)=0;
	//	C2F(putlhsvar)();	
	//}
 // }
 // else
 // {
	//  if ( IsAScalar(1) && (Rhs == 2) )
	//  {
	//	  if (GetType(2)==sci_strings)
	//	  {
	//		  GetRhsVar(2,"c",&m1,&n1,&l1);
	//		  param2=cstk(l1);

	//		  if ( ( strcmp(param2,"EMF") == 0 ) || ( strcmp(param2,"DIB") == 0 ) )
	//		  {
	//			  int num_win=-2;
	//			  GetRhsVar(1,"i",&m1,&n1,&l1);
	//			  num_win=*istk(l1);

	//			  if (num_win>=0)
	//			  {
	//				  struct BCG *ScilabGC=NULL;
	//				  ScilabGC = GetWindowXgcNumber (num_win);

	//				  if (ScilabGC)
	//				  {
	//					  if ( strcmp(param2,"EMF") == 0 )
	//					  {
	//						  CopyToClipboardEMF (ScilabGC);
	//					  }
	//					  else
	//					  {
	//						  CopyToClipboardBitmap (ScilabGC);
	//					  }
	//				  }
	//				  else
	//				  {
	//					  Scierror(999,"Invalid Windows number.");
	//					  return 0;
	//				  }
	//			  }
	//			  else
	//			  {
	//				  Scierror(999,"parameter must be >= 0.");
	//				  return 0;
	//			  }
	//		  }
	//		  else
	//		  {
	//			  Scierror(999,"Second parameter must be 'EMF' or 'DIB'.");
	//			  return 0;
	//		  }
	//	  }
	//	  else
	//	  {
	//		  Scierror(999,"Second parameter must be 'EMF' or 'DIB'.");
	//		  return 0;
	//	  }
	//		LhsVar(1)=0;
	//		C2F(putlhsvar)();	
	//  }
	//  else
	//  {
	//	  Scierror(999,MSG_ERROR10);
	//	  return 0;
	//  }
 // }
 // return 0;
}
/*-----------------------------------------------------------------------------------*/
char * GetTextFromClipboard(LPTW lptw)
{
	char *Text=NULL;

	HDC hdc;
	HGLOBAL hGMem;
	TEXTMETRIC tm;
	UINT type;
	LPSTR lpMem; /* Pointeur sur la chaine du clipboard */
	
	
	/* find out what type to get from clipboard */
	hdc = (HDC)TryToGetDC (lptw->hWndText);
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
			Text= (char*) MALLOC (sizeof(char)*(strlen(lpMem)+1));
			strcpy(Text,lpMem);
			GlobalUnlock (hGMem);
		}
	CloseClipboard ();

	return Text;
}
/*-----------------------------------------------------------------------------------*/
void PutTextInClipboard(LPTW lptw,char *Text)
{
	int size;
	HGLOBAL hGMem;
	LPSTR cbuf;
	
	TEXTMETRIC tm;
	UINT type;
	HDC hdc;

	size=strlen(Text)+1;

	hGMem = GlobalAlloc(GMEM_MOVEABLE, (DWORD)size);
	cbuf = (LPSTR)GlobalLock(hGMem);
	if (cbuf == (LPSTR)NULL) return;
	strcpy(cbuf,Text);

	/* find out what type to put into clipboard */
	hdc = (HDC)TryToGetDC(lptw->hWndText);
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
