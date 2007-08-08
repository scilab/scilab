/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __WINCONSOLE_H__
#define __WINCONSOLE_H__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 
/* Theses functions are used to manipulate console 'dos' added to GUI */
/* only for windows */
/*-----------------------------------------------------------------------------------*/ 
/**
* Rename Scilab Console  with correct name
*/
void RenameConsole(void);

/**
* Create Scilab Console
*/
void CreateScilabConsole(BOOL ShowBanner);

/**
* Close Scilab Console
*/
void CloseScilabConsole(void);

/**
* Update Colors of console
*/
void UpdateConsoleColors(void);

/**
* Save colors before  creation
*/
void SaveConsoleColors(void);

/**
* Restore colors of console
*/
void RestoreConsoleColors(void);

/**
* Check if is a GUI interface
*@return TRUE or FALSE
*/
BOOL IsWindowInterface();

/**
* Find first free scilex number
* @return a value
*/
int FindFreeScilexNumber(void);

/**
* hide scilex console
*/
void HideScilex(void);

/**
* show scilex console
*/
void ShowScilex(void);

/**
* switch between hide and show
*/
void SwitchConsole(void);

/**
* Get console state
*@return state 0 hide , 1 show
*/
int GetConsoleState(void);

/**
* Set console state
* @param[in] 0 hide , 1 show
*/
void SetConsoleState(int state);

/**
* get scilex console name
* @return a name
*/
char *getScilexConsoleName(void);

#endif /* __WINCONSOLE_H__ */
/*-----------------------------------------------------------------------------------*/ 
