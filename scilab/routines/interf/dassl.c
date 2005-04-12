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

  if (setjmp(slatec_jmp_env)) return;
  C2F(ddassl)(res, neq, t, y, yprime, tout, info, rtol, atol, idid, rwork, 
	      lrw, iwork, liw, rpar, ipar, jac);
}
