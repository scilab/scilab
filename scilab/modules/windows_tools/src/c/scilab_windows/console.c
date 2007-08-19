/*-----------------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#define  _WIN32_WINNT 0x0500 // GetConsoleWindow
#include <Windows.h>
#include <shlwapi.h>
#include <stdio.h>
#pragma comment(lib, "shlwapi.lib")
#include "console.h"
#include "version.h"
#include "MALLOC.h"
#include "../WinConsole.h"
/*-----------------------------------------------------------------------------------*/
#define NameConsole "Console" 
/*-----------------------------------------------------------------------------------*/
static CONSOLE_SCREEN_BUFFER_INFO csbiInfoSave;
static char ScilexConsoleName[MAX_PATH];
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
void RenameConsole(void)
{
	HWND hScilex = NULL;
	char CurrentConsoleName[MAX_PATH];
	char CurrentConsoleNameTmp[MAX_PATH];

	GetConsoleTitle(CurrentConsoleName,MAX_PATH);
	strncpy(CurrentConsoleNameTmp,CurrentConsoleName,strlen(NameConsole));
	CurrentConsoleNameTmp[strlen(NameConsole)]='\0';

	if ( strcmp(CurrentConsoleNameTmp,NameConsole) != 0)	 
	{
		wsprintf(ScilexConsoleName,"%s %s",NameConsole,SCI_VERSION_STRING);
		SetConsoleTitle(ScilexConsoleName);
	}

	hScilex = GetConsoleWindow();
	if (hScilex)
	{
		HMENU hmenuConsole = NULL;
		// Desactive croix dans la console
		hmenuConsole=GetSystemMenu(hScilex, FALSE);
		DeleteMenu(hmenuConsole, SC_CLOSE, MF_BYCOMMAND);
	}
}
/*-----------------------------------------------------------------------------------*/
void CreateScilabConsole(int ShowBanner)
{

	HWND hScilex=NULL;

	int Current_Number_of_Scilex=-1; 

	SetConsoleState(0);  /* Console DOS Cachée par défaut */
	AllocConsole();

	Current_Number_of_Scilex=FindFreeScilexNumber();
	wsprintf(ScilexConsoleName,"%s %s (%d)",NameConsole,SCI_VERSION_STRING,Current_Number_of_Scilex);
	SetConsoleTitle(ScilexConsoleName);

	CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
	freopen("CONOUT$", "wb", stdout); /* redirect stdout --> CONOUT$*/
	freopen("CONOUT$", "wb", stderr); /* redirect stderr --> CONOUT$*/

	if (ShowBanner)
	{
		char line[80];

		strcpy(line,"        ___________________________________________\r\n");
		printf(line);
		wsprintf(line,"                         %s\r\n\n",SCI_VERSION_STRING);
		printf(line);
		strcpy(line,"                  Copyright (c) 1989-2007          \r\n");
		printf(line);
		strcpy(line,"              Consortium Scilab (INRIA, ENPC)      \r\n");
		printf(line);
		strcpy(line,"        ___________________________________________\r\n\r\n");
		printf(line);
	}

	hScilex = GetConsoleWindow();
	if (hScilex)
	{
		HMENU hmenuConsole = NULL;
		// Desactive croix dans la console
		hmenuConsole=GetSystemMenu(hScilex, FALSE);
		DeleteMenu(hmenuConsole, SC_CLOSE, MF_BYCOMMAND);

		/* Cache la fenetre Console */
		ShowWindow(hScilex,SW_HIDE); 
	}
}
/*-----------------------------------------------------------------------------------*/
/* Retourne un numéro valide pour nommer les fenetres associées à ce process */
int FindFreeScilexNumber(void)
{
	HWND hScilexN=NULL;
	int Number_of_Scilex=0;
	char NameScilex[MAX_PATH];

	wsprintf(NameScilex,"%s (%d)",SCI_VERSION_STRING,Number_of_Scilex);
	hScilexN=FindWindow(NULL,NameScilex);
	while ( hScilexN )
	{
		Number_of_Scilex++;
		wsprintf(NameScilex,"%s (%d)",SCI_VERSION_STRING,Number_of_Scilex);
		hScilexN = FindWindow(NULL,NameScilex);
	}
	return Number_of_Scilex;
}
/*-----------------------------------------------------------------------------------*/
void CloseScilabConsole(void)
{
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
}
/*-----------------------------------------------------------------------------------*/
char *getScilexConsoleName(void)
{
	char *retName = NULL;

	if (strlen(ScilexConsoleName) > 0)
	{
		retName = (char*)MALLOC(sizeof(char)*(strlen(ScilexConsoleName)+1));
		if (retName) strcpy(retName,ScilexConsoleName);
	}
	return retName;
}
/*-----------------------------------------------------------------------------------*/
