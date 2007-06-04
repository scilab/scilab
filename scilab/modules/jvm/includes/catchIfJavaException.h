/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre Ledru */
/*-----------------------------------------------------------------------------------*/
#ifndef __CATCHIFJAVAEXCEPTION_H__
#define __CATCHIFJAVAEXCEPTION_H__

#include "machine.h" /* BOOL */

/**
 * Check if there is an exception and print if it is the case
 * @param errorMsg the error message which will be displayed if an exception is catched 
 * @return 
 */
BOOL catchIfJavaException(char *errorMsg);

#endif /* __CATCHIFJAVAEXCEPTION_H__*/
/*-----------------------------------------------------------------------------------*/
