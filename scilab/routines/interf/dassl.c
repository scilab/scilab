/* C driver over ddassl to handle longjump from xerhlt*/

#include <setjmp.h>
#include "../machine.h"
extern jmp_buf slatec_jmp_env; 

extern void C2F(ddassl)(void *res, int *neq, double *t, double *y, double *yprime, 
			double *tout, int *info, double *rtol, double *atol, 
			int *idid, double *rwork, int *lrw, int *iwork, int *liw, 
			double *rpar, int *ipar, void *jac);


void  C2F(dassl)(void *res, int *neq, double *t, double *y, double *yprime, double *tout, 
	    int *info, double *rtol, double *atol, int *idid, double *rwork, 
	    int *lrw, int *iwork, int *liw, double *rpar, int *ipar, void *jac) 
{
  int itemp[12],i;
  if (setjmp(slatec_jmp_env)) return;
  /* Nest instruction is a patch for sun solaris 5.8, 
  without this trick the call to ddassl hangs Scilab */
  for (i=0;i<11;i++) itemp[i]=info[i];
  C2F(ddassl)(res, neq, t, y, yprime, tout, itemp, rtol, atol, idid, rwork, 
	      lrw, iwork, liw, rpar, ipar, jac);
}

