/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "Scierror.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "gw_fileio.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
int int_objprintf(char *fname,unsigned long fname_len)
{
	static int l1, m1, n1, lcount, rval, k ;
	
	char *ptrFormat   = NULL;
	int i             = 0;
	int NumberPercent = 0;
	int NumberCols    = 0;
	int NumberRows    = 0;
	
	Nbvars = 0;
	CheckRhs(1,1000);
	CheckLhs(0,1);
	
	if ( Rhs < 1 )
	{
		Scierror(999,_("%s: Wrong number of input arguments: Must be > 0.\n"),fname);
		return 0;
	}
	
	for (k=2;k<=Rhs;k++)
	{
		if ( (VarType(k) != sci_matrix) && (VarType(k) != sci_strings) )
		{
			OverLoad(k);
			return 0;
		}
	}
	
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	
	ptrFormat=cstk(l1);
	
	for( i=0; i<(int)strlen(ptrFormat); i++)
	{
		if (ptrFormat[i]=='%')
		{
			NumberPercent++;
			if (ptrFormat[i+1]=='%')
			{
				NumberPercent--;i++;
			}
		}
	}
	
	if ( (Rhs - 1) > NumberPercent )
	{
		Scierror(999,_("%s: Wrong number of input arguments: at most %d expected.\n"),fname,NumberPercent);
		return 0;
	}
	
	if( Rhs > 1 )
	{
		for( i = 2 ; i <= Rhs ; i++ )
		{
			int mk = 0;
			int nk = 0;
			
			GetMatrixdims(i,&mk,&nk);
			
			if( NumberRows == 0)
			{
				NumberRows  = mk;
			}
			else
			{
				NumberRows  = Min(NumberRows,mk);
			}
			NumberCols += nk;
		}
	}
	
	if ( NumberCols != NumberPercent )
	{
		Scierror(999,_("%s: Wrong number of input arguments: data doesn't fit with format.\n"),fname);
		return 0;
	}
	
	lcount = 1;
	if (Rhs == 1)
	{
		rval=do_xxprintf(fname,stdout,cstk(l1),Rhs,1,lcount,(char **)0);
	}
	else
	{
		while (1)
		{
			if ((rval = do_xxprintf(fname,stdout,cstk(l1),Rhs,1,lcount,(char **)0)) < 0)
			{
				break;
			}
			lcount++;
			if ( lcount > NumberRows )
			{
				break;
			}
		}
	}
	
	if (rval == RET_BUG) return 0;
	
	LhsVar(1)=0; /** No return value **/
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/ 
