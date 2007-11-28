/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __GETFASTCODE_H__
#define __GETFASTCODE_H__

#include "machine.h"

/**
* converts from ascii to Scilab internal coding 
* @param n integer, length of the string to be converted integer
* @param line: integer array (where Scilab coded string are stored ) 
* @param a char
* @return internal code
* getfastcode uses convertAsciiCodeToScilabCode
*/
integer C2F(getfastcode)(unsigned char *c, unsigned long c_len);

/**
* converts from ascii to Scilab internal coding 
* @param[in] ascii char
* @return scilab code
*/
int convertAsciiCodeToScilabCode(unsigned char scilab_char);

/**
* converts Scilab internal coding to ascii code
* @param[in] scilab_code
* @return ascii_code [0 255]
*/
char convertScilabCodeToAsciiCode(int scilab_code);

#endif /* __GETFASTCODE_H__ */
