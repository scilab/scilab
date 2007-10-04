/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_statistics.h"
#include "machine.h"
#include "stack-c.h"
#include "CdfBase.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(cdfbin) __PARAMS((int *,double *,double *,double *,double *,double *,double *, int *,double *));
/*-----------------------------------------------------------------------------------*/ 
static void cdfbinErr(int status,double bound);
/*-----------------------------------------------------------------------------------*/ 
/*
* hand written interface 
* Interface for cdfbin
*
*     SUBROUTINE CDFBIN ( WHICH, P, Q, S, XN, PR, OMPR, STATUS, BOUND )
*              Cumulative Distribution Function
*                        BINomial distribution
*/
/*-----------------------------------------------------------------------------------*/ 
int cdfbinI(char* fname,unsigned long l)
{ 
	int minrhs = 5,maxrhs = 6,minlhs=1,maxlhs=2,m1,n1,l1;
	Nbvars = 0;
	CheckRhs(minrhs,maxrhs);
	CheckLhs(minlhs,maxlhs);
	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	if ( strcmp(cstk(l1),"PQ")==0) 
	{
		static int callpos[6] = {4,5,0,1,2,3};
		CdfBase(fname,4,2,callpos,"PQ","S,Xn,Pr and Ompr",1,C2F(cdfbin),
			cdfbinErr);
	}
	else if ( strcmp(cstk(l1),"S")==0) 
	{
		static int callpos[6] = {3,4,5,0,1,2};
		CdfBase(fname,5,1,callpos,"S","Xn,Pr,Ompr,P and Q",2,C2F(cdfbin),
			cdfbinErr);
	}
	else if ( strcmp(cstk(l1),"Xn")==0) 
	{
		static int callpos[6] = {2,3,4,5,0,1};
		CdfBase(fname,5,1,callpos,"Xn","Pr,OMPr,P,Q and S",3,C2F(cdfbin),
			cdfbinErr);
	}
	else if ( strcmp(cstk(l1),"PrOmpr")==0) 
	{
		static int callpos[6] = {0,1,2,3,4,5};
		CdfBase(fname,4,2,callpos,"PrOmpr","P,Q,S  and Xn",4,C2F(cdfbin),
			cdfbinErr);
	}
	else 
	{
		Scierror(999,"%s: Wrong first argument %s\n",fname,cstk(l1));
	}      
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
static void cdfbinErr(int status,double bound)
{
	static char *param[7]={"Which", "P","Q","Pr","Ompr","S","Xn"};
	switch ( status ) 
	{
	case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\n",bound);break;
	case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\n",bound);break;
	case 3 : Scierror(999," P + Q .ne. 1 \n");break ;
	case 4 : Scierror(999," Pr + Ompr .ne. 1 \n");break;
	default : 
		Scierror(999,"input parameter %c is out of range \n\tbound exceeded: %f\n",
			param[-status-1],bound);
	}
}
/*-----------------------------------------------------------------------------------*/ 
