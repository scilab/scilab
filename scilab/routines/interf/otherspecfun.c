/*
 *   PURPOSE  
 *      Scilab interface onto some special mathematical 
 *      functions, currently:
 *
 *        1/ legendre associated function
 *        2/ beta function (real positive case)
 *        3/ bessels functions
 *
 *   AUTHORS
 *      Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> (legendre, beta)
 *      Serge Steer <Serge.Steer@inria.fr> (bessel)
 *             
 */
#include <string.h> /*pour strcmp */
#include <math.h>
#include <setjmp.h>
#include "../stack-c.h"



#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) < (b) ? (b) : (a))



/* fortran functions headers */
double F2C(dgammacody)(double *);
double F2C(betaln)(double *, double *);
extern void  C2F(dbeskv) (double *x,int* nx, double *alpha, int *na, int *kode, 
			  double *r, double *w, int *ierr);
extern void  C2F(zbeskv) (double *xr,double *xi,int* nx, double *alpha, int *na, 
			  int *kode, double *rr,double *ri, double *wr, double *wi, int *ierr);
extern void  C2F(dbesiv) (double *x,int* nx, double *alpha, int *na, int *kode, 
			  double *r, double *w, int *ierr);
extern void  C2F(zbesiv) (double *xr,double *xi,int* nx, double *alpha, int *na, 
			  int *kode, double *rr,double *ri, double *wr, double *wi, int *ierr);
extern void  C2F(dbesjv) (double *x,int* nx, double *alpha, int *na, int *kode, 
			  double *r, double *w, int *ierr);
extern void  C2F(zbesjv) (double *xr,double *xi,int* nx, double *alpha, int *na, 
			  int *kode, double *rr,double *ri, double *wr, double *wi, int *ierr);
extern void  C2F(dbesyv) (double *x,int* nx, double *alpha, int *na, int *kode, 
			  double *r, double *w, int *ierr);
extern void  C2F(zbesyv) (double *xr,double *xi,int* nx, double *alpha, int *na, 
			  int *kode, double *rr,double *ri, double *wr, double *wi, int *ierr);
extern void  C2F(zbeshv) (double *xr,double *xi,int* nx, double *alpha, int *na, 
			  int *kode, int *K, double *rr,double *ri, double *wr, double *wi, int *ierr);

extern void  C2F(dxlegf)(double *dnu1, int *nudiff, int *mu1, int *mu2, double *x, 
			int *id, double *pqa, int *ipqa, int *ierror);
extern void C2F(msgs)(int *n, int* ierr);
extern void C2F(dset)(int *n, double *a,double *x,int *ix);
extern jmp_buf slatec_jmp_env; 
static int verify_cstr(double x[], int nb_elt, int *xmin, int *xmax)
{
  /*    1/ verify that the array x is formed by non negative integers
   *       regularly spaced with increment equal to 1 (if yes return 1
   *       if not return 0)
   *    2/ computes the min and the max
   *
   *    AUTHOR 
   *       Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
   */
  int i;
  if ( ! (floor(x[0]) == x[0]  &&  x[0] >= 0 ))
    return 0;
  for ( i = 1 ; i < nb_elt ; i++ )
    if ( x[i] != x[i-1]+1.0 )
      return 0;

  *xmin = (int) x[0];
  *xmax = (int) x[nb_elt-1];
  return 1;
} 

static double return_an_inf()
{
   /*    AUTHOR 
    *       Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
    */
  static int first = 1;
  static double inf = 1.0;

  if ( first )
    {
      inf = inf/(inf - (double) first);
      first = 0;
    }
  return (inf);
}

static int intlegendre(char *fname,unsigned long fname_len)
{
  /*
   *   Interface onto the (Slatec) dxleg.f code. 
   *   Scilab calling sequence :
   *
   *   p = legendre(n, m, x [, norm_flag] )
   *
   *      x is a vector with mnx elements (it is better to
   *        have a row vector but this is not forced)
   *
   *      n : a non negative integer scalar (or a vector of such
   *          integer regularly speced with an increment of 1)
   *      m : same constraints than for n
   *
   *      n and m may not be both vectors
   *
   *      norm_flag : optionnal. When it is present and equal to "norm"
   *                  it is a normalised version which is computed
   *    AUTHOR 
   *       Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
   */
  int it, lc, mM, nM, lM, m1, m2, mN, nN, lN, n1, n2, mx, nx, lx, mnx, ms, ns, ls;
  int M_is_scalar = 0, N_is_scalar = 0, normalised, MNp1, lpqa, lipqa, *ipqa;
  double *x, xx, dnu1, *pqa;
  int id, ierror, i, j, nudiff;

  CheckLhs(1,1); CheckRhs(3,4);
  GetRhsVar(1, "d", &mN, &nN, &lN);

  if ( ! verify_cstr(stk(lN), mN*nN, &n1, &n2) )
    {
      Scierror(999,"%s: bad first argument\r\n", fname);
      return 0;
    };
  if ( mN == 1 && nN == 1) N_is_scalar = 1;

  GetRhsVar(2, "d", &mM, &nM, &lM);
  if ( ! verify_cstr(stk(lM), mM*nM, &m1, &m2) )
    {
      Scierror(999,"%s: bad second argument\r\n", fname);
      return 0;
    }
  if ( mM == 1 && nM == 1) M_is_scalar = 1;

  if ( ! M_is_scalar  &&  ! N_is_scalar )
    {
      Scierror(999,"%s: only one of arg1 and arg2 may be a vector\r\n", fname);
      return 0;
    };
       
  GetRhsCVar(3,"d", &it, &mx, &nx, &lx, &lc);
  if ( it != 0 )
    {
      Scierror(999,"%s: 3th argument must be a real matrix\r\n", fname);
      return 0;
    };

  mnx = mx*nx;
  x = stk(lx);
  for ( i = 0 ; i < mnx ; i++ )
    if ( ! (fabs(x[i]) < 1.0) ) 
      {
	Scierror(999,"%s: 3th argument must be a matrix with elements in (-1,1)\r\n", fname);
	return 0;
      };
  
  if ( Rhs == 4 )
    {
      GetRhsVar(4,"c", &ms, &ns, &ls);
      if ( strcmp(cstk(ls),"norm") == 0)
	normalised = 1;
      else
	normalised = 0;
    }
  else
    normalised = 0;
  
  MNp1 = max (n2 - n1, m2 - m1) + 1;

  CreateVar(Rhs+1, "d", &MNp1, &mnx, &lpqa); pqa = stk(lpqa);
  CreateVar(Rhs+2, "i", &MNp1, &mnx, &lipqa); ipqa = istk(lipqa);

  if ( normalised )
    id = 4;
  else
    id = 3;

  nudiff = n2-n1;  
  dnu1 = (double) n1;

  for ( i = 0 ; i < mnx ; i++ )
    {
      xx = fabs(x[i]); /* dxleg computes only for x in [0,1) */
      F2C(dxlegf) (&dnu1, &nudiff, &m1, &m2, &xx, &id, 
		   stk(lpqa+i*MNp1), istk(lipqa+i*MNp1), &ierror);
      if ( ierror != 0 )
	{
	  if ( ierror == 207 )
	    Scierror(999,"%s: overflow or underflow of an extended range number\r\n", fname);
	  else
	    Scierror(999,"%s: error number %d\r\n", fname, ierror);
	  return 0;
	};
    }

  /*  dxlegf returns the result under a form (pqa,ipqa) (to 
   *  compute internaly with an extended exponent range)
   *  When the "exponent" part (ipqa) is 0 then the number is exactly
   *  given by pqa else it leads to an overflow or an underflow.
   */
  for ( i = 0 ; i < mnx*MNp1 ; i++ )
    {
      if ( ipqa[i] < 0 ) 
	pqa[i] = 0.0;
      if ( ipqa[i] > 0 )
	pqa[i] = pqa[i] * return_an_inf(); /* pqa[i] * Inf  to have the sign */
    }

  /* complete the result by odd/even symmetry for negative x */
  for ( i = 0 ; i < mnx ; i++ ) {
    if ( x[i] < 0.0 ) {
      if ( (n1+m1) % 2 == 1 ) {
	for ( j = 0 ; j < MNp1 ; j+=2 )
	  pqa[i*MNp1 + j] = -pqa[i*MNp1 + j];
      }
      else {
	for ( j = 1 ; j < MNp1 ; j+=2 )
	  pqa[i*MNp1 + j] = -pqa[i*MNp1 + j];
      }
    }
  }
  LhsVar(1) = Rhs+1;
  return 0;
}

static int intbeta(char *fname,unsigned long fname_len)
{
  /*
   *   z = beta(x, y)
   *
   *      x, y : matrices of the same size of positive reals
   *
   *   For small x+y values uses the expression with the
   *   gamma function, else the exponential applied on the log beta
   *   function (provided in the dcd lib)
   *
   *   The switch limit have been set by using the gp-pari software.
   *
   *    AUTHOR 
   *       Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
   *
   */
  int mx, nx, itx, lx, lxc, my, ny, ity, ly, lyc,/* it,*/ lz, i;
  double *x, *y, *z, xpy;
  double switch_limit = 2;

  CheckLhs(1,1); CheckRhs(2,2);
  GetRhsCVar(1, "d", &itx, &mx, &nx, &lx, &lxc); x = stk(lx);
  GetRhsCVar(2, "d", &ity, &my, &ny, &ly, &lyc); y = stk(ly);
  CheckSameDims(1,2,mx,nx,my,ny);
  if ( itx == 1  ||  ity == 1 )
    {
      Scierror(999,"%s don't work for complex arguments \r\n", fname);
      return 0;
    };

  CreateVar(3, "d", &mx, &nx, &lz); z = stk(lz);

  for ( i = 0 ; i < mx*nx ; i++ )
    {
      if ( x[i] <= 0.0  ||  y[i] <= 0.0 )
	{
	  Scierror(999,"%s: arguments must be positive \r\n", fname);
	  return 0;
	}
      xpy = x[i] + y[i];
      if ( xpy <= switch_limit )
	z[i] = F2C(dgammacody)(&x[i]) * F2C(dgammacody)(&y[i]) / F2C(dgammacody)(&xpy);
      else
	z[i] = exp(F2C(betaln)(&x[i], &y[i]));
    }
  LhsVar(1) = 3;
  return 0;
}


int intbesselk(char *fname,unsigned long fname_len)
/* Author Serge Steer, Copyright INRIA 2005 */
{
  int m1,n1,l1,m2,n2,it2,l2,l2r,l2i,mr,nr,itr,lr,li,lwi,lwr;
  int r1,r2,na,nx,kode,lpos;
  int ispos ,i;
  int un=1,nl2,ierr;
  double zero=0.0;

  static int *Lstk    = C2F(vstk).lstk-1;

  CheckRhs(2,3);

  kode = 1;
  if (Rhs==3) { /* normalized bessel required */
    GetRhsVar(3,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
    kode = (int)*stk(l1)+1;
  }

  GetRhsVar(1,"d",&m1,&n1,&l1); /* get alpha */
  r1=l1>Lstk[Bot]; /* true if the variable has been passed by reference */

  GetRhsCVar(2,"d",&it2,&m2,&n2,&l2,&l2i); /* get x */
  r2=l2>Lstk[Bot];/* true if the variable has been passed by reference */
  
  if (m1*n1 == 0) { /*besseli([],x) */
    LhsVar(1)=1;
    return 0;
  }
  if (m2*n2 == 0) { /*besseli(alpha,[]) */
    LhsVar(1)=2;
    return 0;
  }
  /* determine if the result is real or complex */
  itr=it2;
  if (itr==0) {
    ispos=1;
    for (i=0;i<m2*n2;i++) {
      if (*stk(l2+i) < 0.0) {ispos=0;break;}
    }
    if (ispos==0) itr=1;
  }

  if (itr==1&&it2==0) { /* transform to complex */
    nl2=m2*n2;
    CreateCVar(3,"d",&itr,&m2,&n2,&l2r,&l2i);
    C2F(dcopy)(&nl2,stk(l2),&un,stk(l2r),&un);
    l2=l2r;
    C2F(dset)(&nl2,&zero,stk(l2i),&un);
    lpos=3;
    r2=0;
  }
  else {
    l2i=l2+m2*n2;
    lpos=2;
  }

  if (m1*n1 == 1) { /*besseli(scalar,matrix) */
    double wr[2],wi[2];
    mr=m2;
    nr=n2;    
    if (r2) { 
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else { 
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=m2*n2;
    na=1;
    if (itr==0)
      C2F(dbeskv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),wr,&ierr);
    else
      C2F(zbeskv) (stk(l2),stk(l2i),&nx,stk(l1),&na, &kode,stk(lr),stk(li),wr,wi,&ierr);
  }
  else if (m2*n2 == 1) { /* besseli(matrix,scalar) */
    int lwr,lwi;
    mr=m1;
    nr=n1;   
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=1;
    na=m1*n1;
    CreateCVar(lpos+2,"d",&itr,&nx,&na,&lwr,&lwi);
    if (itr==0)
      C2F(dbeskv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbeskv) (stk(l2),stk(l2i),&nx,stk(l1),&na, &kode,stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else if ((m1==1 && n2==1)|| (n1==1 && m2==1)) { /* besseli(row,col) or besseli(col,row) */
    int un=1;
    mr=m2*n2;
    nr=m1*n1;
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=m2*n2;
    na=m1*n1;
    CreateCVar(lpos+2,"d",&itr,&un,&na,&lwr,&lwi);
    if (itr==0)
      C2F(dbeskv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbeskv) (stk(l2),stk(l2i),&nx,stk(l1),&na, &kode,stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else { /* element wise case */
    double wr[2],wi[2];
    CheckDimProp(1,2,m1*n1!=m2*n2) 
    mr=m2;
    nr=n2;  
    if (r2) {
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else {
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=mr*nr;
    na=-1;
    if (itr==0)
      C2F(dbeskv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),wr,&ierr);
    else
      C2F(zbeskv) (stk(l2),stk(l2i),&nx,stk(l1),&na, &kode,stk(lr),stk(li),wr,wi,&ierr);
 
  }
  if (ierr==2) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=63;
     C2F(msgs)(&ierr,&un);
 
    }
  }
  else if (ierr==3) {/* inacurate result */
    ierr=4;
    C2F(msgs)(&ierr,&un);
  }
  else if (ierr==4||ierr==5) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=107;
     C2F(msgs)(&ierr,&un);
    }
 }
 

  return 0;
}  

int intbesseli(char *fname,unsigned long fname_len)
/* Author Serge Steer, Copyright INRIA 2005 */
     
{
  int m1,n1,l1,m2,n2,it2,l2,l2r,l2i,mr,nr,itr,lr,li,lwi,lwr;
  int r1,r2,na,nx,kode,lpos;
  int isint, ispos ,i,t;
  int un=1,nl2,ierr;
  double zero=0.0;

  static int *Lstk    = C2F(vstk).lstk-1;

  CheckRhs(2,3);

  kode = 1;
  if (Rhs==3) { /* normalized bessel required */
    GetRhsVar(3,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
    kode = (int)*stk(l1)+1;
  }

  GetRhsVar(1,"d",&m1,&n1,&l1); /* get alpha */
  r1=l1>Lstk[Bot]; /* true if the variable has been passed by reference */

  GetRhsCVar(2,"d",&it2,&m2,&n2,&l2,&l2i); /* get x */
  r2=l2>Lstk[Bot];/* true if the variable has been passed by reference */
  
  if (m1*n1 == 0) { /*besseli([],x) */
    LhsVar(1)=1;
    return 0;
  }
  if (m2*n2 == 0) { /*besseli(alpha,[]) */
    LhsVar(1)=2;
    return 0;
  }
  /* determine if the result is real or complex */
  itr=it2;
  if (itr==0) {
    isint=1; ispos=1;
    for (i=0;i<m1*n1;i++) {
      t=(int)*stk(l1+i);
      if (t != *stk(l1+i)) {isint=0;break;}
    }
    if (isint==0) {
      for (i=0;i<m2*n2;i++) {
	if (*stk(l2+i) < 0.0) {ispos=0;break;}
      }
    }
    if (ispos==0) itr=1;
  }

  if (itr==1&&it2==0) { /* transform to complex */
    nl2=m2*n2;
    CreateCVar(3,"d",&itr,&m2,&n2,&l2r,&l2i);
    C2F(dcopy)(&nl2,stk(l2),&un,stk(l2r),&un);
    l2=l2r;
    C2F(dset)(&nl2,&zero,stk(l2i),&un);
    lpos=3;
    r2=0;
  }
  else {
    l2i=l2+m2*n2;
    lpos=2;
  }

  if (m1*n1 == 1) { /*besseli(scalar,matrix) */
    double wr[2],wi[2];
    mr=m2;
    nr=n2;    
    if (r2) { 
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else { 
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=m2*n2;
    na=1;
    if (itr==0)
      C2F(dbesiv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),wr,&ierr);
    else
      C2F(zbesiv) (stk(l2),stk(l2i),&nx,stk(l1),&na, &kode,stk(lr),stk(li),wr,wi,&ierr);
  }
  else if (m2*n2 == 1) { /* besseli(matrix,scalar) */
    int lwr,lwi,nw;
    mr=m1;
    nr=n1;   
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=1;
    na=m1*n1;
    nw=2*na;
    CreateCVar(lpos+2,"d",&itr,&nx,&nw,&lwr,&lwi);
    if (itr==0)
      C2F(dbesiv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbesiv) (stk(l2),stk(l2i),&nx,stk(l1),&na, &kode,stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else if ((m1==1 && n2==1)|| (n1==1 && m2==1)) { /* besseli(row,col) or besseli(col,row) */
    int un=1,nw;
    mr=m2*n2;
    nr=m1*n1;
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=m2*n2;
    na=m1*n1;
    nw=2*na;
    CreateCVar(lpos+2,"d",&itr,&un,&nw,&lwr,&lwi);
    if (itr==0)
      C2F(dbesiv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbesiv) (stk(l2),stk(l2i),&nx,stk(l1),&na, &kode,stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else { /* element wise case */
    double wr[2],wi[2];
    CheckDimProp(1,2,m1*n1!=m2*n2) 
    mr=m2;
    nr=n2;  
    if (r2) {
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else {
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=mr*nr;
    na=-1;
    if (itr==0)
      C2F(dbesiv) (stk(l2),&nx,stk(l1),&na, &kode,stk(lr),wr,&ierr);
    else
      C2F(zbesiv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,stk(lr),stk(li),wr,wi,&ierr);
 
  }
  if (ierr==2) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=63;
     C2F(msgs)(&ierr,&un);
 
    }
  }
  else if (ierr==3) {/* inacurate result */
    ierr=4;
    C2F(msgs)(&ierr,&un);
  }
  else if (ierr==4||ierr==5) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=107;
     C2F(msgs)(&ierr,&un);
    }
 }
 

  return 0;
}  

int intbesselj(char *fname,unsigned long fname_len)
/* Author Serge Steer, Copyright INRIA 2005 */
{
  int m1,n1,l1,m2,n2,it2,l2,l2r,l2i,mr,nr,itr,lr,li,lwi,lwr;
  int r1,r2,na,nx,kode,lpos;
  int isint, ispos ,i,t;
  int un=1,nl2,ierr;
  double zero=0.0;

  static int *Lstk    = C2F(vstk).lstk-1;

  CheckRhs(2,3);
  kode = 1; /* ignored for real cases */
  if (Rhs==3) { /* normalized bessel required */
    GetRhsVar(3,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
    kode = (int)*stk(l1)+1;
  }

  GetRhsVar(1,"d",&m1,&n1,&l1); /* get alpha */
  r1=l1>Lstk[Bot]; /* true if the variable has been passed by reference */

  GetRhsCVar(2,"d",&it2,&m2,&n2,&l2,&l2i); /* get x */
  r2=l2>Lstk[Bot];/* true if the variable has been passed by reference */
  
  if (m1*n1 == 0) { /*besselj([],x) */
    LhsVar(1)=1;
    return 0;
  }
  if (m2*n2 == 0) { /*besselj(alpha,[]) */
    LhsVar(1)=2;
    return 0;
  }
  /* determine if the result is real or complex */
  itr=it2;
  if (itr==0) {
    isint=1;ispos=1;
    for (i=0;i<m1*n1;i++) {
      t=(int)*stk(l1+i);
      if (t != *stk(l1+i)) {isint=0;break;}
    }
    if (isint==0) {
      for (i=0;i<m2*n2;i++) {
	if (*stk(l2+i) < 0.0) {ispos=0;break;}
      }
    }
    if (ispos==0) itr=1;
  }

  if (itr==1&&it2==0) { /* transform to complex */
    nl2=m2*n2;
    CreateCVar(3,"d",&itr,&m2,&n2,&l2r,&l2i);
    C2F(dcopy)(&nl2,stk(l2),&un,stk(l2r),&un);
    l2=l2r;
    C2F(dset)(&nl2,&zero,stk(l2i),&un);
    lpos=3;
    r2=0;
  }
  else {
    l2i=l2+m2*n2;
    lpos=2;
  }

  if (m1*n1 == 1) { /*besselj(scalar,matrix) */
    double wr[3],wi[3];
    mr=m2;
    nr=n2;    
    if (r2) { 
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else { 
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=m2*n2;
    na=1;
    if (itr==0)
      C2F(dbesjv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),wr,&ierr);
    else
      C2F(zbesjv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,stk(lr),stk(li),wr,wi,&ierr);
  }
  else if (m2*n2 == 1) { /* besselj(matrix,scalar) */
    int lwr,lwi,nw;
    mr=m1;
    nr=n1;   
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=1;
    na=m1*n1;
    nw=3*na;
    CreateCVar(lpos+2,"d",&itr,&nx,&nw,&lwr,&lwi);
    if (itr==0)
      C2F(dbesjv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbesjv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else if ((m1==1 && n2==1)|| (n1==1 && m2==1)) { /* besselj(row,col) or besselj(col,row) */
    int un=1,nw;
    mr=m2*n2;
    nr=m1*n1;
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=m2*n2;
    na=m1*n1;
    nw=3*na;
    CreateCVar(lpos+2,"d",&itr,&un,&nw,&lwr,&lwi);
    if (itr==0)
      C2F(dbesjv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbesjv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode, stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else { /* element wise case */
    double wr[2],wi[2];
    CheckDimProp(1,2,m1*n1!=m2*n2) 
    mr=m2;
    nr=n2;  
    if (r2) {
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else {
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=mr*nr;
    na=-1;
    if (itr==0)
      C2F(dbesjv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),wr,&ierr);
    else
      C2F(zbesjv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,stk(lr),stk(li),wr,wi,&ierr);
 
  }
  if (ierr==2) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=63;
     C2F(msgs)(&ierr,&un);
 
    }
  }
  else if (ierr==3) {/* inacurate result */
    ierr=4;
    C2F(msgs)(&ierr,&un);
  }
  else if (ierr==4||ierr==5) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=107;
     C2F(msgs)(&ierr,&un);
    }
 }
 

  return 0;
}  

int intbessely(char *fname,unsigned long fname_len)
/* Author Serge Steer, Copyright INRIA 2005 */
{
  int m1,n1,l1,m2,n2,it2,l2,l2r,l2i,mr,nr,itr,lr,li,lwi,lwr;
  int r1,r2,na,nx,kode,lpos;
  int ispos ,i;
  int un=1,nl2,ierr;
  double zero=0.0;

  static int *Lstk    = C2F(vstk).lstk-1;

 
  CheckRhs(2,3);
  kode = 1; /* ignored for real cases */
  if (Rhs==3) { /* normalized bessel required */
    GetRhsVar(3,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
    kode = (int)*stk(l1)+1;
  }


  GetRhsVar(1,"d",&m1,&n1,&l1); /* get alpha */
  r1=l1>Lstk[Bot]; /* true if the variable has been passed by reference */

  GetRhsCVar(2,"d",&it2,&m2,&n2,&l2,&l2i); /* get x */
  r2=l2>Lstk[Bot];/* true if the variable has been passed by reference */
  
  if (m1*n1 == 0) { /*bessely([],x) */
    LhsVar(1)=1;
    return 0;
  }
  if (m2*n2 == 0) { /*bessely(alpha,[]) */
    LhsVar(1)=2;
    return 0;
  }
  /* determine if the result is real or complex */
  itr=it2;
  if (itr==0) {
    ispos=1;
    for (i=0;i<m2*n2;i++) {
      if (*stk(l2+i) < 0.0) {ispos=0;break;}
    }
    if (ispos==0) itr=1;
  }

  if (itr==1&&it2==0) { /* transform to complex */
    nl2=m2*n2;
    CreateCVar(3,"d",&itr,&m2,&n2,&l2r,&l2i);
    C2F(dcopy)(&nl2,stk(l2),&un,stk(l2r),&un);
    l2=l2r;
    C2F(dset)(&nl2,&zero,stk(l2i),&un);
    lpos=3;
    r2=0;
  }
  else {
    l2i=l2+m2*n2;
    lpos=2;
  }

  if (m1*n1 == 1) { /*bessely(scalar,matrix) */
    double wr[3],wi[3];
    mr=m2;
    nr=n2;    
    if (r2) { 
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else { 
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=m2*n2;
    na=1;
    if (itr==0)
      C2F(dbesyv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),wr,&ierr);
    else
      C2F(zbesyv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,stk(lr),stk(li),wr,wi,&ierr);
  }
  else if (m2*n2 == 1) { /* bessely(matrix,scalar) */
    int lwr,lwi,nw;
    mr=m1;
    nr=n1;   
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=1;
    na=m1*n1;
    nw=3*na;
    CreateCVar(lpos+2,"d",&itr,&nx,&nw,&lwr,&lwi);
    if (itr==0)
      C2F(dbesyv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbesyv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else if ((m1==1 && n2==1)|| (n1==1 && m2==1)) { /* bessely(row,col) or bessely(col,row) */
    int un=1,nw;
    mr=m2*n2;
    nr=m1*n1;
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=m2*n2;
    na=m1*n1;
    nw=3*na;
    CreateCVar(lpos+2,"d",&itr,&un,&nw,&lwr,&lwi);
    if (itr==0)
      C2F(dbesyv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),stk(lwr),&ierr);
    else
      C2F(zbesyv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode, stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else { /* element wise case */
    double wr[2],wi[2];
    CheckDimProp(1,2,m1*n1!=m2*n2) 
    mr=m2;
    nr=n2;  
    if (r2) {
      CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
      LhsVar(1)=lpos+1;
    }
    else {
      lr=l2;
      li=l2i;
      LhsVar(1)=lpos;
    }
    nx=mr*nr;
    na=-1;
    if (itr==0)
      C2F(dbesyv) (stk(l2),&nx,stk(l1),&na,&kode,stk(lr),wr,&ierr);
    else
      C2F(zbesyv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,stk(lr),stk(li),wr,wi,&ierr);
 
  }
  if (ierr==2) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=63;
     C2F(msgs)(&ierr,&un);
 
    }
  }
  else if (ierr==3) {/* inacurate result */
    ierr=4;
    C2F(msgs)(&ierr,&un);
  }
  else if (ierr==4||ierr==5) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=107;
     C2F(msgs)(&ierr,&un);
    }
 }
 

  return 0;
}  

int intbesselh(char *fname,unsigned long fname_len)
/* Author Serge Steer, Copyright INRIA 2005 */
{
  int m1,n1,l1,m2,n2,it2,l2,/*l2r,*/l2i,mr,nr,itr,lr,li,lwi,lwr,rhs1;
  int r1,r2,na,nx,kode,lpos;
  int un=1,ierr;
  int K;

  static int *Lstk    = C2F(vstk).lstk-1;

 
  CheckRhs(2,4);
  kode = 1;
  rhs1 = Rhs;
  if (Rhs==4) { /* normalized bessel required */
    GetRhsVar(4,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
    kode = (int)*stk(l1)+1;
    rhs1--;
  }
  K = 1; 
  if (Rhs > 2) { /* normalized bessel required */
    GetRhsVar(2,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
    K = (int)*stk(l1);
  }

  GetRhsVar(1,"d",&m1,&n1,&l1); /* get alpha */
  r1=l1>Lstk[Bot]; /* true if the variable has been passed by reference */

  GetRhsCVar(rhs1,"d",&it2,&m2,&n2,&l2,&l2i); /* get x */
  r2=l2>Lstk[Bot];/* true if the variable has been passed by reference */
  
  if (m1*n1 == 0) { /*besselh([],x) */
    LhsVar(1)=1;
    return 0;
  }
  if (m2*n2 == 0) { /*besselh(alpha,[]) */
    LhsVar(1)=rhs1;
    return 0;
  }

  itr=1;
  lpos=Rhs;
  if(it2 == 0) {
    int i;
    CreateVar(lpos+1,"d",&m2,&n2,&l2i);
    for (i=0;i<m2*n2;i++) *stk(l2i+i)=0.0;
    lpos=lpos+1;
  }
  if (m1*n1 == 1) { /*bessely(scalar,matrix) */
    double wr[3],wi[3];
    mr=m2;
    nr=n2;    
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    LhsVar(1)=lpos+1;
 
    nx=m2*n2;
    na=1;
    C2F(zbeshv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,&K,stk(lr),stk(li),wr,wi,&ierr);
  }

  else if (m2*n2 == 1) { /* bessely(matrix,scalar) */
    int lwr,lwi,nw;
    mr=m1;
    nr=n1;   
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=1;
    na=m1*n1;
    nw=3*na;
    CreateCVar(lpos+2,"d",&itr,&nx,&nw,&lwr,&lwi);
    C2F(zbeshv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,&K,stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else if ((m1==1 && n2==1)|| (n1==1 && m2==1)) { /* bessely(row,col) or bessely(col,row) */
    int un=1,nw;
    mr=m2*n2;
    nr=m1*n1;
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    nx=m2*n2;
    na=m1*n1;
    nw=3*na;
    CreateCVar(lpos+2,"d",&itr,&un,&nw,&lwr,&lwi);
    C2F(zbeshv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,&K, stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

    LhsVar(1)=lpos+1;
  }
  else { /* element wise case */
    double wr[2],wi[2];
    CheckDimProp(1,2,m1*n1!=m2*n2) 
    mr=m2;
    nr=n2;  
    CreateCVar(lpos+1,"d",&itr,&mr,&nr,&lr,&li);
    LhsVar(1)=lpos+1;
    nx=mr*nr;
    na=-1;
    C2F(zbeshv) (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,&K,stk(lr),stk(li),wr,wi,&ierr);
  }
  if (ierr==2) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=63;
     C2F(msgs)(&ierr,&un);
 
    }
  }
  else if (ierr==3) {/* inacurate result */
    ierr=4;
    C2F(msgs)(&ierr,&un);
  }
  else if (ierr==4||ierr==5) {
    if ( C2F(errgst).ieee==0) { 
      ierr=69;
      C2F(error)(&ierr);
    }
    else if ( C2F(errgst).ieee==1) {
     ierr=107;
     C2F(msgs)(&ierr,&un);
    }
 }
 

  return 0;
}  


static TabF Tab[]={ 
  {intlegendre, "legendre"},
  {intbeta, "beta"},
  {intbesseli,"besseli"},
  {intbesselj,"besselj"},
  {intbesselk,"besselk"},
  {intbessely,"bessely"},
  {intbesselh,"besselh"}
};

int C2F(otherspfunlib)(void)
{
  Rhs = Max(0, Rhs);
 if (setjmp(slatec_jmp_env)) { 
    Scierror(999,"%s: arguments must be positive \r\n", Tab[Fin-1].name);
    return 0;
  }
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
  return 0;
}
