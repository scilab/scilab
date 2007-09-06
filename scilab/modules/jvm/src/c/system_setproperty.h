/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SYSTEM_SETPROPERTY_H__
#define __SYSTEM_SETPROPERTY_H__

#include "machine.h" /* BOOL */

/**
* set a system property
* @param property name
* @param value
* @return previous value or NULL
*/
char * system_setproperty(char *property,char *value);

#endif /* __SYSTEM_SETPROPERTY_H__ */
/*-----------------------------------------------------------------------------------*/ 
