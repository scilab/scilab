/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_fftwlibraryisloaded.h"
#include "callfftw.h"
#include "gw_fftw.h"
/*-----------------------------------------------------------------------------------*/ 
int sci_fftwlibraryisloaded __PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1;

	n1=1;
	if ( IsLoadedFFTW() )
	{
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
		*istk(l1)=(int)(TRUE);
	}
	else
	{
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
		*istk(l1)=(int)(FALSE);
	}

	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	return(0);
}
/*-----------------------------------------------------------------------------------*/ 
