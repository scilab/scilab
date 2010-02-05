/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
  * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "stack-c.h"
#include "gw_string.h"
#include "NumTokens.h"
#include "StringConvert.h"
#include "sciprint.h"
#include "localization.h"
#include "warningmode.h"
/*--------------------------------------------------------------------------*/ 
int sci_NumTokens(char *fname,unsigned long fname_len)
{
	int l1 = 0,m1 = 0,n1 = 0,l2 = 0,un = 1;

	if (getWarningMode())
	{
		sciprint(_("WARNING: Function %s is obsolete.\n"), fname);
		sciprint(_("WARNING: Please use %s instead.\n"), "tokens");
		sciprint(_("WARNING: This function will be permanently removed in Scilab %s.\n"), "5.2.1");
	}

	Nbvars = 0;
	CheckRhs(1,1);
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	StringConvert(cstk(l1));  /* conversion */
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &un, &un, &l2);
	*stk(l2) = (double) NumTokens(cstk(l1));
	LhsVar(1) = Rhs+1;
	PutLhsVar();
	return 0;
}  
/*--------------------------------------------------------------------------*/ 
