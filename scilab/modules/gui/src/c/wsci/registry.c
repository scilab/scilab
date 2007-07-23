/*-----------------------------------------------------------------------------------*/
/* Allan CORNET */
/* Scilab, INRIA 2004 */
/*-----------------------------------------------------------------------------------*/
#include "registry.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "win_mem_alloc.h" /* MALLOC */
#include "wresource.h"

/*-----------------------------------------------------------------------------------*/
extern void SetDefaultShowToolBar(BOOL valShowToolBar);
extern BOOL SetIhmTextColor(int R,int G,int B,BOOL Refresh);
extern BOOL SetIhmTextBackgroundColor(int R,int G,int B,BOOL Refresh);
extern DWORD GetIhmTextColor(void);
extern int GetLanguageCodeInScilabDotStar(void);
DWORD GetIhmTextBackgroundColor(void);
/*-----------------------------------------------------------------------------------*/
void ReadRegistryTxt (LPTW lptw)
{
	/* Modification Restauration depuis la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Settings
	"Version" correspondant à la version de Scilab
	*/
	HKEY key;
	DWORD result,Length,size=4;
	char Clef[PATH_MAX];
	int Iconic,iconic;
	int SizeX;
	int SizeY;
  	int SysColors;
	DWORD TextColor;
    DWORD BackgroundColor;
  	int Toolbar;
// 	int Language;
  	char TextFontName[PATH_MAX];
  	int TextFontSize;
  	RECT rect;
	
	Length=PATH_MAX;
	
	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",SCI_VERSION_STRING);
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
		lptw->bSysColors = SysColors;
	}
	
	if (lptw->bSysColors == 0)
	{
		if ( RegQueryValueEx(key, "TextColor", 0, NULL, (LPBYTE)&TextColor, &size) !=  ERROR_SUCCESS )
		{
			int R,G,B;
			R=GetRValue(GetSysColor (COLOR_WINDOW));
			G=GetGValue(GetSysColor (COLOR_WINDOW));
			B=GetBValue(GetSysColor (COLOR_WINDOW));
			
			SetIhmTextColor(R,G,B,FALSE);
		}
		else
		{
			int R,G,B;
			R=GetRValue(TextColor);
			G=GetGValue(TextColor);
			B=GetBValue(TextColor);

			SetIhmTextColor(R,G,B,FALSE);
		}

		if ( RegQueryValueEx(key, "BackgroundColor", 0, NULL, (LPBYTE)&BackgroundColor, &size) !=  ERROR_SUCCESS )
		{
			int R,G,B;
			R=GetRValue(GetSysColor (COLOR_WINDOW));
			G=GetGValue(GetSysColor (COLOR_WINDOW));
			B=GetBValue(GetSysColor (COLOR_WINDOW));

			SetIhmTextBackgroundColor(R,G,B,FALSE);
		}
		else
		{
			int R,G,B;
			R=GetRValue(BackgroundColor);
			G=GetGValue(BackgroundColor);
			B=GetBValue(BackgroundColor);

			SetIhmTextBackgroundColor(R,G,B,FALSE);
		}
	}
	
	if ( RegQueryValueEx(key, "ToolBar", 0, NULL, (LPBYTE)&Toolbar, &size) !=  ERROR_SUCCESS )
  	{
		lptw->lpmw->ShowToolBar = TRUE;
	}
	else
	{
		lptw->lpmw->ShowToolBar = Toolbar;
	}

	//if ( RegQueryValueEx(key, "Language", 0, NULL, (LPBYTE)&Language, &size) !=  ERROR_SUCCESS )
 // 	{
	//	int CodeLanguageInScilabDotStar=GetLanguageCodeInScilabDotStar();

	//	if (CodeLanguageInScilabDotStar != -1)
	//	{
	//		lptw->lpmw->CodeLanguage = CodeLanguageInScilabDotStar;
	//	}
	//	else lptw->lpmw->CodeLanguage = 0; /* English Default*/
	//}
	//else
	//{
	//	int CodeLanguageInScilabDotStar=GetLanguageCodeInScilabDotStar();

	//	if (Language != CodeLanguageInScilabDotStar)
	//	{
	//		lptw->lpmw->CodeLanguage = CodeLanguageInScilabDotStar; 
	//	}
	//	else
	//	if  ( (Language == 0) || (Language == 1) )	lptw->lpmw->CodeLanguage = Language;
	//	else lptw->lpmw->CodeLanguage = 0; /* English Default*/
	//}
		
	lptw->lpmw->LockToolBar=FALSE;

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	if (iconic) lptw->nCmdShow = SW_SHOWMINIMIZED;

}
/*-----------------------------------------------------------------------------------*/
/* Sauvegarde la position de la fenetre et la fonte utilisée dans la base de registre */
void WriteRegistryTxt (LPTW lptw)
{
	/* Modification Sauvegarde dans la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Settings
	"Version" correspondant à la version de Scilab
	Sauvegarde dans HKEY_CURRENT_USER car données dépendant de l'utilisateur
	*/

	HKEY key = NULL;
	DWORD result,dwsize=4;
	RECT rect;
	int iconic;
	char Clef[PATH_MAX];
	int SizeX;
	int SizeY;
  	int SysColors;
	DWORD TextColor;
    DWORD BackgroundColor;
  	char TextFontName[PATH_MAX];
  	int TextFontSize;
	BOOL ShowButtons;
//	int LanguageCode;
	LONG TstRegCreateKeyEx=0;

  	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",SCI_VERSION_STRING);  	
  	TstRegCreateKeyEx = RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result);
	if (TstRegCreateKeyEx == ERROR_SUCCESS)
	{
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

		TextColor=GetIhmTextColor();
		RegSetValueEx(key, "TextColor", 0, REG_DWORD, (LPBYTE)&TextColor, dwsize);

		BackgroundColor= GetIhmTextBackgroundColor();
		RegSetValueEx(key, "BackgroundColor", 0, REG_DWORD, (LPBYTE)&BackgroundColor, dwsize);

		ShowButtons=lptw->lpmw->ShowToolBar;
		RegSetValueEx(key, "ToolBar", 0, REG_DWORD, (LPBYTE)&ShowButtons, dwsize);

		/*LanguageCode=lptw->lpmw->CodeLanguage;
		RegSetValueEx(key, "Language", 0, REG_DWORD, (LPBYTE)&LanguageCode, dwsize);
		*/

		RegCloseKey(key);
	}
	
	if (iconic) ShowWindow (lptw->hWndParent, SW_SHOWMINIMIZED);
  
}
/*-----------------------------------------------------------------------------------*/
void WriteRegistryGraph (struct BCG *ScilabGC)
{
	/* Modification Sauvegarde dans la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Graph Settings
	"Version" correspondant à la version de Scilab
	Sauvegarde dans HKEY_CURRENT_USER car données dépendant de l'utilisateur
	*/

	HKEY key;
	DWORD result,dwsize=4;
	char Clef[PATH_MAX];

	RECT rect;
	long GraphSizeX,GraphSizeY;
	int iconic;
	LONG TstRegCreateKeyEx=0;


	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Graph Settings",SCI_VERSION_STRING);  	
  	TstRegCreateKeyEx=RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result);
	if (TstRegCreateKeyEx == ERROR_SUCCESS)
	{
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
	}
  	
	if (iconic) ShowWindow (ScilabGC->hWndParent, SW_SHOWMINIMIZED);
  
}
/*-----------------------------------------------------------------------------------*/
void ReadRegistryGraph (struct BCG *ScilabGC)
{
	/* Modification Restauration depuis la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Graph Settings
	"Version" correspondant à la version de Scilab
	*/
	HKEY key;
	DWORD result,size=4;
	long GraphSizeX,GraphSizeY;
	char Clef[PATH_MAX];
	int Toolbar;
	
	RECT rect;

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Graph Settings",SCI_VERSION_STRING);
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
