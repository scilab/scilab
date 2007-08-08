/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_statistics.h"
#include "machine.h"
#include "stack-c.h"
#include "CdfBase.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(cdfpoi) __PARAMS((int *,double *,double *,double *,double *, int *,double *));
/*-----------------------------------------------------------------------------------*/ 
static void cdfpoiErr(int status,double bound);
/*-----------------------------------------------------------------------------------*/ 
/*
*  hand written interface 
*  Interface for cdfpoi
*  POIsson distribution
*/
/*-----------------------------------------------------------------------------------*/ 
int cdfpoiI(char* fname,unsigned long l)
{ 
	int minrhs = 3,maxrhs = 4,minlhs=1,maxlhs=2,m1,n1,l1;
	Nbvars = 0;
	CheckRhs(minrhs,maxrhs);
	CheckLhs(minlhs,maxlhs);
	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	if ( strcmp(cstk(l1),"PQ")==0) 
	{
		static int callpos[4] = {2,3,0,1};
		CdfBase(fname,2,2,callpos,"PQ","S and Xlam",1,C2F(cdfpoi),
			cdfpoiErr);
	}
	else if ( strcmp(cstk(l1),"S")==0) 
	{
		static int callpos[4] = {1,2,3,0};
		CdfBase(fname,3,1,callpos,"S","Xlam,P and Q",2,C2F(cdfpoi),
			cdfpoiErr);
	}
	else if ( strcmp(cstk(l1),"Xlam")==0) 
	{
		static int callpos[4] = {0,1,2,3};
		CdfBase(fname,3,1,callpos,"Xlam","P,Q and S",3,C2F(cdfpoi),
			cdfpoiErr);
	}
	else 
	{
		Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
	}      
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
static void cdfpoiErr(int status,double bound)
{
	static char *param[7]={"X", "P","Q","S","Xlam"};
	switch ( status ) 
	{
	case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
	case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
	case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
	default : 
		Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
			param[-status-1],bound);
	}
}
/*-----------------------------------------------------------------------------------*/ 

