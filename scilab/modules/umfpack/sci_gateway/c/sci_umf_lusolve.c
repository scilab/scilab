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
  |   3) Interface code for solving Ax=b or A'x=b when          |
  |      the LU factors of A are known                          |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [x] = umf_lusolve(LU_ptr,b [,flag, A])                    |
  |         flag = "Ax=b" or "A'x=b" ("Ax=b" default)           |
  |         if A is given => raffinement is done                |
  |                                                             |
  |      mb : number of rows of the vector b                    |
  |      nb : number of columns of the vector b (>= 1)          |
  |      lb : index of the first element of b in the stack      |
  |       b : pointer to the vector b (gotten with b = stk(lb)) |
  |                                                             |
  |                                                             |
  +------------------------------------------------------------*/
#include "gw_umfpack.h"
#include "sciumfpack.h"
#include "stack-c.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "localization.h"

extern CellAdr *ListNumeric;

int sci_umf_lusolve(char* fname,unsigned long l)
{
	int mb, nb, lrb, lib, itb, mflag, nflag, lflag, lrx, lix, itx;
	int mLU_ptr, nLU_ptr, lLU_ptr, it_flag, i;
	int mA, nA, j, one=1, LastNum;
	SciSparse AA;
	CcsSparse A;
	int NoTranspose, NoRaffinement;
	double *br, *bi, *xr, *xi;

	/* umfpack stuff */
	double Info[UMFPACK_INFO]; // double *Info = (double *) NULL;
	double Control[UMFPACK_CONTROL];
	void * Numeric;
	int lnz, unz, n, n_col, nz_udiag, umf_flag;
	int *Wi, lWi, lW, mW;
	double *W;
  
	/* Check numbers of input/output arguments */
	CheckRhs(2,4); CheckLhs(1,1);

	/* First get arg #1 : the pointer to the LU factors */
	GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mLU_ptr, &nLU_ptr, &lLU_ptr);
	Numeric = (void *) ((unsigned long int) *stk(lLU_ptr));

	/* Check if this pointer is a valid ref to a umfpack LU numeric object */
	if ( ! IsAdrInList(Numeric, ListNumeric, &it_flag) )
	{
		Scierror(999,_("%s: Wrong value for input argument #%d: Must be a valid reference to (umf) LU factors.\n"),fname,1);
		return 0;
	};

	/*  get some parameters of the factorisation (for some checking) */
	if ( it_flag == 0 )
		umfpack_di_get_lunz(&lnz, &unz, &n, &n_col, &nz_udiag, Numeric);
	else
		umfpack_zi_get_lunz(&lnz, &unz, &n, &n_col, &nz_udiag, Numeric);

	if ( n != n_col ) 
	{
		Scierror(999,_("%s: An error occurred: %s.\n"),fname,_("This is not a factorisation of a square matrix"));
		return 0;
	};
	if ( nz_udiag < n ) 
	{
		Scierror(999,_("%s: An error occurred: %s.\n"),fname,_("This is a factorisation of a singular matrix"));
		return 0;
	};

	/* Get now arg #2 : the vector b */
	GetRhsCVar(2,MATRIX_OF_DOUBLE_DATATYPE, &itb, &mb, &nb, &lrb, &lib);
	if (mb != n || nb < 1)    /* test if the right hand side is compatible */
	{
		Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,2);
		return 0;
	};

	/* allocate memory for the solution x */
	if ( it_flag == 1  ||  itb == 1 ) itx = 1; else itx = 0;     
	CreateCVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &itx, &mb, &nb, &lrx, &lix);
	xr = stk(lrx); xi = stk(lix);

	/*  selection between the different options :
	 *   -- solving Ax=b or A'x=b (Note: we could add  A.'x=b)
	 *   -- with or without raffinement
	 */

	if ( Rhs == 2 )
	{
		NoTranspose = 1;
		NoRaffinement = 1;
	}
	else  /* 3 or 4 input arguments but the third must be a string */
	{
		GetRhsVar(3,STRING_DATATYPE,&mflag,&nflag,&lflag);
		if ( strcmp(cstk(lflag),"Ax=b") == 0 )
			NoTranspose = 1;
		else if ( strcmp(cstk(lflag),"A'x=b") == 0 )
			NoTranspose = 0;
		else
		{
			Scierror(999,_("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"),fname,3,"Ax=b","A'x=b");
			return 0;
		};
		if ( Rhs == 4 ) 
		{
			GetRhsVar(4, SPARSE_MATRIX_DATATYPE, &mA, &nA, &AA);
			/*  some check... but we can't be sure that the matrix corresponds to the LU factors */
			if ( mA != nA || mA != n || AA.it != it_flag )  
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: %s.\n"),fname,4,_("Matrix is not compatible with the given LU factors"));
				return 0;
			};
			NoRaffinement = 0;
		}
		else NoRaffinement = 1;   /* only 3 input var => no raffinement */
	}

	/* allocate memory for umfpack_di_wsolve usage or umfpack_zi_wsolve usage*/
	CreateVar(Rhs+2,  MATRIX_OF_INTEGER_DATATYPE, &n, &one, &lWi); Wi = istk(lWi);
	if (it_flag == 1) 
	{
		if (NoRaffinement) mW = 4*n; 
		else mW = 10*n;
	}
	else
	{
		if (NoRaffinement) mW = n; 
		else mW = 5*n;
	}
	CreateVar(Rhs+3, MATRIX_OF_DOUBLE_DATATYPE, &mW, &one, &lW); W  = stk(lW);

	if (! NoRaffinement)
	{
		SciSparseToCcsSparse(Rhs+4, &AA, &A);
		LastNum = Rhs+4;
	}
	else
	{
		A.p = NULL; A.irow = NULL; A.R = NULL; A.I = NULL;
		LastNum = Rhs+3;
	}

	/* get the pointer for b */
	br = stk(lrb); bi = stk(lib);
	if ( it_flag == 1  &&  itb == 0 )
	{
		CreateVar(LastNum+1,MATRIX_OF_DOUBLE_DATATYPE, &mb, &nb, &lib);
		bi = stk(lib);
		for ( i = 0 ; i < mb*nb ; i++ ) bi[i] = 0.0;
	}

	/* init Control */
	if (it_flag == 0) 
		umfpack_di_defaults(Control);
	else
		umfpack_zi_defaults(Control);
	if (NoRaffinement)
		Control[UMFPACK_IRSTEP] = 0;

	if (NoTranspose) 
		umf_flag = UMFPACK_A;
	else
		umf_flag = UMFPACK_At;

	if (it_flag == 0)
	{
		for ( j = 0; j < nb ; j++ ) 
		{
			umfpack_di_wsolve(umf_flag, A.p, A.irow, A.R, &xr[j*mb], &br[j*mb], Numeric, Control, Info, Wi, W);
		}
		if (itx == 1)
		{
			for ( j = 0; j < nb ; j++ ) 
			{
				umfpack_di_wsolve(umf_flag, A.p, A.irow, A.R, &xi[j*mb], &bi[j*mb], Numeric, Control, Info, Wi, W);
			}
		}
	}
    else
	{
		for ( j = 0; j < nb ; j++ ) 
		{
			umfpack_zi_wsolve(umf_flag, A.p, A.irow, A.R, A.I, &xr[j*mb], &xi[j*mb], &br[j*mb], &bi[j*mb], Numeric, Control, Info, Wi, W);
		}
	}

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
