#include <string.h> 

#include "../stack-c.h"
#include "../graphics/Math.h" /* Abs */ 

#include "matdsr.h"

static integer cx1 = 1;
static integer cx0 = 0;
static double c_b40 = 0.;

/*--------------------------------------------------
 * [Ab [,X [,bs]]]=bdiag(A [,rMax]) 
 *--------------------------------------------------*/ 

int C2F(intbdiagr)(char *fname, long unsigned int fname_len)
{
    integer ix1, ix2;
    double dx1;
    int fail;
    double rMax;
    integer ix, j, k, m, n;
    double t;
    integer nbloc, lrMax;
    integer m1, n1, la, le, lj, it;
    integer lw, lx ;
    integer lai, lib, lbs, lxi, lxr;

    CheckRhs(1,2);
    CheckLhs(1,3); 

    GetRhsCVar(1, "d", &it, &m, &n, &la, &lai);
    CheckSquare(1,m,n);

    if (n == 0) {
	CreateVar(2, "d", &cx0, &cx0, &lx);
	CreateVar(3, "d", &cx0, &cx0, &lbs);
	LhsVar(1) = 1;
	LhsVar(2) = 2;
	LhsVar(3) = 3;
	return 0;
    }
    ix1 = (it + 1) * m * n;
    if (C2F(vfinite)(&ix1, stk(la )) == 0) {
	Err = 1;
	Error(264);
	return 0;
    }
    if (Rhs == 2) {
	GetRhsVar(2, "d", &n1, &m1, &lrMax);
	CheckScalar(2,n1,m1); 
	rMax = *stk(lrMax );
    } else {
	rMax = 1.;
	lj = la - 1;
	ix1 = n;
	for (j = 1; j <= ix1; ++j) {
	    t = 0.;
	    ix2 = n;
	    for (ix = 1; ix <= ix2; ++ix) {
		t += (dx1 = *stk(lj + ix ), Abs(dx1));
	    }
	    if (t > rMax) {
		rMax = t;
	    }
	    lj += n;
	}
    }
    CreateCVar(2, "d", &it, &n, &n, &lxr, &lxi);
    ix1 = n << 1;
    CreateVar(3, "d", &cx1, &ix1, &le);
    CreateVar(4, "i", &cx1, &n, &lib);
    CreateVar(5, "d", &cx1, &n, &lw);
    if (it == 0) {
      /*     subroutine bdiag(lda,n,a,epsshr,rMax,er,ei,bs,x,xi,scale,job,fail) */
      C2F(bdiag)(&n, &n, stk(la ), &c_b40, &rMax, stk(le ), stk(le + n ),
		 istk(lib ), stk(lxr ), stk(lxi ), stk(lw ), &cx0, &fail);
    } else {
	C2F(wbdiag)(&n, &n, stk(la ), stk(la + n * n ), &rMax, stk(le ), 
		    stk(le + n ), istk(lib ), stk(lxr ), stk(lxi ), &t, &t, stk(lw ), &cx0, &fail);
    }
    
    if (fail) {
      Scierror(24,"%s:  Non convergence in QR steps.\r\n",fname);
      return 0;
    }
    if (Lhs == 3) {
      nbloc = 0;
      for (k = 1; k <= n; ++k) 
	if (*istk(lib + k - 2 +1) >= 0)  ++nbloc;
      CreateVar(6, "d", &nbloc, &cx1, &lbs);
      ix = 0;
      for (k = 1; k <= n; ++k) {
	if (*istk(lib + k - 2 +1) >= 0) {
	  *stk(lbs + ix ) = (double) *istk(lib + k - 2 +1);
	  ++ix;
	}
      }
    }
    LhsVar(1) = 1;
    LhsVar(2) = 2;
    LhsVar(3) = 6;
    return 0;
} /* intbdiagr_ */


/*-------------------------------------------------------
 * matdsr table 
 *-------------------------------------------------------*/ 

typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

static int C2F(intvoid) (char *fname,unsigned long l) {return 0;}

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} LapackTable;

 
static LapackTable Tab[]={
  {C2F(inthess),"hess"},
  {C2F(intschur),"schur"},
  {C2F(inteig),"spec"},
  {C2F(intbdiagr),"bdiag"},
  {C2F(intvoid),"xxxx"},
  {C2F(intbalanc),"balanc"}
};

int C2F(matdsr)(void)
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
  return 0;
}


