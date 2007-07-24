/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIHOME_H__
#define __SCIHOME_H__

#include "machine.h"

/**
* Set SCIHOME environment variable
* @return TRUE or FALSE
*/
BOOL setSCIHOME(void);

/**
* get SCIHOME  environment variable
* @return a string SCIHOME
*/
char *getSCIHOME(void);

#endif /* __SCIHOME_H__ */
/*-----------------------------------------------------------------------------------*/ 
