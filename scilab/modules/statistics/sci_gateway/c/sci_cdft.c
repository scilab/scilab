/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_statistics.h"
#include "machine.h"
#include "stack-c.h"
#include "CdfBase.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(cdft) __PARAMS((int *,double *,double *,double *,double *, int *,double *));
/*-----------------------------------------------------------------------------------*/ 
static void cdftErr(int status,double bound);
/*-----------------------------------------------------------------------------------*/ 
/*
*  hand written interface 
*  Interface for cdft
*  T distribution
*/
/*-----------------------------------------------------------------------------------*/ 
int cdftI(char* fname,unsigned long l)
{ 
	int minrhs = 3,maxrhs = 4,minlhs=1,maxlhs=2,m1,n1,l1;
	Nbvars = 0;
	CheckRhs(minrhs,maxrhs);
	CheckLhs(minlhs,maxlhs);
	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	if ( strcmp(cstk(l1),"PQ")==0) 
	{
		static int callpos[4] = {2,3,0,1};
		CdfBase(fname,2,2,callpos,"PQ","T and Df",1,C2F(cdft),
			cdftErr);
	}
	else if ( strcmp(cstk(l1),"T")==0) 
	{
		static int callpos[4] = {1,2,3,0};
		CdfBase(fname,3,1,callpos,"T","Df, P and Q",2,C2F(cdft),
			cdftErr);
	}
	else if ( strcmp(cstk(l1),"Df")==0) 
	{
		static int callpos[4] = {0,1,2,3};
		CdfBase(fname,3,1,callpos,"Df","P,Q and T",3,C2F(cdft),
			cdftErr);
	}
	else 
	{
		Scierror(999,"%s: Wrong first argument %s\n",fname,cstk(l1));
	}      
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
static void cdftErr(int status,double bound)
{
	static char *param[7]={"X", "P","Q","T","Df"};
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
