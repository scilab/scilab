
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __CVSTR_H__
#define __CVSTR_H__

#include "machine.h" /* C2F */
#include "dynlib_string.h"

/**
* converts from ascii to Scilab internal coding
* @param n integer, length of the string to be converted integer
* @param line: int array (where Scilab coded string are stored )
* @param str: string
* @param job: int flag
* @param str_len : length of str
* @return 1: code-->ascii
* @return 0: ascii-->code
*/
STRING_IMPEXP int C2F(cvstr)(int * n, int * line, char * str, int * job, unsigned long str_len);

/**
* converts from ascii to Scilab internal coding
* ascii->code is performed from end to the begining
* @param n integer, length of the string to be converted integer
* @param line: int array (where Scilab coded string are stored )
* @param str: string
* @param job: int flag
* @param str_len : length of str
* @return 0: ascii-->code
*/
STRING_IMPEXP int C2F(cvstr1)(int *n, int * line, char * str, int * job, unsigned long str_len);

/**
* converts from Scilab internal coding to ascii
* @param n integer, length of the string to be converted integer
* @param line: int array (where Scilab coded string are stored )
* @param str: string
* @param str_len : length of str
* @return 0
*/
STRING_IMPEXP int C2F(codetoascii)(int *n, int * line, char * str, unsigned long str_len);

/**
* converts from ascii to Scilab internal coding
* @param n integer, length of the string to be converted integer
* @param line: int array (where Scilab coded string are stored )
* @param str: string
* @param flagx:  1 or -1 and this is used when the
* conversion is made with line and str stored at overlapping
* memory zone
* @param str_len : length of str
* @return 0
*/
STRING_IMPEXP int C2F(asciitocode)(int * n, int * line, char * str, int * flagx, unsigned long  str_len);

#endif /* __CVSTR_H__ */
/*--------------------------------------------------------------------------*/
