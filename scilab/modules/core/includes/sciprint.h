/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/* Francois VOGEL , sciprint_full */
/*-----------------------------------------------------------------------------------*/
#ifndef __SCIPRINT_H__
#define __SCIPRINT_H__

#include <stdarg.h>
#include "localization.h"

/* any string of length greater than MAXPRINTF gets truncated in sciprint */
#define MAXPRINTF 512

/* MAXCHARSSCIPRINT_FULL is for sciprint_full - more than this gets truncated */
#define MAXCHARSSCIPRINT_FULL 5000  

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
 * @return return the number of characters printed
 */
int sciprint2(int iv,char *fmt,...);

/**
 * sciprint geared towards long strings (>MAXPRINTF) 
 * the long string is splitted in elements of length 
 * equal to the number of columns from lines()                   
 * @param fmt Format of the format string
 * @param ...
 */
void sciprint_full(char *fmt,...) ;

#ifdef _MSC_VER
struct tagTW;
/**
 * To access variable textwin
 */
struct tagTW * getTextWin( void ) ;
#endif

#endif /* __SCIPRINT_H__ */
/*-----------------------------------------------------------------------------------*/ 
