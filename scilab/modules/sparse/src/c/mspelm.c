/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "elementary_functions.h"
#include "int2db.h"
#include "basout.h"

#define CHAR(x)         (cstk(x))
#define INT(x)  	(istk(x))
#define DOUBLE(x)	( stk(x))
#define CMPLX(x)	(zstk(x))

/*     Copyright INRIA */
/*----------------------------------------------------------
 * conversion of Scilab sparse to Matlab sparse 
 * for use in functions interfaced through mex
 * 
 * C2F(intmsparse) : matlab_sparse 
 * 
 * 
 * 
 *---------------------------------------------------------*/

/* Table of constant values */

static int c1 = 1;
static int c_n1 = -1;
static double c_b46 = 0.;

extern  int  C2F(intmfull)(int *id),  C2F(intmspget)(int *id);
extern  int  C2F(intmsparse)(int *id);

static int wmspful(int *ma, int *na, double *ar, double *ai, int *nela, int *inda, double *rr, double *ri);
static int dmspful(int *ma, int *na, double *a, int *nela, int *inda, double *r__);

extern  int C2F(dspt)();
extern  int C2F(wspt)();
extern  int empty(void);

int C2F(intmsparse)(int *id)
{
    /* System generated locals */
    int I1;
    /* Local variables */
    static int tops;
    static int I, l, m, n;
    static int ia, il, it, lr, lw, ilc, nel, ilr, iat, irc, lat, top0;
    static int kkk;
    
    if( Rhs==2) {
      return empty();
      }
    --id;
    /* Function Body */
    Rhs = Max(0,Rhs);
    top0 = Top + 1 - Rhs;
    tops = Top;

    lw = C2F(vstk).lstk[Top];
    if (Lhs != 1) {
	SciError(41);
	return 0;
    }
    if (Rhs != 1) {
	SciError(39);
	return 0;
    }
    il = C2F(vstk).lstk[Top-1] + C2F(vstk).lstk[Top-1] - 1;
    if (*istk(il) == 5) {
	nel = *istk(il + 4);
	m = *istk(il+1);
	n = *istk(il + 2);
	it = *istk(il + 3);
	ilr = il + 5;
	ilc = ilr + m;
	I1 = ilr + m + nel;
	l = I1 / 2 + 1;

	ia = lw + lw - 1;
	iat = ia + m + 1;
	irc = iat + n + 1;
	I1 = irc + n + nel;
	lat = I1 / 2 + 1;
	lw = lat + nel * (it + 1);
	Err = lw - C2F(vstk).lstk[Bot-1];
	if (Err > 0) {
	    SciError(17);
	    return 0;
	}
	*istk(ia) = 1;
	I1 = m;
	for (I = 1; I <= I1; ++I) {
	    *istk(ia + I) = *istk(ia + I - 1) + *istk(ilr + I - 1);
	}
	if (it == 0) {
	    C2F(dspt)(&m, &n, stk(l), &nel, istk(ilr), istk(ia 
		 ), stk(lat), istk(iat), istk(irc));
	} else {
	    C2F(wspt)(&m, &n, stk(l), stk(l + nel), &
		    nel, istk(ilr), istk(ia), stk(lat),
		     stk(lat + nel), istk(iat), istk(irc));
	}
	*istk(il ) = 7;
	I1 = ilr + n + 1 + nel;
	lr = I1 / 2 + 1;
	I1 = n + 1;
	/*    FD  modif  Jc -1 & Ir -1
      	C2F(icopy)(&I1, istk(iat ), &c1, istk(ilr ), &c1);
	C2F(icopy)(&nel, istk(irc + n ), &c1, istk(ilr + n+1), &c1);   */
	for (kkk=0; kkk<I1; ++kkk) *istk(ilr+kkk)=*istk(iat+kkk)-1;
	for (kkk=0; kkk<nel; ++kkk) *istk(ilr+n+1+kkk)=*istk(irc+n+kkk)-1;
	I1 = nel * (it + 1);
	C2F(unsfdcopy)(&I1, stk(lat ), &c1, stk(lr ), &
		c1);
	C2F(vstk).lstk[Top] = lr + nel * (it + 1);
    } else if (*istk(il ) == 7) {
    } else {
	SciError(44);
	return 0;
    }
    return 0;
}

int empty(void)
{
  int m,n;int k;
  int m1,n1,p1;
  int m2,n2,p2;
  int NZMAX=1;int jc=5;int ir;
  int *header;double *value;
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&p1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&p2);
  m= (int) *stk(p1);
  n= (int) *stk(p2);
  CreateData(3, (6+n+1)*sizeof(int)+sizeof(double));
  header = (int *) GetData(3); value= (double *) header;
  header[0]=7;
  header[1]=m;header[2]=n;header[3]=0;
  header[4]=NZMAX;header[jc]=0;
  ir=jc+n+1;
    for (k=0; k<n; ++k) header[jc+k+1]=0;
  header[ir]=0;
  value[(5+header[2]+header[4])/2 + 1] = 0.0;
  LhsVar(1)=3;
  PutLhsVar();
  return 1;
}

/*---------------------------------------
 * %msp_get 
 *---------------------------------------*/

int C2F(intmspget)(int *id)
{
    int I1, I2, I3;

    /* Local variables */
    static int ilrs;
    static int ityp, j, l, m, n;
    static int j1_;
    static int nc, il, it, lv, lw;
    static double tv;
    static int ilc, nel, nelmax, ilr, lij, ilv, top0, kkk;

    /* Parameter adjustments */
    --id;

    /* Function Body */
    Rhs = Max(0,Rhs);
    top0 = Top + 1 - Rhs;
    lw = C2F(vstk).lstk[Top];
    if (Rhs != 1) {
	SciError(39);
	return 0;
    }
    if (Lhs > 3) {
	SciError(41);
	return 0;
    }
    il = C2F(vstk).lstk[Top -1] + C2F(vstk).lstk[Top -1] - 1;
    ityp = *istk(il );
    nelmax = *istk(il + 4);
    m = *istk(il+1);
    n = *istk(il + 2);
    it = *istk(il + 3);
    ilr = il + 5;
    nel = *istk(ilr + n);
    /* printf("mspelm: nelmax,nel %i %i\n", nelmax,nel); */
    ilc = ilr + n + 1;
    I1 = ilc + nelmax;
    l = I1 / 2 + 1;
    if (nel == 0) {
	*istk(il ) = 1;
	*istk(il+1) = 0;
	*istk(il + 2) = 0;
	*istk(il + 3) = 0;
	I1 = il + 4;
	C2F(vstk).lstk[Top] = I1 / 2 + 1;
	if (Lhs >= 2) {
	    ++Top;
	    il = C2F(vstk).lstk[Top -1] + C2F(vstk).lstk[Top -1] - 
		    1;
	    *istk(il ) = 1;
	    *istk(il+1) = 0;
	    *istk(il + 2) = 0;
	    *istk(il + 3) = 0;
	    I1 = il + 4;
	    C2F(vstk).lstk[Top] = I1 / 2 + 1;
	}
	if (Lhs == 3) {
	    ++Top;
	    il = C2F(vstk).lstk[Top -1] + C2F(vstk).lstk[Top -1] - 1;
	    *istk(il ) = 1;
	    *istk(il+1) = 1;
	    *istk(il + 2) = 2;
	    *istk(il + 3) = 0;
	    I1 = il + 4;
	    l = I1 / 2 + 1;
	    *stk(l ) = (double) m;
	    *stk(l+1) = (double) n;
	    C2F(vstk).lstk[Top] = l + 2;
	}
	return 0;
    }
    I1 = il + 4;
    lij = I1 / 2 + 1;
    I1 = lij + (nel << 1);
    ilv = I1 + I1 - 1;
    I1 = ilv + 4;
    lv = I1 / 2 + 1;
    I2 = lw, I3 = lv + nel * (it + 1);
    I1 = Max(I2,I3);
    ilrs = I1 + I1 - 1;
    I1 = ilrs + n + 1 + nel;
    lw = I1 / 2 + 1;
    Err = lw - C2F(vstk).lstk[Bot -1];
    if (Err > 0) {
	SciError(17);
	return 0;
    }
    I1 = n + nel + 1;
    /* FD 1ere colonne de ij = indices en C + 1 */
    for (kkk=0; kkk<I1; ++kkk) *istk(ilrs+kkk)=*istk(ilr+kkk)+1;
    /*    C2F(icopy)(&I1, istk(ilr ), &c1, istk(ilrs ), &c1); */

    /*                    V             */
    if (l >= lv) {
	I1 = nel * (it + 1);
	/*	printf("vvvvvvvvvvvvvvvvvv\n");
	printf("%f\n",stk(l));
	printf("%f\n",stk(l+1));
	printf("%f\n",stk(l+2));
	printf("%f\n",stk(l+3));
	printf("vvvvvvvvvvvvvvvvvv\n"); */
	C2F(unsfdcopy)(&I1, stk(l ), &c1, stk(lv ), &c1);
    } else {
	I1 = nel * (it + 1);
	/* printf("wwwwwwwwwwwwwww\n");
	printf("%f\n",*stk(l));
	printf("%f\n",*stk(l+1));
	printf("%f\n",*stk(l+2));
	printf("%f\n",*stk(l+3));
	printf("wwwwwwwwwwwwwwwwwwww\n"); */
	C2F(unsfdcopy)(&I1, stk(l ), &c_n1, stk(lv ), &c_n1);
    }

    C2F(int2db)(&nel, istk(ilrs + n+1), &c1, stk(lij), &c1);
    for (j = 1; j <= n; ++j) {
	nc = *istk(ilrs + j ) - *istk(ilrs + j - 1);
	j1_ = *istk(ilrs + j - 1) -1;
	tv = (double) j;
	C2F(dset)(&nc, &tv, stk(lij + nel + j1_ ), &c1);
    }

    /*            ij               */
    *istk(il ) = 1;
    *istk(il+1) = nel;
    *istk(il + 2) = 2;
    *istk(il + 3) = 0;
    C2F(vstk).lstk[Top] = lij + (nel << 1);
    if (Lhs >= 2) {
      /*           V              */
	++Top;
	il = C2F(vstk).lstk[Top -1] + C2F(vstk).lstk[Top -1] - 1;
	*istk(il ) = 1;
	*istk(il+1) = nel;
	*istk(il + 2) = 1;
	*istk(il + 3) = it;
	C2F(vstk).lstk[Top] = lv + nel * (it + 1);
    }
    if (Lhs == 3) {
      /*            mn             */
	++Top;
	il = C2F(vstk).lstk[Top -1] + C2F(vstk).lstk[Top -1] - 1;
	*istk(il ) = 1;
	*istk(il+1) = 1;
	*istk(il + 2) = 2;
	*istk(il + 3) = 0;
	I1 = il + 4;
	l = I1 / 2 + 1;
	*stk(l ) = (double) m;
	*stk(l+1) = (double) n;
	C2F(vstk).lstk[Top] = l + 2;
    }
    return 0;
} 

/*---------------------------------------
 * %msp_full 
 *---------------------------------------*/

int C2F(intmfull)(int *id)
{
    int I1, I2, I3;

    /* Local variables */
    static int l, m, n;
    static int il, it, ls, lw, ilc, nel, ilr, ils, kkk;
    static int top0;

    /* Parameter adjustments */
    --id;

    /* Function Body */
    Rhs = Max(0,Rhs);
    top0 = Top + 1 - Rhs;
    lw = C2F(vstk).lstk[Top];
    if (Rhs != 1) {
	SciError(39);
	return 0;
    }
    if (Lhs != 1) {
	SciError(41);
	return 0;
    }
    il = C2F(vstk).lstk[Top -1] + C2F(vstk).lstk[Top -1] - 1;
    nel = *istk(il + 4);
    m = *istk(il+1);
    n = *istk(il + 2);
    it = *istk(il + 3);
    ilr = il + 5;
    ilc = ilr + n + 1;
    I1 = ilc + nel;
    l = I1 / 2 + 1;
    I1 = il + 4;
    /* Computing MAX */
    I3 = I1 / 2 + 1 + m * n * (it + 1);
    I2 = Max(I3,lw);
    ils = I2 + I2 - 1;
    I1 = ils + n + 1 + nel;
    ls = I1 / 2 + 1;
    lw = ls + nel * (it + 1);
    Err = lw - C2F(vstk).lstk[Bot-1];
    if (Err > 0) {
	SciError(17);
	return 0;
    }
    I1 = n + 1 + nel;
    /*     FD modif  */
    for (kkk=0; kkk<I1; ++kkk) *istk(ils+kkk)=*istk(ilr+kkk)+1;
    /* C2F(icopy)(&I1, istk(ilr ), &c1, istk(ils ), &c1); */
    I1 = nel * (it + 1);
    C2F(unsfdcopy)(&I1, stk(l ), &c1, stk(ls ), &c1);
    *istk(il ) = 1;
    I1 = il + 4;
    l = I1 / 2 + 1;
    if (it == 0) {
	dmspful(&m, &n, stk(ls ), &nel, istk(ils ), stk(l ));
    } else {
      wmspful(&m, &n, stk(ls ), stk(ls + nel ), &nel,istk(ils ), 
	      stk(l ), stk(l + m *n )); 
    }
    C2F(vstk).lstk[Top] = l + m * n * (it + 1);
    return 0;
}

static int dmspful(int *ma, int *na, double *a, int *nela, int *inda, double *rr)
{
    int I1, I2;
    static int I, j, k, ii, nj;

    I1 = *ma * *na;
    C2F(dset)(&I1, &c_b46,rr, &c1);
    k = 0;
    I1 = *na;
    for (j = 1; j <= I1; ++j) {
	nj = inda[j] - inda[j-1];
	if (nj > 0) {
	    I2 = nj;
	    for (ii = 1; ii <= I2; ++ii) {
		I = inda[*na  + k + ii];
		rr[I + (j - 1) * *ma -1 ] = a[k + ii -1];
	    }
	    k += nj;
	}
    }
    return 0;
} 

static int wmspful(int *ma, int *na, double *ar, double *ai, int *nela, int *inda, double *rr, double *ri)
{
  int I1, I2;
  static int I, j, k, ii, nj;

  I1 = *ma * *na;
  C2F(dset)(&I1, &c_b46, rr, &c1);
  I1 = *ma * *na;
  C2F(dset)(&I1, &c_b46, ri, &c1);
  k = 0;
  I1 = *na;
  for (j = 1; j <= I1; ++j) {
    nj = inda[j] - inda[j-1];
    if (nj > 0) {
      I2 = nj;
      for (ii = 1; ii <= I2; ++ii) {
	I = inda[*na + k + ii];
	rr[I + (j - 1) * *ma -1] = ar[k + ii -1];
	ri[I + (j - 1) * *ma -1] = ai[k + ii -1];
      }
      k += nj;
    }
  }
  return 0;
}


