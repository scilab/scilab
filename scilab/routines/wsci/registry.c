/*-----------------------------------------------------------------------------------*/
/* Allan CORNET */
/* Scilab, INRIA 2004 */
/*-----------------------------------------------------------------------------------*/
#include "registry.h"
/*-----------------------------------------------------------------------------------*/
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
/*-----------------------------------------------------------------------------------*/
extern void SetDefaultShowToolBar(BOOL valShowToolBar);
/*-----------------------------------------------------------------------------------*/
BOOL WindowsQueryRegistryNumberOfElementsInList(char *ParamIn1,char *ParamIn2,int *Number)
{
	BOOL bOK=TRUE;

	HKEY hKeyToOpen;
	HKEY hTestKey;

	hKeyToOpen =GetHkeyrootFromString(ParamIn1);
	if( RegOpenKeyEx(hKeyToOpen,ParamIn2, 0, KEY_READ, &hTestKey) == ERROR_SUCCESS )
	{
		DWORD retCode; 

		TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
		DWORD    cchClassName = MAX_PATH;  // size of class string 
		DWORD    cSubKeys=0;               // number of subkeys 
		DWORD    cbMaxSubKey;              // longest subkey size 
		DWORD    cchMaxClass;              // longest class string 
		DWORD    cValues;              // number of values for key 
		DWORD    cchMaxValue;          // longest value name 
		DWORD    cbMaxValueData;       // longest value data 
		DWORD    cbSecurityDescriptor; // size of security descriptor 
		FILETIME ftLastWriteTime;      // last write time 
      
		retCode = RegQueryInfoKey(
        hTestKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 

		if (retCode != ERROR_SUCCESS) bOK=FALSE;
		else *Number=cValues;

		RegCloseKey(hKeyToOpen);
	}
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int InterfaceWindowsQueryRegistry _PARAMS((char *fname))
{

  static int l1,n1,m1;

  char *param1=NULL,*param2=NULL,*param3=NULL;

  Rhs=Max(0,Rhs);
  CheckRhs(0,3);
  CheckLhs(0,1);

  if ( (Rhs == 3) || (Rhs == 2) )
	{
		char *output=NULL ;
		int *paramoutINT=NULL;
		BOOL OuputIsREG_SZ;
		BOOL TestWinQuery=FALSE;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		param1=cstk(l1);
		GetRhsVar(2,"c",&m1,&n1,&l1);
		param2=cstk(l1);
		if ( Rhs == 3 )
		{
			GetRhsVar(3,"c",&m1,&n1,&l1);
			param3=cstk(l1);

			if (strcmp(param1,"name") == 0)
			{
				int Col=0;
				int NumbersElm=0;

				WindowsQueryRegistryNumberOfElementsInList(param2,param3,&NumbersElm);
				if (NumbersElm)
				{
					static char *ListKeysName[255];

					if (NumbersElm > 255) NumbersElm=255;
					if ( WindowsQueryRegistryList(param2,param3,NumbersElm,ListKeysName) )
					{
						int i=0;

						CreateVarFromPtr( Rhs+1, "S", &NumbersElm, &n1, &ListKeysName);

						for (i=0; i<NumbersElm;i++)
						{
							free(ListKeysName[i]);
							ListKeysName[i]=NULL;
						}
						free(ListKeysName);

						LhsVar(1)=Rhs+1;
						return 0;
					}
					else
					{
						Scierror(999,"Error.\r\n");
						LhsVar(1)=0;
						return 0;
					}
				}
				else
				{
					m1=0;
					n1=0;
					l1=0;
					CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
					return 0;
				}
				return 0;
			}
		}

		output=(char*)malloc(MAX_PATH*sizeof(char));
		paramoutINT=(int*)malloc(sizeof(int));

		if ( Rhs == 3 )
		{
			TestWinQuery=WindowsQueryRegistry(param1,param2,param3,output,paramoutINT,&OuputIsREG_SZ);
		}
		else
		{
			TestWinQuery=WindowsQueryRegistry(param1,param2,NULL,output,paramoutINT,&OuputIsREG_SZ);
		}

		if ( TestWinQuery )
		{
			n1=1;
			if ( OuputIsREG_SZ )
			{
				CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);
			}
			else
			{
				CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
			}
	
			LhsVar(1) = 1;
		}
		else
		{
			Scierror(999,"Cannot query value of this type.\r\n");
			LhsVar(1)=0;
		}
	    
		free(output);
		free(paramoutINT);
	}
  else
  {
	Scierror(999,"Two arguments required\r\n");
	LhsVar(1)=0;
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
BOOL WindowsQueryRegistry(char *ParamIn1,char *ParamIn2,char *ParamIn3,char *ParamOut1,int *ParamOut2,BOOL *OuputIsREG_SZ)
{
	BOOL bOK=TRUE;
	
	HKEY key;
	DWORD Length=MAX_PATH;
	HKEY hKeyToOpen=NULL;

	hKeyToOpen =GetHkeyrootFromString(ParamIn1);
		
	if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, KEY_QUERY_VALUE , &key) == ERROR_SUCCESS )
	{
		DWORD size=4;
		char Line[MAX_PATH];
		int  Num=0;

		if ( RegQueryValueEx(key, ParamIn3, NULL, NULL, (LPBYTE)&Num, &size) == ERROR_SUCCESS )
		{
			*ParamOut2=Num;
			*OuputIsREG_SZ=FALSE;
		}
		else if ( RegQueryValueEx(key, ParamIn3, NULL, NULL, (LPBYTE)&Line, &Length) == ERROR_SUCCESS )
		{
			wsprintf(ParamOut1,"%s",Line);
			*OuputIsREG_SZ=TRUE;
		}
		else
		{
			bOK=FALSE;
		}
		RegCloseKey(key);
		
	}
	else bOK=FALSE;
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL WindowsQueryRegistryList(char *ParamIn1,char *ParamIn2,int dimMax,char **ListKeys)
{
	BOOL bOK=TRUE;
	HKEY key;
	HKEY hKeyToOpen=NULL;

	hKeyToOpen =GetHkeyrootFromString(ParamIn1);
			
	if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, KEY_READ , &key) == ERROR_SUCCESS )
	{
		int i=0;
		for (i=0; i<dimMax; i++) 
			{ 
				TCHAR    achKey[MAX_KEY_LENGTH];
				DWORD    cbName = MAX_KEY_LENGTH;
				DWORD	Type;
				DWORD retCode;

				retCode=RegEnumValue(key,i,
					achKey,
					&cbName,
					NULL,
					&Type,
					NULL,
					NULL);

				if (retCode != ERROR_SUCCESS) 
				{
					 bOK=FALSE;			
					 RegCloseKey(key);
					 return bOK;
				}
				else
				{
					char *chaine=NULL;
					chaine=(char*) malloc(sizeof(char)*255);
					strcpy(chaine,achKey);
					*ListKeys=chaine;
					*ListKeys++;
				}
			}

		RegCloseKey(key);
		
	}
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
HKEY GetHkeyrootFromString(char *string)
{
	HKEY hkey=NULL;

	if ( strcmp(string,"HKEY_CLASSES_ROOT") == 0 )	hkey = HKEY_CLASSES_ROOT;
	if ( strcmp(string,"HKEY_CURRENT_USER") == 0 )	hkey = HKEY_CURRENT_USER;
	if ( strcmp(string,"HKEY_LOCAL_MACHINE") == 0 )	hkey = HKEY_LOCAL_MACHINE;
	if ( strcmp(string,"HKEY_USERS") == 0 )			hkey = HKEY_USERS;
	if ( strcmp(string,"HKEY_DYN_DATA") == 0 )		hkey = HKEY_DYN_DATA;

	return hkey;
}
/*-----------------------------------------------------------------------------------*/
void ReadTextIni (LPTW lptw)
{
	/* Modification Restauration depuis la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Settings
	"Version" correspondant à la version de Scilab
	*/
	
	//extern void SetConsoleColorWindow(DWORD rgb);
	//extern void SetConsoleColorWindowText(DWORD rgb);
	//extern void SetConsoleColorHighlight(DWORD rgb);
	//extern void SetConsoleColorHighlightText(DWORD rgb);

	HKEY key;
	DWORD result,Length,size=4;
	char Clef[MAX_PATH];
	int Iconic,iconic;
	int SizeX;
	int SizeY;
  	int SysColors;
	//DWORD ColorWindow;
 //   DWORD ColorWindowText;
 //   DWORD ColorHighlight;
 //   DWORD ColorHighlightText;
  	int Toolbar;
  	int Language;
  	char TextFontName[MAX_PATH];
  	int TextFontSize;
  	RECT rect;
	
	Length=MAX_PATH;
	
	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",VERSION);
  	result=RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key);

	if ( RegQueryValueEx(key, "TextOriginX", 0, NULL, (LPBYTE)&rect.left, &size) !=  ERROR_SUCCESS )
	{
		lptw->Origin.x = CW_USEDEFAULT;
	}
	else
	{
		lptw->Origin.x = rect.left;
	}
	
  	if ( RegQueryValueEx(key, "TextOriginY", 0, NULL, (LPBYTE)&rect.top, &size) !=  ERROR_SUCCESS )
  	{
  		lptw->Origin.y = CW_USEDEFAULT;
	}
	else
	{
		lptw->Origin.y = rect.top;
	}
  	
  	if ( RegQueryValueEx(key, "TextSizeX", 0, NULL, (LPBYTE)&SizeX, &size) !=  ERROR_SUCCESS )
  	{
  		lptw->Size.x = CW_USEDEFAULT;
	}
	else
	{
		lptw->Size.x = SizeX;
	}
	
  	if ( RegQueryValueEx(key, "TextSizeY", 0, NULL, (LPBYTE)&SizeY, &size) !=  ERROR_SUCCESS )
  	{
  		lptw->Size.y = CW_USEDEFAULT;
	}
	else
	{
		lptw->Size.y = SizeY;
	}
  	
  	if ( RegQueryValueEx(key, "TextMinimized", 0, NULL, (LPBYTE)&Iconic, &size) !=  ERROR_SUCCESS )
  	{
  			iconic = 0;
	}
	else
	{
			iconic = Iconic;
	}
	
  	if ( RegQueryValueEx(key, "TextFontName", 0, NULL,(LPBYTE)TextFontName,&Length) !=  ERROR_SUCCESS )
  	{
  		strcpy (lptw->fontname, TEXTFONTNAME);
  	}
  	else
  	{
  		
  		strcpy (lptw->fontname, TextFontName);
  	}
  	
  	if ( RegQueryValueEx(key, "TextFontSize", 0, NULL, (LPBYTE)&TextFontSize, &size) !=  ERROR_SUCCESS )
  	{
  		lptw->fontsize = TEXTFONTSIZE;
	}
	else
	{
		lptw->fontsize = TextFontSize;
	}
	
  	if ( RegQueryValueEx(key, "SysColors", 0, NULL, (LPBYTE)&SysColors, &size) !=  ERROR_SUCCESS )
	{
		lptw->bSysColors = 1;
	}
	else
	{
		//lptw->bSysColors = SysColors;
		// Pour le moment en attendant l'ajout des boites de dialogues ...
		lptw->bSysColors = 1;
	}
	
	//if ( RegQueryValueEx(key, "ColorWindow", 0, NULL, (LPBYTE)&ColorWindow, &size) !=  ERROR_SUCCESS )
	//{
	//	SetConsoleColorWindow(GetSysColor (COLOR_WINDOW));
	//}
	//else
	//{
	//	SetConsoleColorWindow(ColorWindow);
	//}

	//if ( RegQueryValueEx(key, "ColorWindowText", 0, NULL, (LPBYTE)&ColorWindowText, &size) !=  ERROR_SUCCESS )
	//{
	//	SetConsoleColorWindowText(GetSysColor (COLOR_WINDOWTEXT));
	//}
	//else
	//{
	//	SetConsoleColorWindowText(ColorWindowText);
	//}

	//if ( RegQueryValueEx(key, "ColorHighlight", 0, NULL, (LPBYTE)&ColorHighlight, &size) !=  ERROR_SUCCESS )
	//{
	//	SetConsoleColorHighlight(GetSysColor (COLOR_HIGHLIGHT));
	//}
	//else
	//{
	//	SetConsoleColorHighlight(ColorHighlight);
	//}

	//if ( RegQueryValueEx(key, "ColorHighlightText", 0, NULL, (LPBYTE)&ColorHighlightText, &size) !=  ERROR_SUCCESS )
	//{
	//	SetConsoleColorHighlightText(GetSysColor (COLOR_HIGHLIGHTTEXT));
	//}
	//else
	//{
	//	SetConsoleColorHighlightText(ColorHighlightText);
	//}

	/*if (lptw->bSysColors)
	{
		SetConsoleColorWindow(GetSysColor (COLOR_WINDOW));
		SetConsoleColorWindowText(GetSysColor (COLOR_WINDOWTEXT));
		SetConsoleColorHighlight(GetSysColor (COLOR_HIGHLIGHT));
		SetConsoleColorHighlightText(GetSysColor (COLOR_HIGHLIGHTTEXT));
	}*/
	
	if ( RegQueryValueEx(key, "ToolBar", 0, NULL, (LPBYTE)&Toolbar, &size) !=  ERROR_SUCCESS )
  	{
		lptw->lpmw->ShowToolBar = TRUE;
	}
	else
	{
		lptw->lpmw->ShowToolBar = Toolbar;
	}
	
	if ( RegQueryValueEx(key, "Language", 0, NULL, (LPBYTE)&Language, &size) !=  ERROR_SUCCESS )
  	{
		lptw->lpmw->CodeLanguage = 0; /* English Default*/
	}
	else
	{
		lptw->lpmw->CodeLanguage = Language;
	}
	
		
	lptw->lpmw->LockToolBar=FALSE;

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	if (iconic) lptw->nCmdShow = SW_SHOWMINIMIZED;

}
/*-----------------------------------------------------------------------------------*/
/* Sauvegarde la position de la fenetre et la fonte utilisée dans la base de registre */
void WriteTextIni (LPTW lptw)
{
	/* Modification Sauvegarde dans la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Settings
	"Version" correspondant à la version de Scilab
	Sauvegarde dans HKEY_CURRENT_USER car données dépendant de l'utilisateur
	*/
	/*extern DWORD GetConsoleColorWindow(void);
	extern DWORD GetConsoleColorWindowText(void);
	extern DWORD GetConsoleColorHighlight(void);
	extern DWORD GetConsoleColorHighlightText(void);*/

	HKEY key;
	DWORD result,dwsize=4;
	RECT rect;
	int iconic;
	char Clef[MAX_PATH];
	int SizeX;
	int SizeY;
  	int SysColors;
	//DWORD ColorWindow;
 //   DWORD ColorWindowText;
 //   DWORD ColorHighlight;
 //   DWORD ColorHighlightText;
  	char TextFontName[MAX_PATH];
  	int TextFontSize;
	BOOL ShowButtons;
	int LanguageCode;

  	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",VERSION);  	
  	RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result);
  	iconic = IsIconic (lptw->hWndParent);
  	if (iconic) ShowWindow (lptw->hWndParent, SW_SHOWNORMAL);
  	
  	GetWindowRect (lptw->hWndParent, &rect);
  	RegSetValueEx(key, "TextOriginX", 0, REG_DWORD, (LPBYTE)&rect.left, dwsize);
  	RegSetValueEx(key, "TextOriginY", 0, REG_DWORD, (LPBYTE)&rect.top, dwsize);
  		
  	SizeX=rect.right - rect.left;
  	SizeY=rect.bottom - rect.top;
  	RegSetValueEx(key, "TextSizeX", 0, REG_DWORD, (LPBYTE)&SizeX, dwsize);
  	RegSetValueEx(key, "TextSizeY", 0, REG_DWORD, (LPBYTE)&SizeY, dwsize);
  	  	
  	RegSetValueEx(key, "TextMinimized", 0, REG_DWORD, (LPBYTE)&iconic, dwsize);
 	
  	strcpy(TextFontName,lptw->fontname);
  	RegSetValueEx(key, "TextFontName", 0, REG_SZ, (LPBYTE)TextFontName, strlen(TextFontName)+1);
  	  	
  	TextFontSize=lptw->fontsize;
  	RegSetValueEx(key, "TextFontSize", 0, REG_DWORD, (LPBYTE)&TextFontSize, dwsize);
  	
  	SysColors=lptw->bSysColors;
	RegSetValueEx(key, "SysColors", 0, REG_DWORD, (LPBYTE)&SysColors, dwsize);

	//ColorWindow=GetConsoleColorWindow();
	//RegSetValueEx(key, "ColorWindow", 0, REG_DWORD, (LPBYTE)&ColorWindow, dwsize);

	//ColorWindowText=GetConsoleColorWindowText();
	//RegSetValueEx(key, "ColorWindowText", 0, REG_DWORD, (LPBYTE)&ColorWindowText, dwsize);

 //   ColorHighlight=GetConsoleColorHighlight();
	//RegSetValueEx(key, "ColorHighlight", 0, REG_DWORD, (LPBYTE)&ColorHighlight, dwsize);

 //   ColorHighlightText=GetConsoleColorHighlightText();
	//RegSetValueEx(key, "ColorHighlightText", 0, REG_DWORD, (LPBYTE)&ColorHighlightText, dwsize);
	//
	ShowButtons=lptw->lpmw->ShowToolBar;
	RegSetValueEx(key, "ToolBar", 0, REG_DWORD, (LPBYTE)&ShowButtons, dwsize);
	
	LanguageCode=lptw->lpmw->CodeLanguage;
	RegSetValueEx(key, "Language", 0, REG_DWORD, (LPBYTE)&LanguageCode, dwsize);
	
	RegCloseKey(key);
	
	if (iconic) ShowWindow (lptw->hWndParent, SW_SHOWMINIMIZED);
  
}
/*-----------------------------------------------------------------------------------*/
void WriteGraphIni (struct BCG *ScilabGC)
{
	/* Modification Sauvegarde dans la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Graph Settings
	"Version" correspondant à la version de Scilab
	Sauvegarde dans HKEY_CURRENT_USER car données dépendant de l'utilisateur
	*/

	HKEY key;
	DWORD result,dwsize=4;
	char Clef[MAX_PATH];

	RECT rect;
	long GraphSizeX,GraphSizeY;
	int iconic;

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Graph Settings",VERSION);  	
  	RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result);
  	iconic = IsIconic (ScilabGC->hWndParent);
	if (iconic) ShowWindow (ScilabGC->hWndParent, SW_SHOWNORMAL);

	GetWindowRect (ScilabGC->hWndParent, &rect);
	RegSetValueEx(key, "GraphOriginX", 0, REG_DWORD, (LPBYTE)&rect.left, dwsize);
  	RegSetValueEx(key, "GraphOriginY", 0, REG_DWORD, (LPBYTE)&rect.top, dwsize);

	GraphSizeX=rect.right - rect.left;
	GraphSizeY=rect.bottom - rect.top;
	RegSetValueEx(key, "GraphSizeX", 0, REG_DWORD,(LPBYTE)&GraphSizeX, dwsize);
  	RegSetValueEx(key, "GraphSizeY", 0, REG_DWORD,(LPBYTE)&GraphSizeY, dwsize);
	RegSetValueEx(key, "ToolBar", 0, REG_DWORD, (LPBYTE)&ScilabGC->lpmw.ShowToolBar, dwsize);

	RegCloseKey(key);
	if (iconic) ShowWindow (ScilabGC->hWndParent, SW_SHOWMINIMIZED);
  
}
/*-----------------------------------------------------------------------------------*/
void ReadGraphIni (struct BCG *ScilabGC)
{
	/* Modification Restauration depuis la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Graph Settings
	"Version" correspondant à la version de Scilab
	*/
	HKEY key;
	DWORD result,size=4;
	long GraphSizeX,GraphSizeY;
	char Clef[MAX_PATH];
	int Toolbar;
	
	RECT rect;

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Graph Settings",VERSION);
  	result=RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key);

	if ( RegQueryValueEx(key, "GraphOriginX", 0, NULL, (LPBYTE)&rect.left, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Origin.x = CW_USEDEFAULT;
	}
	else
	{
		ScilabGC->lpgw->Origin.x = rect.left;
	}

	if ( RegQueryValueEx(key, "GraphOriginY", 0, NULL, (LPBYTE)&rect.top, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Origin.y = CW_USEDEFAULT;
	}
	else
	{
		ScilabGC->lpgw->Origin.y = rect.top;
	}

	if ( RegQueryValueEx(key, "GraphSizeX", 0, NULL, (LPBYTE)&GraphSizeX, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Size.x = CW_USEDEFAULT;
	}
	else
	{
	ScilabGC->lpgw->Size.x = GraphSizeX;
	}

	if ( RegQueryValueEx(key, "GraphSizeY", 0, NULL, (LPBYTE)&GraphSizeY, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Size.y = CW_USEDEFAULT;
	}
	else
	{
		ScilabGC->lpgw->Size.y = GraphSizeY;
	}

	if ( RegQueryValueEx(key, "ToolBar", 0, NULL, (LPBYTE)&Toolbar, &size) !=  ERROR_SUCCESS )
  	{
		ScilabGC->lpmw.ShowToolBar = TRUE;
	}
	else
	{
		ScilabGC->lpmw.ShowToolBar  = Toolbar;
	}

    SetDefaultShowToolBar(ScilabGC->lpmw.ShowToolBar);
	ScilabGC->lpmw.LockToolBar=FALSE;

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);
}
/*-----------------------------------------------------------------------------------*/