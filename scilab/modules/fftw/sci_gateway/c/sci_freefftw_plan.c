/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* ALAN LAYEC initial revision  : 30/09/06 fftw3 toolbox*/
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_freefftw_plan.h"
#include "fftw_utilities.h"
#include "gw_fftw.h"
/*-----------------------------------------------------------------------------------*/ 
extern FFTW_Plan_struct Sci_Plan;
/*-----------------------------------------------------------------------------------*/ 
/* freefftw_plan function.
*
* Scilab Calling sequence :
*   -->[a]=freefftw_plan();
*
*  a is an double scalar given the
*  current plan number stored in
*  Sci_Plan structure. (always 0!)
*
* free all the static structure Sci_Plan.
*
*/
int sci_freefftw_plan __PARAMS((char *fname,unsigned long fname_len))
{
	static int l1, m1, n1;

	CheckLhs(0,1);

	/* */
	FreeFTTWPlan();

	/* */
	m1=1;n1=m1;
	CreateVar( Rhs+1, "d", &m1, &n1, &l1);
	*stk(l1)=Sci_Plan.nb_CPlan;

	/* */
	LhsVar(1)=Rhs+1;
	PutLhsVar();
	/* */
	return(0);
}
/*-----------------------------------------------------------------------------------*/ 
