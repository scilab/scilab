/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETVARIABLESNAME_H__
#define __GETVARIABLESNAME_H__

#include "machine.h" /* BOOL */

/**
* get names of all variables (global+local) on stack
* duplicated names (global+local) are removed
* @param[out]size of array
* @param[in] BOOL to sort output
* @return a array of strings
*/
char **getVariablesName(int *sizearray, BOOL sorted);

/**
* get names of all local variables on stack
* @param[out]size of array
* @param[in] BOOL to sort output
* @return a array of strings
*/
char **getLocalVariablesName(int *sizearray,BOOL sorted);

/**
* get names of all global variables on stack
* @param[out]size of array
* @param[in] BOOL to sort output
* @return a array of strings
*/
char **getGlobalVariablesName(int *sizearray, BOOL sorted);

#endif /*__GETVARIABLESNAME_H__ */
/*-----------------------------------------------------------------------------------*/ 
