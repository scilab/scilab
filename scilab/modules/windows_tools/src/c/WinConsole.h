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

#endif /* __WINCONSOLE_H__ */
/*-----------------------------------------------------------------------------------*/ 
