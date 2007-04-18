/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Alan Layec : - initial  revision : 18/04/07 */
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/
#include "sci_fftw_forget_wisdom.h"
#include "fftw_utilities.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
/*-----------------------------------------------------------------------------------*/
extern FFTW_Plan_struct Sci_Forward_Plan;
extern FFTW_Plan_struct Sci_Backward_Plan;
/*-----------------------------------------------------------------------------------*/

/* Reset fftw wisdom
 *
 * Scilab Calling sequence :
 *   -->fftw_forget_wisdom();
 *
 * Input : Nothing
 *
 * Output : Nothing
 *
 */
int sci_fftw_forget_wisdom __PARAMS((char *fname,unsigned long fname_len))
{
 CheckRhs(0,0);

 FreeFFTWPlan(&Sci_Backward_Plan);
 FreeFFTWPlan(&Sci_Forward_Plan);

 call_fftw_forget_wisdom();

 PutLhsVar();

 return(0);
}
/*-----------------------------------------------------------------------------------*/
