/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include "machine.h" 
#include "Scierror.h"
#include "with_java.h"
#include "InitializeJVM.h"
#include "TerminateJVM.h"
#include "loadBackGroundClassPath.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/ 
/* not using gw_jvm.h */
int gw_jvm(void);
/*--------------------------------------------------------------------------*/ 
int gw_jvm(void)
{
	Scierror(999,_("Scilab Java module not installed.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/ 
BOOL InitializeJVM(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
BOOL TerminateJVM(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
BOOL with_java(void)
{
	return FALSE;	
}
/*--------------------------------------------------------------------------*/ 
BOOL loadBackGroundClassPath(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
