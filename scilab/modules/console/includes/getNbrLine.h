/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef _GETNBRLINE_H
#define _GETNBRLINE_H

#include <wchar.h>

/**
 * Get how may lines the string take in the window.
 * @param string whose number of line will be counted
 * @return number of line used by the string.
 */
int getNbrLine(wchar_t * str);

#endif /* !_GETNBRLINE_H */
