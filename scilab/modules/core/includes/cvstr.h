/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __CVSTR_H__
#define __CVSTR_H__

#include "machine.h" /* C2F */
/**
*   converts from ascii to Scilab internal coding 
* @param n integer, length of the string to be converted integer
* @param line: integer array (where Scilab coded string are stored ) 
* @param string: string 
* @param job: integer flag 
* 1: code-->ascii 
* 0: ascii-->code 
*/
int C2F(cvstr)(integer * n,integer * line,char * str,integer * job,unsigned long str_len);

#endif /* __CVSTR_H__ */
