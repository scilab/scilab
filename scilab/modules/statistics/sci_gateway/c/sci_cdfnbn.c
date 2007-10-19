/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_statistics.h"
#include "machine.h"
#include "stack-c.h"
#include "CdfBase.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(cdfnbn) __PARAMS((int *,double *,double *,double *,double *,double *,double *, int *,double *));
/*-----------------------------------------------------------------------------------*/ 
static void cdfnbnErr(int status,double bound);
/*-----------------------------------------------------------------------------------*/ 
/*
*  hand written interface 
*  Interface for cdfnbn 
* SUBROUTINE cdfnbn(which,p,q,s,xn,pr,ompr,status,bound)
*               Cumulative Distribution Function
*               Negative BiNomial distribution
*/
/*-----------------------------------------------------------------------------------*/ 
int cdfnbnI(char* fname,unsigned long l)
{ 
	int minrhs = 5,maxrhs = 6,minlhs=1,maxlhs=2,m1,n1,l1;
	Nbvars = 0;
	CheckRhs(minrhs,maxrhs);
	CheckLhs(minlhs,maxlhs);
	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	if ( strcmp(cstk(l1),"PQ")==0) 
	{
		static int callpos[6] = {4,5,0,1,2,3};
		CdfBase(fname,4,2,callpos,"PQ",_("S,XN,PR and OMPR"),1,C2F(cdfnbn),
			cdfnbnErr);
	}
	else if ( strcmp(cstk(l1),"S")==0) 
	{
		static int callpos[6] = {3,4,5,0,1,2};
		CdfBase(fname,5,1,callpos,"S",_("XN,PR,OMPR,P and Q"),2,C2F(cdfnbn),
			cdfnbnErr);
	}
	else if ( strcmp(cstk(l1),"Xn")==0) 
	{
		static int callpos[6] = {2,3,4,5,0,1};
		CdfBase(fname,5,1,callpos,"Xn",_("PR,OMPR,P,Q and S"),3,C2F(cdfnbn),
			cdfnbnErr);
		return 0;
	}
	else if ( strcmp(cstk(l1),"PrOmpr")==0) 
	{
		static int callpos[6] = {0,1,2,3,4,5};
		CdfBase(fname,4,2,callpos,"PrOmpr",_("P,Q,S and Xn"),4,C2F(cdfnbn),
			cdfnbnErr);
	}
	else 
	{
		Scierror(999,_("%s: Wrong first argument %s\n"),fname,cstk(l1));
	}      
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
static void cdfnbnErr(int status,double bound)
{
	static char param[]="-PQXYAB";
	switch ( status ) 
	{
	case 1 : Scierror(999,_("answer appears to be lower than lowest search bound %f\n"),bound);break;
	case 2 : Scierror(999,_("answer appears to be higher than greatest search bound %f\n"),bound);break;
	case 3 : Scierror(999," P + Q .ne. 1 \n");break ;
	case 4 : Scierror(999," Pr + Ompr .ne. 1 \n");break;
	default : 
		Scierror(999,"input parameter %c is out of range \n\tbound exceeded: %f\n",
			param[-status-1],bound);
	}
}
/*-----------------------------------------------------------------------------------*/ 
