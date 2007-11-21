/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __STRIPBLANKS_H__
#define __STRIPBLANKS_H__

#include "BOOL.h"
/**
* stripblanks 
* remove blank or tab characters in a string matrix
* param[in] InputStrings
* param[out] OutputStrings
* param[in] InputStringsDim size of InputStrings 
* param[in] BOOLEAN remove TAB if TRUE
*/


void stripblanks(char **InputStrings,char **OutputStrings,int InputStringsDim,BOOL withTAB);

#endif /* __STRIPBLANKS_H__ */
/*-----------------------------------------------------------------------------------*/
