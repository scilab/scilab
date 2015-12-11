/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
