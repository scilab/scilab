#include "../stack-c.h"
/*     Copyright INRIA */

/* Table of constant values */

static integer c1 = 1;
static integer c2 = 2;
static integer c41 = 41;
static integer c39 = 39;
static integer c17 = 17;
static integer c44 = 44;
static integer c_n1 = -1;
static double c_b46 = 0.;

extern  int  C2F(intmfull)(),  C2F(intmspget)();
static integer id[6];
extern  int  F2C(basout)(),  C2F(intmsparse)();


int C2F(intmsparse)(id)
integer *id;
{
    /* System generated locals */
    integer I1;

    /* Local variables */
    extern  int F2C(dspt)();
    static integer tops;
    extern  int F2C(wspt)();
    static integer I, l, m, n;
    extern  int F2C(unsfdcopy)(), F2C(icopy)(), F2C(error)();
    static integer ia, il, it, lr, lw, ilc, nel, ilr, iat, irc, lat, top0;
    static integer kkk;
    /* Parameter adjustments */
    --id;

    /* Function Body */
    Rhs = Max(0,Rhs);
    top0 = Top + 1 - Rhs;
    tops = Top;

    lw = C2F(vstk).Lstk[Top];
    if (Lhs != 1) {
	F2C(error)(&c41);
	return 0;
    }
    if (Rhs != 1) {
	F2C(error)(&c39);
	return 0;
    }
    il = C2F(vstk).Lstk[Top-1] + C2F(vstk).Lstk[Top-1] - 1;
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
	Err = lw - C2F(vstk).Lstk[Bot-1];
	if (Err > 0) {
	    F2C(error)(&c17);
	    return 0;
	}
	*istk(ia) = 1;
	I1 = m;
	for (I = 1; I <= I1; ++I) {
	    *istk(ia + I) = *istk(ia + I - 1) + *istk(ilr + I - 1);
	}
	if (it == 0) {
	    F2C(dspt)(&m, &n, stk(l), &nel, istk(ilr), istk(ia 
		 ), stk(lat), istk(iat), istk(irc));
	} else {
	    F2C(wspt)(&m, &n, stk(l), stk(l + nel), &
		    nel, istk(ilr), istk(ia), stk(lat),
		     stk(lat + nel), istk(iat), istk(irc));
	}
	*istk(il ) = 7;
	I1 = ilr + n + 1 + nel;
	lr = I1 / 2 + 1;
	I1 = n + 1;
	/*    FD  modif  Jc -1 & Ir -1
      	F2C(icopy)(&I1, istk(iat ), &c1, istk(ilr ), &c1);
	F2C(icopy)(&nel, istk(irc + n ), &c1, istk(ilr + n+1), &c1);   */
	for (kkk=0; kkk<I1; ++kkk) *istk(ilr+kkk)=*istk(iat+kkk)-1;
	for (kkk=0; kkk<nel; ++kkk) *istk(ilr+n+1+kkk)=*istk(irc+n+kkk)-1;
	I1 = nel * (it + 1);
	F2C(unsfdcopy)(&I1, stk(lat ), &c1, stk(lr ), &
		c1);
	C2F(vstk).Lstk[Top] = lr + nel * (it + 1);
    } else if (*istk(il ) == 7) {
    } else {
	F2C(error)(&c44);
	return 0;
    }
}


int C2F(intmspget)(id)
integer *id;
{
    integer I1, I2, I3;

    /* Local variables */
    extern  int F2C(dset)();
    static integer ilrs;
    static integer ityp, j, l, m, n;
    extern  int F2C(unsfdcopy)(), F2C(icopy)(), F2C(error)();
    static integer j1;
    extern  int F2C(int2db)();
    static integer nc, il, it, lv, lw;
    static double tv;
    static integer ilc, nel, nelmax, ilr, lij, ilv, top0, kkk;

    /* Parameter adjustments */
    --id;

    /* Function Body */
    Rhs = Max(0,Rhs);
    top0 = Top + 1 - Rhs;
    lw = C2F(vstk).Lstk[Top];
    if (Rhs != 1) {
	F2C(error)(&c39);
	return 0;
    }
    if (Lhs > 3) {
	F2C(error)(&c41);
	return 0;
    }
    il = C2F(vstk).Lstk[Top -1] + C2F(vstk).Lstk[Top -1] - 1;
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
	C2F(vstk).Lstk[Top] = I1 / 2 + 1;
	if (Lhs >= 2) {
	    ++Top;
	    il = C2F(vstk).Lstk[Top -1] + C2F(vstk).Lstk[Top -1] - 
		    1;
	    *istk(il ) = 1;
	    *istk(il+1) = 0;
	    *istk(il + 2) = 0;
	    *istk(il + 3) = 0;
	    I1 = il + 4;
	    C2F(vstk).Lstk[Top] = I1 / 2 + 1;
	}
	if (Lhs == 3) {
	    ++Top;
	    il = C2F(vstk).Lstk[Top -1] + C2F(vstk).Lstk[Top -1] - 1;
	    *istk(il ) = 1;
	    *istk(il+1) = 1;
	    *istk(il + 2) = 2;
	    *istk(il + 3) = 0;
	    I1 = il + 4;
	    l = I1 / 2 + 1;
	    *stk(l ) = (double) m;
	    *stk(l+1) = (double) n;
	    C2F(vstk).Lstk[Top] = l + 2;
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
    Err = lw - C2F(vstk).Lstk[Bot -1];
    if (Err > 0) {
	F2C(error)(&c17);
	return 0;
    }
    I1 = n + nel + 1;
    /* FD 1ere colonne de ij = indices en C + 1 */
    for (kkk=0; kkk<I1; ++kkk) *istk(ilrs+kkk)=*istk(ilr+kkk)+1;
    /*    F2C(icopy)(&I1, istk(ilr ), &c1, istk(ilrs ), &c1); */

    /*                    V             */
    if (l >= lv) {
	I1 = nel * (it + 1);
	/*	printf("vvvvvvvvvvvvvvvvvv\n");
	printf("%f\n",stk(l));
	printf("%f\n",stk(l+1));
	printf("%f\n",stk(l+2));
	printf("%f\n",stk(l+3));
	printf("vvvvvvvvvvvvvvvvvv\n"); */
	F2C(unsfdcopy)(&I1, stk(l ), &c1, stk(lv ), &c1);
    } else {
	I1 = nel * (it + 1);
	/* printf("wwwwwwwwwwwwwww\n");
	printf("%f\n",*stk(l));
	printf("%f\n",*stk(l+1));
	printf("%f\n",*stk(l+2));
	printf("%f\n",*stk(l+3));
	printf("wwwwwwwwwwwwwwwwwwww\n"); */
	F2C(unsfdcopy)(&I1, stk(l ), &c_n1, stk(lv ), &c_n1);
    }

    F2C(int2db)(&nel, istk(ilrs + n+1), &c1, stk(lij), &c1);
    for (j = 1; j <= n; ++j) {
	nc = *istk(ilrs + j ) - *istk(ilrs + j - 1);
	j1 = *istk(ilrs + j - 1) -1;
	tv = (double) j;
	F2C(dset)(&nc, &tv, stk(lij + nel + j1 ), &c1);
    }

    /*            ij               */
    *istk(il ) = 1;
    *istk(il+1) = nel;
    *istk(il + 2) = 2;
    *istk(il + 3) = 0;
    C2F(vstk).Lstk[Top] = lij + (nel << 1);
    if (Lhs >= 2) {
      /*           V              */
	++Top;
	il = C2F(vstk).Lstk[Top -1] + C2F(vstk).Lstk[Top -1] - 1;
	*istk(il ) = 1;
	*istk(il+1) = nel;
	*istk(il + 2) = 1;
	*istk(il + 3) = it;
	C2F(vstk).Lstk[Top] = lv + nel * (it + 1);
    }
    if (Lhs == 3) {
      /*            mn             */
	++Top;
	il = C2F(vstk).Lstk[Top -1] + C2F(vstk).Lstk[Top -1] - 1;
	*istk(il ) = 1;
	*istk(il+1) = 1;
	*istk(il + 2) = 2;
	*istk(il + 3) = 0;
	I1 = il + 4;
	l = I1 / 2 + 1;
	*stk(l ) = (double) m;
	*stk(l+1) = (double) n;
	C2F(vstk).Lstk[Top] = l + 2;
    }
    return 0;
} 


int C2F(intmfull)(id)
     integer *id;
{
    integer I1, I2, I3;

    /* Local variables */
    static integer l, m, n;
    extern  int F2C(unsfdcopy)(), F2C(icopy)(), F2C(error)();
    static integer il, it, ls, lw, ilc, nel, ilr, ils, kkk;
    extern  int dmspful(), wmspful();
    static integer top0;

    /* Parameter adjustments */
    --id;

    /* Function Body */
    Rhs = Max(0,Rhs);
    top0 = Top + 1 - Rhs;
    lw = C2F(vstk).Lstk[Top];
    if (Rhs != 1) {
	F2C(error)(&c39);
	return 0;
    }
    if (Lhs != 1) {
	F2C(error)(&c41);
	return 0;
    }
    il = C2F(vstk).Lstk[Top -1] + C2F(vstk).Lstk[Top -1] - 1;
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
    Err = lw - C2F(vstk).Lstk[Bot-1];
    if (Err > 0) {
	F2C(error)(&c17);
	return 0;
    }
    I1 = n + 1 + nel;
    /*     FD modif  */
    for (kkk=0; kkk<I1; ++kkk) *istk(ils+kkk)=*istk(ilr+kkk)+1;
    /* F2C(icopy)(&I1, istk(ilr ), &c1, istk(ils ), &c1); */
    I1 = nel * (it + 1);
    F2C(unsfdcopy)(&I1, stk(l ), &c1, stk(ls ), &c1);
    *istk(il ) = 1;
    I1 = il + 4;
    l = I1 / 2 + 1;
    if (it == 0) {
	dmspful(&m, &n, stk(ls ), &nel, istk(ils ), stk(l ));
    } else {
      wmspful(&m, &n, stk(ls ), stk(ls + nel ), &nel,istk(ils ), 
	      stk(l ), stk(l + m *n )); 
    }
    C2F(vstk).Lstk[Top] = l + m * n * (it + 1);
    return 0;
} /* intmfull_ */


 int dmspful(ma, na, a, nela, inda, r__)
integer *ma, *na;
double *a;
integer *nela, *inda;
double *r__;
{
    /* System generated locals */
    integer I1, I2;

    /* Local variables */
    extern  int F2C(dset)();
    static integer I, j, k, ii, nj;

    /* Parameter adjustments */
    --r__;
    --inda;
    --a;

    /* Function Body */
    I1 = *ma * *na;
    F2C(dset)(&I1, &c_b46, &r__[1], &c1);
    k = 0;
    I1 = *na;
    for (j = 1; j <= I1; ++j) {
	nj = inda[j + 1] - inda[j];
	if (nj > 0) {
	    I2 = nj;
	    for (ii = 1; ii <= I2; ++ii) {
		I = inda[*na + 1 + k + ii];
		r__[I + (j - 1) * *ma] = a[k + ii];
/* L5: */
	    }
	    k += nj;
	}
/* L10: */
    }
} /* dmspful_ */

int wmspful(ma, na, ar, ai, nela, inda, rr, ri)
     integer *ma, *na;
     double *ar, *ai;
     integer *nela, *inda;
     double *rr, *ri;
{
    integer I1, I2;

    /* Local variables */
    extern   int F2C(dset)();
    static integer I, j, k, ii, nj;

    /* Parameter adjustments */
    --ri;
    --rr;
    --inda;
    --ai;
    --ar;

    /* Function Body */
    I1 = *ma * *na;
    F2C(dset)(&I1, &c_b46, &rr[1], &c1);
    I1 = *ma * *na;
    F2C(dset)(&I1, &c_b46, &ri[1], &c1);
    k = 0;
    I1 = *na;
    for (j = 1; j <= I1; ++j) {
	nj = inda[j + 1] - inda[j];
	if (nj > 0) {
	    I2 = nj;
	    for (ii = 1; ii <= I2; ++ii) {
		I = inda[*na + 1 + k + ii];
		rr[I + (j - 1) * *ma] = ar[k + ii];
		ri[I + (j - 1) * *ma] = ai[k + ii];
		/* L5: */
	    }
	    k += nj;
	}
	/* L10: */
    }
}


