/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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
#ifndef __TERMPOSITION_H__
#define __TERMPOSITION_H__

/**
* Set Cursor position
* @param[in] x
* @param[in] y
*/
void TermSetPosition(int x, int y);

/**
* Get Cursor position
* @param[out] x
* @param[out] y
*/
void TermGetPosition(int *x, int *y);

#endif /* __TERMPOSITION_H__ */
/*--------------------------------------------------------------------------*/
