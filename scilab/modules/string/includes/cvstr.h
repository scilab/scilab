
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __CVSTR_H__
#define __CVSTR_H__

#include "machine.h" /* C2F */

/**
* converts from ascii to Scilab internal coding 
* @param n integer, length of the string to be converted integer
* @param line: integer array (where Scilab coded string are stored ) 
* @param str: string 
* @param job: integer flag 
* @param str_len : length of str
* @return 1: code-->ascii 
* @return 0: ascii-->code 
*/
int C2F(cvstr)(integer * n,integer * line,char * str,integer * job,unsigned long str_len);

/**
* converts from ascii to Scilab internal coding 
* ascii->code is performed from end to the begining 
* @param n integer, length of the string to be converted integer
* @param line: integer array (where Scilab coded string are stored ) 
* @param str: string 
* @param job: integer flag 
* @param str_len : length of str
* @return 0: ascii-->code 
*/
int C2F(cvstr1)(integer *n,integer * line,char * str,integer * job,unsigned long str_len);

/**
* converts from Scilab internal coding to ascii
* @param n integer, length of the string to be converted integer
* @param line: integer array (where Scilab coded string are stored ) 
* @param str: string 
* @param str_len : length of str
* @return 0
*/
int C2F(codetoascii)(integer *n,integer * line,char * str, unsigned long str_len);

/**
* converts from ascii to Scilab internal coding 
* @param n integer, length of the string to be converted integer
* @param line: integer array (where Scilab coded string are stored ) 
* @param str: string 
* @param flagx:  1 or -1 and this is used when the 
* conversion is made with line and str stored at overlapping 
* memory zone 
* @param str_len : length of str
* @return 0
*/
int C2F(asciitocode)(integer * n,integer * line,char * str,integer * flagx,unsigned long  str_len);

#endif /* __CVSTR_H__ */
/*--------------------------------------------------------------------------*/
