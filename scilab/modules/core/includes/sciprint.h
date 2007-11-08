/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/* Francois VOGEL , sciprint_full */
/*-----------------------------------------------------------------------------------*/
#ifndef __SCIPRINT_H__
#define __SCIPRINT_H__

#include <stdarg.h>

/**
 * Print
 * @param fmt Format of the format string
 * @param ...
 */
void sciprint(char *fmt,...) ;

/**
 * Print
 * works like as sciprint but with an added first argument which is 
 * ignored (used in do_printf) 
 * @see sciprint
 * @param iv
 * @param fmt Format of the format string
 * @param ...
 * @return the number of characters printed
 */
int sciprint2(int iv,char *fmt,...);


#endif /* __SCIPRINT_H__ */
/*-----------------------------------------------------------------------------------*/ 
