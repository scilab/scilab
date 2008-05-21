/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "gotoxy_nw.h"
/*--------------------------------------------------------------------------*/
void gotoxy_nw(int x, int y)
{
	COORD c;
	c.X = (SHORT)(x - 1);
	c.Y = (SHORT)(y - 1);
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/*--------------------------------------------------------------------------*/