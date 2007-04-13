/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* ALAN LAYEC initial revision  : 30/09/06 fftw3 toolbox*/
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_fftw_inplace.h"
#include "fftw_utilities.h"
#include "gw_fftw.h"
/*-----------------------------------------------------------------------------------*/ 
extern FFTW_Plan_struct Sci_Plan;
/*-----------------------------------------------------------------------------------*/ 
/* fftw_inplace function.
*
* Scilab Calling sequence :
*   -->[a]=fftw_inplace();
* or
*   -->[a]=fftw_inplace(b);
*
* with a and b boolean scalar.
*
* give and set if fftw works in place.
* (default is true)
*/
int sci_fftw_inplace __PARAMS((char *fname,unsigned long fname_len))
{
	static int l1, l2, m1, n1;

	CheckRhs(0,1);

	if (Rhs==1)
	{
		GetRhsVar(1,"b",&m1,&n1,&l1);

		CheckDims(1,m1,n1,1,1);

		Sci_Plan.in=*istk(l1);
	}

	/* */
	m1=1;n1=m1;
	CreateVar(Rhs+2, "b",&m1,&n1,&l2)
		*istk(l2)=Sci_Plan.in;

	/* */
	LhsVar(1)=Rhs+2;
	PutLhsVar();
	/* */
	return(0);
}
/*-----------------------------------------------------------------------------------*/ 
