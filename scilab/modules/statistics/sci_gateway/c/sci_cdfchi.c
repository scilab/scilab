/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_statistics.h"
#include "stack-c.h"
#include "CdfBase.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(cdfchi)(int *,double *,double *,double *,double *, int *,double *);
/*--------------------------------------------------------------------------*/
static void cdfchiErr(int status,double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*  Interface for cdfchi
*
*      SUBROUTINE CDFCHI( WHICH, P, Q, X, DF, STATUS, BOUND )
*              Cumulative Distribution Function
*              CHI-Square distribution
*/
/*--------------------------------------------------------------------------*/
int cdfchiI(char* fname,unsigned long l)
{
	int m1,n1,l1;
	Nbvars = 0;
	CheckRhs(3,4);
	CheckLhs(1,2);
	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	if ( strcmp(cstk(l1),"PQ")==0)
	{
		static int callpos[4] = {2,3,0,1};
		CdfBase(fname,2,2,callpos,"PQ",_("X and Df"),1,C2F(cdfchi),
			cdfchiErr);
	}
	else if ( strcmp(cstk(l1),"X")==0)
	{
		static int callpos[4] = {1,2,3,0};
		CdfBase(fname,3,1,callpos,"X",_("Df, P and Q"),2,C2F(cdfchi),
			cdfchiErr);
	}
	else if ( strcmp(cstk(l1),"Df")==0)
	{
		static int callpos[4] = {0,1,2,3};
		CdfBase(fname,3,1,callpos,"Df",_("P,Q and X"),3,C2F(cdfchi),
			cdfchiErr);
	}
	else
	{
		Scierror(999,_("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"),fname,1,"PQ","X","DF");
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static void cdfchiErr(int status,double bound)
{
	static char *param[7]={"X", "P","Q","X","Df"};
	switch ( status )
	{
	case 1 : Scierror(999,_("Answer appears to be lower than lowest search bound %f\n"),bound);break;
	case 2 : Scierror(999,_("Answer appears to be higher than greatest search bound %f\n"),bound);break;
	case 3 : Scierror(999," P + Q .ne. 1 \n");break ;
	case 10 : Scierror(999,_("cdfchi: Error in cumgam\n"));break;
	default :
		Scierror(999,_("Input argument %c is out of range.\nBound exceeded: %f\n"),
			param[-status-1],bound);
	}
}
/*--------------------------------------------------------------------------*/
