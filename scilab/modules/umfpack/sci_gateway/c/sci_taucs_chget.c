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
  |   11) Interface code for getting the Choleky factorisation  |
  |       p and C                                               |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [C,p] = taucs_chget(C_ptr)                                |
  |                                                             |
  |    C  : the upper triangle                                  |
  |    p  : the permutation                                     |
  |                                                             |
  +------------------------------------------------------------*/
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "localization.h"

extern CellAdr *ListCholFactors;

int sci_taucs_chget(char* fname, unsigned long l)
{

	int mC_ptr, nC_ptr, lC_ptr;
	taucs_handle_factors * pC;
	taucs_ccs_matrix * C;
	SciSparse S;
	int one = 1, nnz, lp, i, pl_miss, it_flag;

	/* Check numbers of input/output arguments */
	CheckRhs(1,1); CheckLhs(1,3);

	/* get the pointer to the Choleski factorisation handle */
	GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mC_ptr, &nC_ptr, &lC_ptr);
	pC = (taucs_handle_factors *) ((unsigned long int) *stk(lC_ptr));

	/* Check if the pointer is a valid ref to ... */
	if (! IsAdrInList( (Adr)pC, ListCholFactors, &it_flag) )
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: Must be a valid reference to a Cholesky factorisation"),fname,1);
			return 0;
		};

	C = taucs_supernodal_factor_to_ccs(pC->C);
	if (! C) 
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	};

	/* set up S fields */
	nnz = 0;
	for (i = 0 ; i < C->m ; i++)
		{
			C->colptr[i] = C->colptr[i+1] - C->colptr[i];
			nnz += C->colptr[i];
		}
	for ( i = 0 ; i < nnz ; i++ )
		C->rowind[i]++;
  
	S.m = C->m ; S.n = C->n; S.it = 0; S.nel = nnz;
	S.R = C->values; S.I = NULL; 
	S.icol = C->rowind; S.mnel = C->colptr;
  
	if (! test_size_for_sparse(2 , S.m, S.it, S.nel, &pl_miss))
	{
		taucs_ccs_free(C); 
		Scierror(999,_("%s: No more memory : increase stacksize %d supplementary words needed.\n"),fname, pl_miss);
		return 0;
	}
	CreateVarFromPtr(2,SPARSE_MATRIX_DATATYPE,&S.m,&S.n,&S);  
	taucs_ccs_free(C); 
  
	/* now p */
	CreateVar(3,MATRIX_OF_INTEGER_DATATYPE, &(S.m), &one, &lp); 
	for  (i = 0 ; i < S.m ; i++) *istk(lp+i) = pC->p[i]+1;

	LhsVar(1) = 2;
	LhsVar(2) = 3;
	PutLhsVar();
	return 0;
}
