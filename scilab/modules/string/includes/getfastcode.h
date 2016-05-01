
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
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
#ifndef __GETFASTCODE_H__
#define __GETFASTCODE_H__

#include "machine.h"
#include "dynlib_string.h"

/**
* converts from ascii to Scilab internal coding
* @param n integer, length of the string to be converted integer
* @param line: int array (where Scilab coded string are stored )
* @param a char
* @return internal code
* getfastcode uses convertAsciiCodeToScilabCode
*/
STRING_IMPEXP int C2F(getfastcode)(unsigned char *c, unsigned long c_len);

/**
* converts from ascii to Scilab internal coding
* @param[in] ascii char
* @return scilab code
*/
STRING_IMPEXP int convertAsciiCodeToScilabCode(unsigned char scilab_char);

/**
* converts Scilab internal coding to ascii code
* @param[in] scilab_code
* @return ascii_code [0 255]
*/
STRING_IMPEXP unsigned char convertScilabCodeToAsciiCode(int scilab_code);

#endif /* __GETFASTCODE_H__ */
