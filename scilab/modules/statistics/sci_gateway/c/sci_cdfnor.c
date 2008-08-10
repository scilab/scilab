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
#include "machine.h"
#include "stack-c.h"
#include "CdfBase.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(cdfnor)(int *,double *,double *,double*,double *,double *, int *,double *);
/*--------------------------------------------------------------------------*/
static void cdfnorErr(int status,double bound);
/*--------------------------------------------------------------------------*/
/*
* hand written interface
* Interface for cdfnor
* Cumulative Distribution Function
* NORmal distribution
*/
/*--------------------------------------------------------------------------*/
int cdfnorI(char* fname,unsigned long l)
{
	int m1,n1,l1;
	Nbvars = 0;
	CheckRhs(4,5);
	CheckLhs(1,2);
	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	if ( strcmp(cstk(l1),"PQ")==0)
	{
		static int callpos[5] = {3,4,0,1,2};
		CdfBase(fname,3,2,callpos,"PQ",_("X,Mean and Std"),1,C2F(cdfnor),
			cdfnorErr);
	}
	else if ( strcmp(cstk(l1),"X")==0)
	{
		static int callpos[5] = {2,3,4,0,1};
		CdfBase(fname,4,1,callpos,"X",_("Mean,Std,P and Q"),2,C2F(cdfnor),
			cdfnorErr);
	}
	else if ( strcmp(cstk(l1),"Mean")==0)
	{
		static int callpos[5] = {1,2,3,4,0};
		CdfBase(fname,4,1,callpos,"Mean",_("Std,P,Q and X"),3,C2F(cdfnor),
			cdfnorErr);
	}
	else if ( strcmp(cstk(l1),"Std")==0)
	{
		static int callpos[5] = {0,1,2,3,4};
		CdfBase(fname,4,1,callpos,"Std",_("P,Q,X and Mean"),4,C2F(cdfnor),
			cdfnorErr);
	}
	else
	{
		Scierror(999,_("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"),fname,1,"PQ","X","Mean","Std");
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static void cdfnorErr(int status,double bound)
{
	static char *param[7]={"X", "P","Q","X","Mean","Std"};
	switch ( status )
	{
	case 1 : Scierror(999,_("Answer appears to be lower than lowest search bound %f\n"),bound);break;
	case 2 : Scierror(999,_("Answer appears to be higher than greatest search bound %f\n"),bound);break;
	case 3 : Scierror(999," P + Q .ne. 1\n");break ;
	case 4 : Scierror(999,_(" Std must not be zero\n"));break ;
	default :
		Scierror(999,_("Input argument %c is out of range.\nBound exceeded: %f\n"),
			param[-status-1],bound);
	}
}
/*--------------------------------------------------------------------------*/
