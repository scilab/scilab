
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __SETSCILABCHARACTERSCODES_H__
#define __SETSCILABCHARACTERSCODES_H__


/** ASCII printable characters
*  http://en.wikipedia.org/wiki/ASCII
*/

/**
* Scilab codes for known characters
* In Scilab, chars are first converted as integers (positives and some
* negatives) and we have :
*
*        code | 0  1 .... 9 | 10  11 ...  35 | 36  37  38  39 | 40
*        -----+-------------+----------------+----------------+------
*        char | 0  1 .... 9 |  a   b ...   z |  _   #   !   $ | blank
*        ============================================================
*        char |             |  A   B ...   Z |          ?     | tab
*        -------------------+----------------+----------------+------
*        code |             |-10 -11 ... -35 |         -38    ! -40
*
*       In fact (for the mapping code -> char), code = -1
*       to -9 correspond also to chars 1 to 9 and code = -36,-37,-39
*       to the char 0
*
*       So if c is a scilab coded char then :
*
*           abs(c) <= 9  => c is a digit
*           abs(c) < 40  => c is an alphanum Scilab char (which
*                           comprise _ but also # ! $ ?). Moreover
*                           Scilab names may begin with %
*           abs(c) == 40 => c is a blank "like" (blank or tab) char
*	The upper-case characters and some equivalents are coded by the lower-case code with a sign
*	change.
*   ref : Guide for Developers (Scilab Internals)
*/

/**
* Set Scilab Characters Codes in C2F(cha1)
*/
void setScilabCharactersCodes(void);

#endif /* __SETSCILABCHARACTERSCODES_H__ */
/*--------------------------------------------------------------------------*/
