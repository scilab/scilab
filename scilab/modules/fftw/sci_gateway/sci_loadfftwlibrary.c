/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_loadfftwlibrary.h"
#include "callfftw.h"
/*-----------------------------------------------------------------------------------*/ 
int sci_loadfftwlibrary __PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	n1=1;
	if ( LoadFFTWLibrary() )
	{
		CreateVar(Rhs+1, "b", &n1,&n1,&l1);
		*istk(l1)=(int)(TRUE);
	}
	else
	{
		CreateVar(Rhs+1, "b", &n1,&n1,&l1);
		*istk(l1)=(int)(FALSE);
	}

	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();


	return(0);
}
/*-----------------------------------------------------------------------------------*/ 
