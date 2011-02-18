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
  | 1) Interface code for the direct solving :                  |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   Case 1 :                                                  |
  |   [x] = umfpack(A,"\",b) -> solve directly Ax=b             |
  |                                                             |
  |   Case 2 :                                                  |
  |   [x] = umfpack(b,"/",A) -> solve directly xA=b <=> A'x'=b' |
  |                                                             |
  |   Var description (to complete...)                          |
  |   ---------------                                           |
  |      mA : number of rows of the matrix A                    |
  |      nA : number of columns of A                            |
  |       A : pointer to a sparse matrix struct                 |
  |                                                             |
  |      Require mA == nA (matrix must be square), mA > 0       |
  |                                                             |
  |      mb : number of rows of the vector b                    |
  |      nb : number of columns of the vector b                 |
  |      lb : index of the first element of b in the stack      |
  |       b : pointer to the vector b (gotten with b = stk(lb)) |
  |                                                             |
  |      Require mb = mA , nb >= 1 in Case 1                    |
  |              mb >= 1 , nb = mA in Case 2                    |
  |                                                             |
  +------------------------------------------------------------*/
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "Scierror.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"

int sci_umfpack(char* fname,unsigned long l)
{
	int mA, nA, mb, nb, lrb, lib, itb, ms, ns, ls, itx, lrx, lix, i;
	int num_A, num_b, one=1;
	int mW, lW, lWi;
	int Case, stat;
	double *br, *bi, *xr, *xi;
	SciSparse AA;
	CcsSparse A;

	/* umfpack stuff */
	double *Control = (double *) NULL; //, *Info = (double *) NULL;
	double Info[UMFPACK_INFO];
	void *Symbolic, *Numeric;
	int *Wi;
	double *W;

  
	/* Check numbers of input/output arguments */
	CheckRhs(3,3); CheckLhs(1,1);

	/* First get arg #2 : a string of length 1 */
	GetRhsVar(2,STRING_DATATYPE, &ms, &ns, &ls);

	/* select Case 1 or 2 depending (of the first char of) the string ... */ 
	if (*cstk(ls) == '\\')
	{
		Case = 1; num_A = 1; num_b = 3;
	}
	else if (*cstk(ls) == '/')
	{
		Case = 2; num_A = 3; num_b = 1;
	}
	else
	{
		Scierror(999,_("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"),fname,2,"\\","/");
		return 0;
	}

	GetRhsVar(num_A,SPARSE_MATRIX_DATATYPE,&mA,&nA,&AA);
	if ( mA != nA || mA < 1 )
	{
		Scierror(999,_("%s: Wrong size for input argument #%d.\n"), fname, num_A);
		return 0;
	};
 
	GetRhsCVar(num_b,MATRIX_OF_DOUBLE_DATATYPE,&itb,&mb,&nb,&lrb,&lib);
	if ( (Case==1 && ( mb != mA || nb < 1 )) || (Case==2 && ( nb != mA || mb < 1 )) )
	{
		Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname, num_b);
		return 0;
	};

	SciSparseToCcsSparse(4, &AA, &A);

	/* allocate memory for the solution x */
	if ( A.it == 1  ||  itb == 1 ) itx = 1; else itx = 0;     
	CreateCVar(5,MATRIX_OF_DOUBLE_DATATYPE, &itx, &mb, &nb, &lrx, &lix);
	xr = stk(lrx); xi = stk(lix);

	/* allocate memory for umfpack_di_wsolve usage or umfpack_zi_wsolve usage*/
	CreateVar(6, MATRIX_OF_INTEGER_DATATYPE, &mA, &one, &lWi); Wi = istk(lWi);
	if (A.it == 1) mW = 10*mA; else mW = 5*mA;
	CreateVar(7, MATRIX_OF_DOUBLE_DATATYPE, &mW, &one, &lW); W  = stk(lW);

	/* get the pointer for b */
	br = stk(lrb); bi = stk(lib);
	if ( A.it == 1  &&  itb == 0 )
	{
		CreateVar(8,MATRIX_OF_DOUBLE_DATATYPE, &mb, &nb, &lib); 
		bi = stk(lib);
		for ( i = 0 ; i < mb*nb ; i++ ) bi[i] = 0.0;
	}

	/* Now calling umfpack routines */
	if (A.it == 1)
	{
		stat = umfpack_zi_symbolic(mA, nA, A.p, A.irow, A.R, A.I, &Symbolic, Control, Info);
	}
	else
	{
		stat = umfpack_di_symbolic(mA, nA, A.p, A.irow, A.R, &Symbolic, Control, Info);
	}
	if ( stat  != UMFPACK_OK ) 
	{
		Scierror(999,_("%s: An error occurred: %s: %s\n"),fname,_("symbolic factorization"),UmfErrorMes(stat));
		return 0;
	};

	if (A.it == 1)
	{
		stat = umfpack_zi_numeric(A.p, A.irow, A.R, A.I, Symbolic, &Numeric, Control, Info);
	}
	else
	{
		stat = umfpack_di_numeric(A.p, A.irow, A.R, Symbolic, &Numeric, Control, Info);
	}

	if (A.it == 1) 
	{
		umfpack_zi_free_symbolic(&Symbolic); 
	}
	else
	{
		umfpack_di_free_symbolic(&Symbolic);
	}

	if ( stat  != UMFPACK_OK ) 
	{
		if (A.it == 1)
		{
			umfpack_zi_free_numeric(&Numeric); 
		}
		else
		{
			umfpack_di_free_numeric(&Numeric); 
		}
		Scierror(999,_("%s: An error occurred: %s: %s\n"),fname,_("numeric factorization"),UmfErrorMes(stat));
		return 0;
	};


	if ( Case == 1 )   /*  x = A\b  <=> Ax = b */
	{  
		if (A.it == 0)
		{
			for ( i = 0 ; i < nb ; i++ )
			{
				umfpack_di_wsolve(UMFPACK_A, A.p, A.irow, A.R, &xr[i*mb], &br[i*mb],
								  Numeric, Control, Info, Wi, W);
			}
			if (itb == 1)
			{
				for ( i = 0 ; i < nb ; i++ )
				{
					umfpack_di_wsolve(UMFPACK_A, A.p, A.irow, A.R, &xi[i*mb], &bi[i*mb],
									  Numeric, Control, Info, Wi, W);
				}
			}
		}
		else /*  A.it == 1  */
		{
			for ( i = 0 ; i < nb ; i++ )
			{
				umfpack_zi_wsolve(UMFPACK_A, A.p, A.irow, A.R, A.I, &xr[i*mb], &xi[i*mb], 
								  &br[i*mb], &bi[i*mb], Numeric, Control, Info, Wi, W);
			}
		}
	}
	else  /* Case == 2,   x = b/A  <=> x A = b <=> A.'x.' = b.' */
	{
		if (A.it == 0)
		{
			TransposeMatrix(br, mb, nb, xr);    /* put b in x (with transposition) */
			for ( i=0 ; i < mb ; i++ )
			{
				umfpack_di_wsolve(UMFPACK_At, A.p, A.irow, A.R, &br[i*nb], &xr[i*nb],
								  Numeric, Control, Info, Wi, W);      /* the solutions are in br */
			}
			TransposeMatrix(br, nb, mb, xr);         /* put now br in xr with transposition */
			if (itb == 1)
			{
				TransposeMatrix(bi, mb, nb, xi);    /* put b in x (with transposition) */
				for ( i=0 ; i < mb ; i++ )
				{
					umfpack_di_wsolve(UMFPACK_At, A.p, A.irow, A.R, &bi[i*nb], &xi[i*nb], 
									  Numeric, Control, Info, Wi, W);      /* the solutions are in bi */
				}
				TransposeMatrix(bi, nb, mb, xi);         /* put now bi in xi with transposition */
			}
		}
		else /*  A.it==1  */
		{
			TransposeMatrix(br, mb, nb, xr);
			TransposeMatrix(bi, mb, nb, xi);
			for ( i=0 ; i < mb ; i++ )
			{
				umfpack_zi_wsolve(UMFPACK_Aat, A.p, A.irow, A.R, A.I, &br[i*nb], &bi[i*nb],
								  &xr[i*nb], &xi[i*nb], Numeric, Control, Info, Wi, W);
			}
			TransposeMatrix(br, nb, mb, xr);
			TransposeMatrix(bi, nb, mb, xi);
		}
	}

	if (A.it == 1)
	{
		umfpack_zi_free_numeric(&Numeric);
	}
	else
	{
		umfpack_di_free_numeric(&Numeric);
	}

	LhsVar(1) = 5;
	C2F(putlhsvar)();

	return 0;
}
