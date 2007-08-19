/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

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
* Rename Scilab Console  with correct name
*/
void RenameConsole(void);

/**
* Create Scilab Console
*/
void CreateScilabConsole(BOOL ShowBanner);

/**
* Find first free scilex number
* @return a value
*/
int FindFreeScilexNumber(void);

/**
* Close Scilab Console
*/
void CloseScilabConsole(void);

/**
* get scilex console name
* @return a name
*/
char *getScilexConsoleName(void);

#endif /* __CONSOLE_H__ */
/*-----------------------------------------------------------------------------------*/ 
