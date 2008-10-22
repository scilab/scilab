/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "machine.h" /* C2F */
#include "BOOL.h" 
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
int gw_umfpack(void)
{
	Scierror(999,_("Scilab UMFPACK module not installed.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/ 
BOOL withumfpack(void)
{ 
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
int C2F(withumfpack)(int *rep)
{ 
	*rep =0;
	return 0;
}
/*--------------------------------------------------------------------------*/ 
