/* Allan CORNET */
/* INRIA 2005 */

#include "WinConsole.h"
#include "..\version.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
/*-----------------------------------------------------------------------------------*/
static CONSOLE_SCREEN_BUFFER_INFO csbiInfoSave;
char ScilexConsoleName[MAX_PATH];
static BOOL WindowMode;
static int Windows_Console_State;/* 0 Hide 1 Show */
/*-----------------------------------------------------------------------------------*/
typedef  HWND (WINAPI * GetConsoleWindowPROC) (void);
typedef  UINT (WINAPI * GetWindowModuleFileNamePROC) (HWND,LPTSTR,UINT);
/*-----------------------------------------------------------------------------------*/
void RenameConsole(void)
{
	HMENU hmenuConsole = NULL;
	char CurrentConsoleName[MAX_PATH];
	char CurrentConsoleNameTmp[MAX_PATH];

	GetConsoleTitle(CurrentConsoleName,MAX_PATH);
	strncpy(CurrentConsoleNameTmp,CurrentConsoleName,strlen(NameConsole));
	CurrentConsoleNameTmp[strlen(NameConsole)]='\0';

	if ( strcmp(CurrentConsoleNameTmp,NameConsole) != 0)	 
	{
		wsprintf(ScilexConsoleName,"%s %s",NameConsole,VERSION);
		SetConsoleTitle(ScilexConsoleName);
	}

	// Desactive croix dans la console
	hmenuConsole=GetSystemMenu(ScilabGetConsoleWindow(), FALSE);
	DeleteMenu(hmenuConsole, SC_CLOSE, MF_BYCOMMAND);

}
/*-----------------------------------------------------------------------------------*/
void CreateScilabConsole(int ShowBanner)
{
	HMENU hmenuConsole = NULL;
	int Current_Number_of_Scilex=-1; 

	Windows_Console_State=0; /* Console DOS Cachée par défaut */
	AllocConsole();

	Current_Number_of_Scilex=FindFreeScilexNumber();
	wsprintf(ScilexConsoleName,"%s %s (%d)",NameConsole,VERSION,Current_Number_of_Scilex);
	SetConsoleTitle(ScilexConsoleName);

	CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    freopen("CONOUT$", "wb", stdout); /* redirect stdout --> CONOUT$*/
	freopen("CONOUT$", "wb", stderr); /* redirect stderr --> CONOUT$*/
	
	if (ShowBanner)
	{
		char line[80];

		strcpy(line,"        ___________________________________________\r\n");
		printf(line);
	    wsprintf(line,"                         %s\r\n\n",VERSION);
		printf(line);
		strcpy(line,"                  Copyright (c) 1989-2007          \r\n");
		printf(line);
		strcpy(line,"              Consortium Scilab (INRIA, ENPC)      \r\n");
		printf(line);
    	strcpy(line,"        ___________________________________________\r\n\r\n");
		printf(line);
	}

	// Desactive croix dans la console
	hmenuConsole=GetSystemMenu(ScilabGetConsoleWindow(), FALSE);
	DeleteMenu(hmenuConsole, SC_CLOSE, MF_BYCOMMAND);

	HideScilex(); /* Cache la fenetre Console */
}
/*-----------------------------------------------------------------------------------*/
void CloseScilabConsole(void)
{
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
}
/*-----------------------------------------------------------------------------------*/
void UpdateConsoleColors(void)
{
	  HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	  COORD Coord;
	  DWORD cWritten;

	  Coord.X=0;
	  Coord.Y=0;
	 
	  FillConsoleOutputAttribute( hConsole,
								  BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED|BACKGROUND_INTENSITY,
								  csbiInfoSave.dwSize.X*csbiInfoSave.dwSize.Y,
								  Coord,
								  &cWritten); 


	  SetConsoleTextAttribute(hConsole,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED|BACKGROUND_INTENSITY);
 
}
/*-----------------------------------------------------------------------------------*/
void SaveConsoleColors(void)
{
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfoSave); 
}
/*-----------------------------------------------------------------------------------*/
void RestoreConsoleColors(void)
{
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Coord;
	DWORD cWritten;

	Coord.X=0;
	Coord.Y=0;

	FillConsoleOutputAttribute( hConsole,
								csbiInfoSave.wAttributes,
								csbiInfoSave.dwSize.X*csbiInfoSave.dwSize.Y,
								Coord,
								&cWritten);
	SetConsoleTextAttribute(hConsole,csbiInfoSave.wAttributes);
}
/*-----------------------------------------------------------------------------------*/
BOOL IsWindowInterface(void)
{
	return WindowMode;
}
/*-----------------------------------------------------------------------------------*/
/* Retourne un numéro valide pour nommer les fenetres associées à ce process */
int FindFreeScilexNumber(void)
{
	HWND hScilexN=NULL;
	int Number_of_Scilex=0;
	char NameScilex[MAX_PATH];

	wsprintf(NameScilex,"%s (%d)",VERSION,Number_of_Scilex);
	hScilexN=FindWindow(NULL,NameScilex);
	while ( hScilexN )
	{
		UINT NbChar=0;
		char FileName[MAX_PATH+1];
		strcpy(FileName,"");

		NbChar=ScilabGetWindowModuleFileName(hScilexN,FileName,MAX_PATH);
		
		if (NbChar>0)
		{
			Number_of_Scilex++;
			wsprintf(NameScilex,"%s (%d)",VERSION,Number_of_Scilex);
			hScilexN=FindWindow(NULL,NameScilex);
		}
		else
		{
			hScilexN=NULL;
		}
	}

	return Number_of_Scilex;
}
/*-----------------------------------------------------------------------------------*/
/*Cache la fenetre Scilex(x) de ce processus */
void HideScilex(void)
{
	HWND hScilex=NULL;
	hScilex=ScilabGetConsoleWindow();
	if (hScilex)
	{
		ShowWindow(hScilex,SW_HIDE);	
	}
}
/*-----------------------------------------------------------------------------------*/
/*Montre la fenetre Scilex(x) de ce processus */
void ShowScilex(void)
{
  HWND hScilex=NULL;
  hScilex=ScilabGetConsoleWindow();
  if (hScilex)
  {
  	ShowWindow(hScilex,SW_SHOWNOACTIVATE);
  }
}
/*-----------------------------------------------------------------------------------*/
void SwitchConsole(void)
{
	switch (GetConsoleState())
	{
		/* La fenetre etait cachée , on la restaure */
		case 0:
		{
			ShowScilex();
			SetConsoleState(1);
			SetConsoleTitle(ScilexConsoleName);
		}
		break;
		/* La fenetre etait apparente , on la cache */ 
		case 1:
		{
			HideScilex();
			SetConsoleState(0);
			SetConsoleTitle(ScilexConsoleName);
		}
		break;
	}
}
/*-----------------------------------------------------------------------------------*/
void SetWindowMode(BOOL ON)
{
	  WindowMode=ON;
}
/*-----------------------------------------------------------------------------------*/
int GetConsoleState(void)
{
	return Windows_Console_State;
}
/*-----------------------------------------------------------------------------------*/
void SetConsoleState(int state)
{
	Windows_Console_State=state;
}
/*-----------------------------------------------------------------------------------*/
HWND ScilabGetConsoleWindow(void)
{
	HWND hWndReturn=NULL;
	HINSTANCE Kernel32Dll = LoadLibrary ("kernel32"); 

	if ( Kernel32Dll ) 
		{ 
			GetConsoleWindowPROC myGetConsoleWindow = (GetConsoleWindowPROC) GetProcAddress(Kernel32Dll,"GetConsoleWindow"); 

			if ( myGetConsoleWindow ) 
			{ 
				// 2k AND more
				hWndReturn = (myGetConsoleWindow)(); 
			} 
			else 
			{ 
				// 98
				hWndReturn = FindWindow(NULL,ScilexConsoleName);
			} 
		}
	else
	{
		hWndReturn = FindWindow(NULL,ScilexConsoleName);
	}

	FreeLibrary( Kernel32Dll ); 

	return hWndReturn;
}
/*-----------------------------------------------------------------------------------*/
UINT ScilabGetWindowModuleFileName(HWND hwnd,LPTSTR lpszFileName,UINT cchFileNameMax)
{
	UINT UintReturn=0;
	HINSTANCE User32Dll = LoadLibrary ("user32"); 

	if ( User32Dll ) 
	{ 
		GetWindowModuleFileNamePROC myGetWindowModuleFileName = (GetWindowModuleFileNamePROC) GetProcAddress(User32Dll,"GetWindowModuleFileName"); 

		if ( myGetWindowModuleFileName ) 
		{ 
			UintReturn=myGetWindowModuleFileName(hwnd,lpszFileName,cchFileNameMax);
		} 
		else 
		{ 
			UintReturn=-1;
		} 
	}
	else
	{
		UintReturn=-1;
	}

	FreeLibrary( User32Dll ); 

	return UintReturn;
}
/*-----------------------------------------------------------------------------------*/
