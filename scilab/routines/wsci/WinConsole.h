/* Allan CORNET */
/* INRIA 2005 */
#ifndef __WINCONSOLE_H__
#define __WINCONSOLE_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <Winuser.h>

#define NameConsole "Console"

void RenameConsole(void);
void CreateScilabConsole(BOOL ShowBanner);
void CloseScilabConsole(void);

void UpdateConsoleColors(void);
void SaveConsoleColors(void);
void RestoreConsoleColors(void);

void SetWindowMode(BOOL ON);
BOOL IsWindowInterface();

/* Retourne un numéro valide pour nommer les fenetres associées à ce process */
int FindFreeScilexNumber(void);

void HideScilex(void);
void ShowScilex(void);
void SwitchConsole(void);
int GetConsoleState(void); /* Show 1 or Hide 0 */
void SetConsoleState(int state);


#endif /* __WINCONSOLE_H__ */