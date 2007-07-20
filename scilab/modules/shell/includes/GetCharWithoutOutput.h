#ifndef __GETCHARWITHOUTOUTPUT_H__
#define __GETCHARWITHOUTOUTPUT_H__
/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
/**
* Make the console wait for a user input character (used for [more (y or n) ?])
* @return code for the character entered
*/
int GetCharWithoutOutput(void);
/*-----------------------------------------------------------------------------------*/
#endif /* __GETCHARWITHOUTOUTPUT_H__ */

