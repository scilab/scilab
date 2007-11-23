/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Ledru Sylvestre */
/* @author Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __SCIPRINT_ND_H__
#define __SCIPRINT_ND_H__

#include <stdarg.h>

/**
* Print without diary
* @param fmt Format of the format string
* @param ...
*/
void sciprint_nd(char *fmt,...);

/**
* Print without diary
* works as sciprint_nd
* difference is that the argument list is a pointer to a list of arguments
* @param fmt Format of the format string
* @param ...
*/
void scivprint_nd(char *fmt,va_list args);

#endif /* __SCIPRINT_ND_H__ */
/*--------------------------------------------------------------------------*/
