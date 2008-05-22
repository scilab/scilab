/*
 *   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project 
 *   <bruno.pincon@iecn.u-nancy.fr>
 *   contributor:  Antonio Manoel Ferreria Frasson, Universidade Federal do 
 *                 Espírito Santo, Brazil. <frasson@ele.ufes.br>.
 *
 * PURPOSE: Scilab interfaces routines onto the UMFPACK sparse solver
 * (Tim Davis) and onto the TAUCS snmf choleski solver (Sivan Teledo)
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 */


/*------------------------------------------------------------+
  |   8) Interface code for solving Ax=b  when the Cholesky     |
  |      factors of A are known                                 |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [x] = taucs_chsolve(C_ptr,b [,A])                         |
  |                                                             |
  |      mb : number of rows of the vector b                    |
  |      nb : number of columns of the vector b (>= 1)          |
  |      lb : index of the first element of b in the stack      |
  |       b : pointer to the vector b (gotten with b = stk(lb)) |
  |       A : (optional) if provided a refiment step if taken   |
  |                                                             |
  +------------------------------------------------------------*/
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"

extern CellAdr *ListCholFactors;


int sci_taucs_chsolve(char* fname, unsigned long l)
{
	int mb, nb, lb, lx, one=1, lv, lres;
	int mC_ptr, nC_ptr, lC_ptr;
	int mA, nA, i, j, n, it_flag, Refinement;
	double *b, *x, *v, *res, norm_res, norm_res_bis;
	long double *wk = NULL;
	int A_is_upper_triangular;
	taucs_handle_factors * pC;
	SciSparse A;

	/* Check numbers of input/output arguments */
	CheckRhs(2,3); CheckLhs(1,1);

	/* First get arg #1 : the pointer to the Cholesky factors */
	GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mC_ptr, &nC_ptr, &lC_ptr);
	pC = (taucs_handle_factors *) ((unsigned long int) *stk(lC_ptr));

	/* Check if this pointer is a valid ref to a Cholesky factor object */
	if ( ! IsAdrInList( (Adr)pC, ListCholFactors, &it_flag) )
	{
		Scierror(999,_("%s: Wrong value for input argument #%d: not a valid reference to Cholesky factors"),fname,1);
		return 0;
	};

	/*  the number of rows/lines of the matrix  */
	n = pC->n;
	/* Get now arg #2 : the vector b */
	GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE ,&mb,&nb,&lb);

	/* test if the right hand side is compatible */
	if (mb != n || nb < 1)  
	{
		Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,2);
		return 0;
	};

	/* get the pointer for b */
	b = stk(lb);

	if ( Rhs == 3 )
	{
		GetRhsVar(3,  SPARSE_MATRIX_DATATYPE, &mA, &nA, &A);
		if ( mA != nA  ||  mA != n  ||  A.it == 1 )
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: not compatible with the Choleski factorisation.\n"),fname,3);
			return 0;
		};
		Refinement = 1;
		A_is_upper_triangular = is_sparse_upper_triangular(&A);
	}
	else
	{
		Refinement = 0;
	}
	
	/* allocate memory for the solution x */
	CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &mb, &nb, &lx);
	x = stk(lx);

	/* allocate memory for a temporary vector v */
	CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &mb, &one, &lv); v = stk(lv);

	if ( Refinement )
	{
		CreateVar(Rhs+3, MATRIX_OF_DOUBLE_DATATYPE, &mb, &one, &lres); res = stk(lres);
		if ( A_is_upper_triangular )
			if ( (wk = MALLOC( n * sizeof(long double))) == NULL )
			{
				Scierror(999,_("%s: not enough memory.\n"),fname);
				return 0;
			};
	}

	for ( j = 0; j < nb ; j++ )
		{
			taucs_vec_permute(n, &b[j*mb], &x[j*mb], pC->p);
			taucs_supernodal_solve_llt(pC->C, v, &x[j*mb]);  /* FIXME : add a test here */
			taucs_vec_ipermute(n, v, &x[j*mb], pC->p);
			if ( Refinement )
				{
					/* do one iterative refinement */
					residu_with_prec_for_chol(&A, &x[j*mb], &b[j*mb], res, &norm_res, A_is_upper_triangular, wk);
					/*  FIXME: do a test if the norm_res has an anormal value and send a warning
					 *         (the user has certainly not give the good matrix A 
					 */
					taucs_vec_permute(n, res, v, pC->p);
					taucs_supernodal_solve_llt(pC->C, res, v);  /* FIXME : add a test here */
					taucs_vec_ipermute(n, res, v, pC->p);
					for ( i = 0 ; i < n ; i++ )
						v[i] = x[j*mb+i] - v[i];    /* v is the refined solution */
					residu_with_prec_for_chol(&A, v, &b[j*mb], res, &norm_res_bis, A_is_upper_triangular, wk);
					/* accept it if the 2 norm of the residual is improved */
					if ( norm_res_bis < norm_res )
						for ( i = 0 ; i < n ; i++ )
							x[j*mb+i] = v[i];
				}
		}
  
	FREE(wk);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
