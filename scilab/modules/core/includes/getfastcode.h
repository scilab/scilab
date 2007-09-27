/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __GETFASTCODE_H__
#define __GETFASTCODE_H__

#include "machine.h"

/**
* converts from ascii to Scilab internal coding 
* @param n integer, length of the string to be converted integer
* @param line: integer array (where Scilab coded string are stored ) 
* @param string: string 
* @param job: integer flag 
* 1: code-->ascii 
* 0: ascii-->code 
*/
integer C2F(getfastcode)(unsigned char *c, unsigned long c_len);

#endif /* __GETFASTCODE_H__ */
