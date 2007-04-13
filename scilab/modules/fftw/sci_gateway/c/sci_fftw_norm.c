/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* ALAN LAYEC initial revision  : 30/09/06 fftw3 toolbox*/
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_fftw_norm.h"
#include "fftw_utilities.h"
#include "gw_fftw.h"
/*-----------------------------------------------------------------------------------*/ 
extern FFTW_Plan_struct Sci_Plan;
/*-----------------------------------------------------------------------------------*/ 
/* fftw_norm function.
*
* Scilab Calling sequence :
*   -->[a]=fftw_norm();
* or
*   -->[a]=fftw_norm(b);
*
* with a and b boolean scalar.
*
* give and set if scilab fftw function
* compute normalized ifft.
* (default is true)
*
*/
int sci_fftw_norm __PARAMS((char *fname,unsigned long fname_len))
{
	static int l1, l2, m1, n1;

	CheckRhs(0,1);

	if (Rhs==1)
	{
		GetRhsVar(1,"b",&m1,&n1,&l1);

		CheckDims(1,m1,n1,1,1);

		Sci_Plan.norm=*istk(l1);
	}

	/* */
	m1=1;n1=m1;
	CreateVar(Rhs+2, "b",&m1,&n1,&l2)
		*istk(l2)=Sci_Plan.norm;

	/* */
	LhsVar(1)=Rhs+2;
	PutLhsVar();
	/* */
	return(0);
}
/*-----------------------------------------------------------------------------------*/ 
