/*----------------------------------------------------------------------------*/
/* INRIA 2007 */
/*----------------------------------------------------------------------------*/
#ifndef __STRCATFUNCTION_H__
#define __STRCATFUNCTION_H__
#include "machine.h"
#define STAR '*'
#define COL 'c'
#define ROW 'r'
/*------------------------------------------------------------------------*/
/**
* strcatfunction
* description To connect the two strings
* @param[in] Input_String_One
* @param[in] Input_String_Two
* @param[in] Num_of_Input
* @param[out] Output_StringMatrix
* @return bool 
*/
/*------------------------------------------------------------------------*/
BOOL strcatfunction(char **Input_String_One,char **Input_String_Two,int Num_of_Input,char **Output_StringMatrix);

#endif /* __STRCATFUNCTION_H__ */
/*----------------------------------------------------------------------------*/
