/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "fromc.h"
/*--------------------------------------------------------------------------*/ 
static int bFromC = FALSE;
/*--------------------------------------------------------------------------*/ 
void SetFromCToON(void)
{
	bFromC=(int)(TRUE);
}
/*--------------------------------------------------------------------------*/ 
int IsFromC(void)
{
	return bFromC;
}
/*--------------------------------------------------------------------------*/ 
