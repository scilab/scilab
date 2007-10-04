/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_statistics.h"
#include "machine.h"
#include "stack-c.h"
#include "CdfBase.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(cdff) __PARAMS((int *,double *,double *,double*,double *,double *, int *,double *));
/*-----------------------------------------------------------------------------------*/ 
static void cdffErr(int status,double bound);
/*-----------------------------------------------------------------------------------*/ 
/*
*  hand written interface 
*      Interface for cdff
*               Cumulative Distribution Function
*              F distribution
*/
/*-----------------------------------------------------------------------------------*/ 
int cdffI(char* fname,unsigned long l)
{ 
	int minrhs = 4,maxrhs = 5,minlhs=1,maxlhs=2,m1,n1,l1;
	Nbvars = 0;
	CheckRhs(minrhs,maxrhs);
	CheckLhs(minlhs,maxlhs);
	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	if ( strcmp(cstk(l1),"PQ")==0) 
	{
		static int callpos[5] = {3,4,0,1,2};
		CdfBase(fname,3,2,callpos,"PQ","F,Dfn and Dfd",1,C2F(cdff),
			cdffErr);
	}
	else if ( strcmp(cstk(l1),"F")==0) 
	{
		static int callpos[5] = {2,3,4,0,1};
		CdfBase(fname,4,1,callpos,"F","Dfn,Dfd,P and Q",2,C2F(cdff),
			cdffErr);
	}
	else if ( strcmp(cstk(l1),"Dfn")==0) 
	{
		static int callpos[5] = {1,2,3,4,0};
		CdfBase(fname,4,1,callpos,"Dfn","Dfd,P,Q and F",3,C2F(cdff),
			cdffErr);
	}
	else if ( strcmp(cstk(l1),"Dfd")==0) 
	{
		static int callpos[5] = {0,1,2,3,4};
		CdfBase(fname,4,1,callpos,"Dfd","P,Q,F and Dfn",4,C2F(cdff),
			cdffErr);
	}
	else 
	{
		Scierror(999,"%s: Wrong first argument %s\n",fname,cstk(l1));
	}      
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
static void cdffErr(int status,double bound)
{
	static char *param[7]={"X", "P","Q","F","Dfn","Dfd"};
	switch ( status ) 
	{
	case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\n",bound);break;
	case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\n",bound);break;
	case 3 : Scierror(999," P + Q .ne. 1 \n");break ;
	default : 
		Scierror(999,"input parameter %c is out of range \n\tbound exceeded: %f\n",
			param[-status-1],bound);
	}
}
/*-----------------------------------------------------------------------------------*/ 
