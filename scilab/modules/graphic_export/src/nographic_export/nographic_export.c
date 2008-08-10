/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "machine.h" 
#include "nographic_export.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
int gw_graphic_export(void)
{
	Scierror(999,_("Scilab Graphic export module not installed.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/ 
