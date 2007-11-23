/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __SCIPRINT_H__
#define __SCIPRINT_H__

#include <stdarg.h>

/**
 * Print
 * @param fmt Format of the format string
 * @param ...
 */
void sciprint(char *fmt,...) ;

/*
* Print
* works as sciprint
* difference is that the argument list is a pointer to a list of arguments
* @param fmt Format of the format string
* @param [in] args
*/
void scivprint(char *fmt,va_list args);

/**
 * Print
 * works as sciprint but with an added first argument which is 
 * ignored (used in do_printf) 
 * @see sciprint
 * @param iv
 * @param fmt Format of the format string
 * @param ...
 * @return the number of characters printed
 */
int sciprint2(int iv,char *fmt,...);

#endif /* __SCIPRINT_H__ */
/*--------------------------------------------------------------------------*/ 
