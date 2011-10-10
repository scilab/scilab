/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_optimization.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
/* fortran subroutines */
extern double C2F(dlamch)  (char *CMACH, unsigned long int);
extern int C2F(ql0001)(int *m,int *me,int *mmax,int *n,int *nmax,int *mnn,
            double *c,double *d,double *a,double *b,double *xl,
            double *xu,double *x,double *u,int *iout,int *ifail,
            int *iprint,double *war,int *lwar,int *iwar,int *liwar,
            double *eps1);
/*--------------------------------------------------------------------------*/
int sci_qld(char *fname,unsigned long fname_len)
{
	static int un = 1,zero = 0;
	static int Q = 0, n = 0, nbis = 0;
	static int p = 0, unbis = 0;
	static int C = 0, mmax = 0, m = 0, mnn = 0;
	static int b = 0, mbis = 0;
	static int me = 0, pipo = 0;
	static int lb = 0, ub = 0, leps = 0;
	static int x = 0, lambda = 0, inform = 0, ifail = 0;
	int next = 0;

	static int war = 0, lwar = 0, iwar = 0, iout = 0, k = 0, l = 0;
	static double eps1 = 0;

	/*   Check rhs and lhs   */
	CheckRhs(7,8) ;
	CheckLhs(1,3) ;

	/* RhsVar: qld(Q,p,C,b,lb,ub,me,eps) */
	/*             1,2,3,4,5 ,6 ,7, 8  */
	eps1 = C2F(dlamch)("e",1L);
	next = Rhs+1;
	/*   Variable 1 (Q)   */
	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &n, &nbis, &Q);
	CheckSquare(1,n,nbis);

	/*   Variable 2 (p)   */
	GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &unbis, &p);
	CheckLength(2,nbis*unbis,n);

	/*   Variable 3 (C)   */
	GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m, &nbis, &C);
	if (( nbis != n ) && (m > 0))
	{
		Scierror(205,_("%s: Wrong size for input argument #%d: number of columns %d expected.\n"),fname,3,n);
		return 0;
	}
	mmax = m;
	mnn = m + n + n;

	/*   Variable 4 (b)   */
	GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &mbis, &unbis, &b);
	CheckLength(4,mbis*unbis,m);

	/*   Variable 5 (lb)   */
	GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &unbis, &lb);
	if (nbis*unbis==0) 
	{
		CreateVar(next,MATRIX_OF_DOUBLE_DATATYPE, &n, &un, &lb);
		for(k=0; k<n; k++) stk(lb)[k] = -C2F(dlamch)("o",1L);
		next = next+1;
	}
	else CheckLength(5,nbis*unbis,n);

	/*   Variable 6 (ub)   */
	GetRhsVar(6,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &unbis, &ub);
	if (nbis*unbis==0) 
	{
		CreateVar(next,MATRIX_OF_DOUBLE_DATATYPE, &n, &un, &ub);
		for(k=0; k<n; k++) stk(ub)[k] = C2F(dlamch)("o",1L);
		next = next+1;
	}
	else CheckLength(6,nbis*unbis,n);

	/*   Variable 7 (me)   */
	GetRhsVar(7,MATRIX_OF_INTEGER_DATATYPE, &pipo, &unbis, &me);
	CheckScalar(7,pipo,unbis);
	if ((*istk(me)<0) || (*istk(me)>n))
	{
		Err = 7;
		SciError(116);
		return 0;
	}

	if(Rhs == 8)
	{
		/*   Variable 8 (eps1)   */
		GetRhsVar(8,MATRIX_OF_DOUBLE_DATATYPE, &pipo, &unbis, &leps);
		CheckScalar(8,pipo,unbis);
		eps1 = Max(eps1,*stk(leps));
	}

	/* Internal variables: x, lambda, inform, C_mmax, b_mmax */
	CreateVar(next,MATRIX_OF_DOUBLE_DATATYPE, &n, &un, &x);

	CreateVar(next+1,MATRIX_OF_DOUBLE_DATATYPE, &mnn, &un, &lambda);

	CreateVar(next+2,MATRIX_OF_INTEGER_DATATYPE, &un, &un, &inform);

	lwar = 3*n*n/2+10*n+2*mmax+2;
	CreateVar(next+3,MATRIX_OF_DOUBLE_DATATYPE, &lwar, &un, &war);
	CreateVar(next+4,MATRIX_OF_INTEGER_DATATYPE, &n, &un, &iwar);
	istk(iwar)[0]=1;/*Cholesky factorisation required*/

	/* Change the sign of C*/
	for(k=0; k<n; k++) 
	{
		for(l=0; l<m; l++) stk(C)[k*m+l] = -stk(C)[k*m+l];
    }

	iout = 0;
	ifail=0;
	C2F(ql0001)(&m, istk(me), &mmax, &n, &n, &mnn, stk(Q), stk(p), stk(C),
		stk(b), stk(lb), stk(ub), stk(x), stk(lambda), &iout,
		&ifail, &zero, stk(war), &lwar, istk(iwar), &n, &eps1);


	/* LhsVar: [x, lambda, inform] = qld(...) */
	if (ifail == 0) 
	{
		LhsVar(1) = next;
		LhsVar(2) = next+1;
		if (Lhs == 3) 
		{
			LhsVar(3) = next+2;
			*istk(inform) = ifail;
		}
		PutLhsVar();
	}
	else if (ifail == 1)
		Scierror(24,_("%s: Too many iterations (more than %d).\n"),fname,40*(n+m));
	else if (ifail == 2)
		Scierror(24,_("%s: Accuracy insufficient to satisfy convergence criterion.\n"),fname);
	else if (ifail == 5)
		Scierror(999,_("%s: Length of working array is too short.\n"),fname);
	else if (ifail>10)
		Scierror(999,_("%s: The constraints are inconsistent.\n"),fname);
	else 
	{
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
