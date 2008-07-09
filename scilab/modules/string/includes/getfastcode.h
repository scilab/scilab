
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
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
unsigned char convertScilabCodeToAsciiCode(int scilab_code);

#endif /* __GETFASTCODE_H__ */
