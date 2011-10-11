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
  |   6) Interface code for getting the LU factors              |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [L,U,p,q,R] = umf_luget(LU_ptr)                           |
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

extern CellAdr *ListNumeric;

int sci_umf_luget(char* fname, unsigned long l)
{
	/* 
	 *  LU_ptr is (a pointer to) a factorisation of A, we have:
	 *             -1
	 *          P R  A Q = L U
	 *
	 *      A is n_row x n_col
	 *      L is n_row x n
	 *      U is n     x n_col     n = min(n_row, n_col)
	 */

	int mLU_ptr = 0, nLU_ptr = 0, lLU_ptr = 0;
	void * Numeric = NULL;
	int lnz = 0, unz = 0, n_row = 0, n_col = 0, n = 0, nz_udiag = 0, i = 0, stat = 0, do_recip = 0, it_flag = 0;
	int *L_mnel = NULL, *L_icol = NULL, *L_ptrow = NULL, *U_mnel = NULL, *U_icol = NULL, *U_ptrow = NULL, *V_irow = NULL, *V_ptcol = NULL;
	double *L_R = NULL, *L_I = NULL, *U_R = NULL, *U_I = NULL, *V_R = NULL, *V_I = NULL, *Rs = NULL;
	int *p = NULL, *q = NULL, one=1, pl_miss = 0, error_flag = 0 ;

	SciSparse L, U;

	/* Check numbers of input/output arguments */
	CheckRhs(1,1); CheckLhs(1,5);

	/* get the pointer to the LU factors */
	GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mLU_ptr, &nLU_ptr, &lLU_ptr);
	Numeric = (void *) ((unsigned long int) *stk(lLU_ptr));

	/* Check if the pointer is a valid ref to ... */
	if ( IsAdrInList(Numeric, ListNumeric, &it_flag) )
	{
		if (it_flag == 0 )
			umfpack_di_get_lunz(&lnz, &unz, &n_row, &n_col, &nz_udiag, Numeric);
		else
			umfpack_zi_get_lunz(&lnz, &unz, &n_row, &n_col, &nz_udiag, Numeric);
	}
	else
	{
		Scierror(999,_("%s: Wrong value for input argument #%d: Must be a valid reference to (umf) LU factors.\n"),fname,1);
		return 0;
	}

	if (n_row <= n_col) n = n_row; else n = n_col;
	L_mnel  = (int*)MALLOC( n_row   * sizeof(int));
	L_icol  = (int*)MALLOC( lnz     * sizeof(int));
	L_ptrow = (int*)MALLOC((n_row+1)* sizeof(int));
	L_R     = (double*)MALLOC( lnz     * sizeof(double));
	U_mnel  = (int*)MALLOC( n       * sizeof(int));
	U_icol  = (int*)MALLOC( unz     * sizeof(int));
	U_ptrow = (int*)MALLOC((n+1)    * sizeof(int));
	U_R     = (double*)MALLOC( unz     * sizeof(double));
	V_irow  = (int*)MALLOC( unz     * sizeof(int));
	V_ptcol = (int*)MALLOC((n_col+1)* sizeof(int));
	V_R     = (double*)MALLOC( unz     * sizeof(double));
	p       = (int*)MALLOC( n_row   * sizeof(int));
	q       = (int*)MALLOC( n_col   * sizeof(int));
	Rs      = (double*)MALLOC( n_row   * sizeof(double)); 
	if ( it_flag == 1 )
	{
		L_I = (double*)MALLOC( lnz     * sizeof(double));
		U_I = (double*)MALLOC( unz     * sizeof(double));
		V_I = (double*)MALLOC( unz     * sizeof(double));
	}
	else
	{ L_I = U_I = V_I = NULL; }

	if (    !(L_mnel && L_icol && L_R && L_ptrow  && p &&
			  U_mnel && U_icol && U_R && U_ptrow  && q &&
			  V_irow && V_R && V_ptcol  && Rs)
			|| (it_flag && !(L_I && U_I && V_I))   )
	{
		error_flag = 1;
		goto the_end;
	}

	if ( it_flag == 0 )
	{
		stat = umfpack_di_get_numeric(L_ptrow, L_icol, L_R, V_ptcol, V_irow, V_R, 
									  p, q, (double *)NULL, &do_recip, Rs, Numeric);
	}
	else
	{
		stat = umfpack_zi_get_numeric(L_ptrow, L_icol, L_R, L_I, V_ptcol, V_irow, V_R, V_I, 
									  p, q, (double *)NULL, (double *)NULL, &do_recip, Rs, Numeric);
	}

	if ( stat != UMFPACK_OK ) { error_flag = 2; goto the_end; };

	if ( do_recip )
	{
		for ( i = 0 ; i < n_row ; i++ ) Rs[i] = 1.0 / Rs[i];
	}

	if ( it_flag == 0 )
	{
		stat = umfpack_di_transpose(n, n_col, V_ptcol, V_irow, V_R, (int *) NULL, 
									(int*) NULL, U_ptrow, U_icol, U_R);  
	}
	else
	{
		stat = umfpack_zi_transpose(n, n_col, V_ptcol, V_irow, V_R, V_I, (int *) NULL, 
									(int*) NULL, U_ptrow, U_icol, U_R, U_I, 0);  
	}

	if ( stat != UMFPACK_OK ) { error_flag = 2; goto the_end; };

	for ( i = 0 ; i < n_row ; i++ ) 
	{
		L_mnel[i] = L_ptrow[i+1] - L_ptrow[i];
	}
	for ( i = 0 ; i < n ; i++ ) 
	{
		U_mnel[i] = U_ptrow[i+1] - U_ptrow[i];
	}

	for ( i = 0 ; i < lnz ; i++ ) L_icol[i]++;
	for ( i = 0 ; i < unz ; i++ ) U_icol[i]++;
      
	for ( i = 0 ; i < n_row ; i++ ) p[i]++; 
	for ( i = 0 ; i < n_col ; i++ ) q[i]++; 
  
	L.m = n_row; L.n = n; L.it = it_flag; L.nel = lnz; L.mnel = L_mnel; L.icol = L_icol; L.R = L_R; L.I = L_I; 
	U.m = n; U.n = n_col; U.it = it_flag; U.nel = unz; U.mnel = U_mnel; U.icol = U_icol; U.R = U_R; U.I = U_I; 
     
	if (! test_size_for_sparse(2 , L.m, L.it, L.nel, &pl_miss)) { error_flag = 3; goto the_end; }; 
	CreateVarFromPtr(2,SPARSE_MATRIX_DATATYPE,&n_row ,&n  , &L);
	if (! test_size_for_sparse(3 , U.m, U.it, U.nel, &pl_miss)) { error_flag = 3; goto the_end; };
	CreateVarFromPtr(3,SPARSE_MATRIX_DATATYPE,&n ,&n_col  , &U);
	if (! test_size_for_mat(4 , n_row, 1, 0, &pl_miss)) { error_flag = 3; goto the_end; };
	CreateVarFromPtr(4,MATRIX_OF_INTEGER_DATATYPE,&n_row ,&one, &p);
	if (! test_size_for_mat(5 , n_col, 1, 0, &pl_miss)) { error_flag = 3; goto the_end; };
	CreateVarFromPtr(5,MATRIX_OF_INTEGER_DATATYPE,&n_col ,&one, &q);
	if (! test_size_for_mat(6 , n_row, 1, 0, &pl_miss)) { error_flag = 3; goto the_end; };
	CreateVarFromPtr(6,MATRIX_OF_DOUBLE_DATATYPE,&n_row ,&one, &Rs);

 the_end:
	FREE(L_mnel); FREE(L_icol); FREE(L_R); FREE(L_ptrow); FREE(p);
	FREE(U_mnel); FREE(U_icol); FREE(U_R); FREE(U_ptrow); FREE(q);
	FREE(V_irow); FREE(V_R); FREE(V_ptcol); FREE(Rs);
	if ( it_flag == 1 )
		{ FREE(L_I); FREE(V_I); FREE(U_I); }

	switch (error_flag)
	{
		case 0:   /* no error */
			LhsVar(1) = 2;
			LhsVar(2) = 3;
			LhsVar(3) = 4;
			LhsVar(4) = 5;
			LhsVar(5) = 6;
		break;

		case 1:   /* enough memory (with malloc) */
			Scierror(999,_("%s: No more memory.\n"),fname);
		break;

		case 2:   /* a problem with one umfpack routine */
			Scierror(999,"%s: %s\n",fname,UmfErrorMes(stat));
		break;

		case 3:  /* not enough place in the scilab stack */
			Scierror(999,_("%s: No more memory : increase stacksize %d supplementary words needed.\n"),
				fname,
				pl_miss);
		break;
	}
	PutLhsVar();
	return 0;
}


