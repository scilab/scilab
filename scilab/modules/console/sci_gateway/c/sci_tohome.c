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
/*------------------------------------------------------------------------*/
/* desc : interface for tohome primitive                                  */
/*------------------------------------------------------------------------*/
#include "gw_console.h"
#include "api_scilab.h"
#include "localization.h"
#include "tohome.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int sci_tohome(char *fname,unsigned long fname_len)
{

	CheckRhs(0,0);
	CheckLhs(0,1);

	if (!tohome())
	{
		sciprint(_("%s: This feature has not been implemented in this mode.\n"),fname);
	}

	LhsVar(1) = 0;
	PutLhsVar();

	return 0 ;
}
/*--------------------------------------------------------------------------*/
