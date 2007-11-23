/*--------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Francois VOGEL  sciprint_full function */
/*--------------------------------------------------------------------------*/ 
#ifndef __SCIPRINT_FULL_H__
#define __SCIPRINT_FULL_H__

#include <stdarg.h>

/**
* sciprint geared towards long strings (>MAXPRINTF) 
* the long string is splitted in elements of length 
* equal to the number of columns from lines()                   
* @param fmt Format of the format string
* @param ...
*/
void sciprint_full(char *fmt,...) ;

#endif /* __SCIPRINT_FULL_H__ */
/*--------------------------------------------------------------------------*/ 
