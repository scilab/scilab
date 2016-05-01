/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
