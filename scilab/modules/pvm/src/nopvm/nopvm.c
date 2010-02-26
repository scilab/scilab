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
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#pragma comment(lib,"../../../../bin/libintl.lib")
#endif
/*--------------------------------------------------------------------------*/ 
int gw_pvm(void)
{
	Scierror(999,_("Scilab PVM module not installed.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/ 
