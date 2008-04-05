/*--------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/*--------------------------------------------------------------------------*/ 
#include "Scierror.h"
#include "machine.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "gw_fileio.h"
#include "filesmanagement.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
int int_objfprintf(char *fname,unsigned long fname_len)
{
	FILE *f;
	static int l1, m1, n1,l2,m2,n2,lcount,rval, mx, mk, nk, k;
	char *ptrFormat=NULL;
	int i=0;
	int NumberPercent=0;
	int param1=0;

	Nbvars = 0;
	CheckRhs(1,1000);
	CheckLhs(0,1);
	if ( Rhs < 2 ) 
	{ 
		Scierror(999,_("%s: Wrong number of input arguments: At least %d expected.\n"),fname,2);
		return 0;
	}
	for (k=3;k<=Rhs;k++) 
	{
		if (VarType(k) !=sci_matrix && VarType(k) !=sci_strings) {OverLoad(k);return 0;}
	}
	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1); /* file id */
	GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2); /* format */
	ptrFormat=cstk(l2);

	param1=*istk(l1);

	if ((f= GetFileOpenedInScilab(param1)) == (FILE *)0)
	{
		Scierror(999,_("%s: Wrong file descriptor: %d.\n"),fname,*istk(l1));
		return 0;
	}

	for(i=0;i<(int)strlen(ptrFormat);i++)
	{
		if (ptrFormat[i]=='%') 
		{
			NumberPercent++;
			if (ptrFormat[i+1]=='%') {NumberPercent--;i++;}
		}
	}

	if (NumberPercent<Rhs-2)
	{
		Scierror(999,_("%s: Wrong number of input arguments: %d expected.\n"),fname,NumberPercent+2);
		return 0;
	}

	mx=0;
	if (Rhs>=3) 
	{
		GetMatrixdims(3,&mx,&nk);
		for (k=4;k<=Rhs;k++) 
		{
			GetMatrixdims(k,&mk,&nk);
			mx = Min(mx,mk);
		}
	}
	lcount=1;
	if (Rhs == 2) 
		rval=do_xxprintf("fprintf",f,cstk(l2),Rhs,2,lcount,(char **)0);
	else while (1) 
	{
		if ((rval=do_xxprintf("fprintf",f,cstk(l2),Rhs,2,lcount,(char **)0)) < 0) break;
		lcount++;
		if (lcount>mx) break;
	}
	if (rval == RET_BUG) return 0;
	LhsVar(1)=0; /** No return value **/
	PutLhsVar();
	return 0;
} 
/*--------------------------------------------------------------------------*/ 
