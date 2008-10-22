
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCIPRINT_H__
#define __SCIPRINT_H__

#include <stdarg.h>
#include "BOOL.h"

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

/**
* print a string 
* @param[in] buffer to disp
* @param[in] TRUE or FALSE if we use diary
*/
void printf_scilab(char *buffer,BOOL withDiary);
#endif /* __SCIPRINT_H__ */
/*--------------------------------------------------------------------------*/ 
