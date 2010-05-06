/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Serge Steer
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
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
/* fortran subroutines */
extern int C2F(qpgen2)(double *dmat, double *dvec, int *fddmat, int *n, 
					   double *sol, double *crval, double *amat, double *bvec, 
					   int *fdamat, int *q, int *meq, int *iact, int *nact, 
					   int *iter, double *work, int *ierr);  
extern int C2F(qpgen1sci)(double *dmat, double *dvec, int *fddmat, int *n, double *sol, double *crval, 
						  int *colnnz, int *nzrindex, double *amat,
						  double *bvec, int *q, int *meq, int *iact, int *nact, 
						  int *iter, double *work, int *ierr);  
extern int C2F(spt)(int *m, int *n, int *nel, int *it, int *ptr, 
					double *A_R,  double *A_I, int *mnel,  int *A_icol, 
					double *At_R, double *At_I,int *At_mnel, int *At_icol);
/*--------------------------------------------------------------------------*/ 
int sci_qp_solve(char *fname,unsigned long fname_len)
{
	static int un = 1,deux = 2;
        // n : first dimension of Q
        // nbis : second dimension of Q (nbis is expected to be equal to n)
	static int Q = 0, n = 0, nbis = 0;
	static int p = 0, unbis = 0;
	static int C = 0, m = 0;
	static int b = 0, mbis = 0;
	static int me = 0, pipo = 0;
	static int x = 0, iter = 0, iact = 0, nact = 0,crval = 0,ierr = 0;
	int next = 0,r = 0;
	static int lw = 0,  k = 0;
	static SciSparse Sp;
	static int issparse = 0;
	double *work = NULL;

	/*   Check rhs and lhs   */
	CheckRhs(5,5) ;
	CheckLhs(1,4) ;

	/*Warning this interface does not support arguments passed by reference */

	/* RhsVar: qp_solve(Q,p,C,b,me) */
	/*                1,2,3,4,5   */
	next = Rhs+1;
	/*   Variable 1 (Q)   */
	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &n, &nbis, &Q);
	CheckSquare(1,n,nbis);

	/*   Variable 2 (p)   */
	GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &unbis, &p);
	CheckLength(2,nbis*unbis,n);

	/*   Variable 3 (C)   */
	issparse =  (GetType(3)==5);
	if (!issparse) 
	{
		GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &m, &C);
	}
	else 
	{
		GetRhsVar(3,SPARSE_MATRIX_DATATYPE, &nbis, &m, &Sp);
	}

	if (( nbis != n ) && (m > 0))
	{
		Scierror(205,_("%s: Argument 3: wrong number of columns %d expected\n"), fname, n);
		return 0;
        }

	/*   Variable 4 (b)   */
	GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &mbis, &unbis, &b);
	CheckLength(4,mbis*unbis,m);

	/*   Variable 5 (me)   */
	GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE, &pipo, &unbis, &me);
	CheckScalar(5,pipo,unbis);
	if ((*istk(me)<0) || (*istk(me)>n))
	{
		Err = 7;
		SciError(116);
		return 0;
	}

	/* Lhs variables: x, iact, iter, crval */
	next= Rhs;
	CreateVar(next+1,MATRIX_OF_DOUBLE_DATATYPE, &n, &un, &x);
	CreateVar(next+2,MATRIX_OF_INTEGER_DATATYPE, &m, &un, &iact);
	CreateVar(next+3,MATRIX_OF_INTEGER_DATATYPE, &deux, &un, &iter);
	CreateVar(next+4,MATRIX_OF_DOUBLE_DATATYPE, &un, &un, &crval);

	r = Min(n,m);
	lw =  2*n+r*(r+5)/2 + 2*m +1;
	if((work=(double *)MALLOC(lw*sizeof(double)))==NULL) 
	{
		Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
	}
	/* change the sign of  C and b.*/
	ierr=0;
	if (!issparse) 
	{ 
		/* linear constraints matrix is stored full */
		C2F(qpgen2)(stk(Q), stk(p), &n, &n,  stk(x), stk(crval), stk(C),
		stk(b), &n, &m, istk(me), istk(iact), &nact,istk(iter), work, 
		&ierr);
	}
	else 
	{ 
		/* linear constraints matrix is a sparse matrix */
		/* Change the linear constraints matrix representation:
		qpgen1sci requires column-compressed sparse matrix internal 
		representation while Scilab sparse matrices are row-compressed */
		double *R = NULL, *I = NULL;
		int *ind = NULL;

		if((R=(double *)MALLOC(Sp.nel*sizeof(double)))==NULL) 
		{
			FREE(work);
			work = NULL;
			Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		}
		if((ind=(int *)MALLOC((m+Sp.nel)*sizeof(int)))==NULL) 
		{
			FREE(work);
			work = NULL;
			FREE(R);
			R = NULL;
			Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		}
   
   // Transpose the sparse matrix A
		C2F(spt)(&n, &m, &(Sp.nel) ,  &(Sp.it), (int *)work, 
				Sp.R,  Sp.I,  Sp.mnel,  Sp.icol, R, I,ind,ind+m);

		C2F(qpgen1sci)(stk(Q), stk(p), &n, &n,  stk(x), stk(crval), 
						ind, ind+m,  R, 
						stk(b),&m, istk(me), istk(iact), &nact,istk(iter),
						work, &ierr);
		FREE(work);
		work = NULL;
		FREE(R);
		R = NULL;
		FREE(ind);
		ind = NULL;
	}
	for (k=nact; k<m; k++) istk(iact)[k]=0;
	/* LhsVar: [x, iact, iter, f] = qp_solve(...) */

	if (ierr == 0) 
	{
		for (k=0;k<Lhs;k++) LhsVar(1+k) = next+1+k;
		C2F(putlhsvar)();
	}
	else if (ierr == 1)
	{
		Scierror(999,_("%s: The minimization problem has no solution.\n"),fname);
	}
	else if (ierr == 2)
	{
		Scierror(999,_("%s: Q is not symmetric positive definite.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/ 
