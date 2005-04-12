/* C driver over ddassl to handle longjump from xerhlt*/
#include "../machine.h"

extern int C2F(setslatecjmp)();
extern void C2F(ddassl)(void *res, int *neq, double *t, double *y, double *yprime, 
			double *tout, int *info, double *rtol, double *atol, 
			int *idid, double *rwork, int *lrw, int *iwork, int *liw, 
			double *rpar, int *ipar, void *jac);


void  C2F(dassl)(void *res, int *neq, double *t, double *y, double *yprime, double *tout, 
	    int *info, double *rtol, double *atol, int *idid, double *rwork, 
	    int *lrw, int *iwork, int *liw, double *rpar, int *ipar, void *jac) 
{
  if (C2F(setslatecjmp)() ) return;
  C2F(ddassl)(res, neq, t, y, yprime, tout, info, rtol, atol, idid, rwork, 
	      lrw, iwork, liw, rpar, ipar, jac);
}
