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
  | 2) Interface code for the  LU factorization of A            |
  |                                                             |
  |    Scilab call                                              |
  |    -----------                                              |
  |        [LU_ptr] = umf_lufact(A)                             |
  |                                                             |
  |             A : a square real sparse matrix                 |
  |        LU_ptr : a pointer to the LU fact                    |
  |                                                             |
  |   Var description (to complete...)                          |
  |   ---------------                                           |
  |      mA : number of rows of the matrix A                    |
  |      nA : number of columns of A                            |
  |       A : pointer to a sparse matrix struct                 |
  |                                                             |
  |      Require mA, nA  > 0                                    |
  |                                                             |
  +------------------------------------------------------------*/
#include "gw_umfpack.h"
#include "sciumfpack.h"
#include "stack-c.h"
#include "Scierror.h"
#include "sciprint.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"
#include "warningmode.h"

CellAdr *ListNumeric = NULL;
CellAdr *ListCholFactors = NULL;

/* RAJOUTER un controle sur la taille du pivot */

int sci_umf_lufact(char* fname,unsigned long l)
{
	int mA, nA, stat, one=1;
	SciSparse AA;
	CcsSparse A;

	/* umfpack stuff */
	double *Control = (double *) NULL, *Info = (double *) NULL;
	void *Symbolic, *Numeric;
  
	/* Check numbers of input/output arguments */
	CheckRhs(1,1);  CheckLhs(1,1);

	/* get A the sparse matrix to factorize */ 
	GetRhsVar(1, SPARSE_MATRIX_DATATYPE, &mA, &nA, &AA);
	if (nA <= 0 || mA <= 0 )  
	{
		Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,1);
		return 0;
	};
   
	SciSparseToCcsSparse(2, &AA, &A);

	/* symbolic factorisation */
	if (A.it == 1)
		stat = umfpack_zi_symbolic(nA, mA, A.p, A.irow, A.R, A.I, &Symbolic, Control, Info);
	else
		stat = umfpack_di_symbolic(nA, mA, A.p, A.irow, A.R, &Symbolic, Control, Info);

	if ( stat != UMFPACK_OK ) 
	{
		Scierror(999,_("%s: An error occurred: %s: %s\n"),fname,_("symbolic factorization"),UmfErrorMes(stat));
		return 0;
	};
  
	/* numeric factorisation */
	if (A.it == 1)
		stat = umfpack_zi_numeric(A.p, A.irow, A.R, A.I, Symbolic, &Numeric, Control, Info);
	else
		stat = umfpack_di_numeric(A.p, A.irow, A.R, Symbolic, &Numeric, Control, Info);

	if (A.it == 1) 
		umfpack_zi_free_symbolic(&Symbolic); 
	else
		umfpack_di_free_symbolic(&Symbolic);

	if ( stat != UMFPACK_OK  &&  stat != UMFPACK_WARNING_singular_matrix ) 
	{
		Scierror(999,_("%s: An error occurred: %s: %s\n"),fname,_("symbolic factorization"),UmfErrorMes(stat));
		return 0;
	};

	if ( stat == UMFPACK_WARNING_singular_matrix  &&  mA == nA ) 
	{
		if (getWarningMode())
		{
			sciprint("\n%s:%s\n",_("Warning"),_("The (square) matrix appears to be singular."));
		}
	}

	/*  add the pointer in the list ListNumeric  */
	if (! AddAdrToList(Numeric, A.it, &ListNumeric))
	{ 
		/* AddAdrToList return 0 if malloc have failed : as it is just
		   for storing 2 pointers this is unlikely to occurs but ... */
		if (A.it == 1) 
		{
			umfpack_zi_free_numeric(&Numeric);
		}
		else
		{
			umfpack_di_free_numeric(&Numeric);
		}
		
		Scierror(999,_("%s: An error occurred: %s\n"),fname,_("no place to store the LU pointer in ListNumeric."));
		return 0;
	};

	/* create the scilab object to store the pointer onto the LU factors */
	CreateVarFromPtr(3,SCILAB_POINTER_DATATYPE,&one,&one, Numeric);

	/* return the pointer */
	LhsVar(1) = 3;
	C2F(putlhsvar)();
	return 0;
}
