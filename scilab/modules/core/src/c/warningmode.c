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
#include "warningmode.h"
/*--------------------------------------------------------------------------*/
static BOOL WARNINGMODEON=TRUE;
/*--------------------------------------------------------------------------*/
BOOL setWarningMode(BOOL ON)
{
	WARNINGMODEON = ON;
	return WARNINGMODEON;
}
/*--------------------------------------------------------------------------*/
BOOL getWarningMode(void)
{
	return WARNINGMODEON;
}
/*--------------------------------------------------------------------------*/
int C2F(iswarningon)(int *rep)
{
	if (WARNINGMODEON) *rep=1;
	else *rep=0;
	return 0;
}
/*--------------------------------------------------------------------------*/
