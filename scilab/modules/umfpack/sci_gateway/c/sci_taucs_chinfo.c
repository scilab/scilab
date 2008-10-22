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
  |   10) Interface code for getting some informations          |
  |       about the Cholesky factor                             |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [OK, n, cnz] = taucs_chinfo(C_ptr)                        |
  |                                                             |
  |    OK  : a boolean true if C_ptr is a valid pointer to an   |
  |          taucs Choleski numeric handle                      |
  |    n   : size of the matrix (n,n)                           |
  |    cnz : number of non zero elements in C                   |
  |                                                             |
  +------------------------------------------------------------*/
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"

extern CellAdr *ListCholFactors;

int sci_taucs_chinfo(char* fname, unsigned long l)
{
	int mC_ptr, nC_ptr, lC_ptr;
	taucs_handle_factors * pC;
	int OK, cnz, n;
	int one = 1, ind_OK, ind_n, ind_cnz, it_flag;

	/* Check numbers of input/output arguments */
	CheckRhs(1,1); CheckLhs(1,3);

	/* get the pointer to the Choleski handle factor */
	GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mC_ptr, &nC_ptr, &lC_ptr);
	pC = (taucs_handle_factors *) ((unsigned long int) *stk(lC_ptr));

	/* Check if the pointer is a valid ref to ... */
	if ( IsAdrInList( (Adr)pC, ListCholFactors, &it_flag) )
		{
			n = pC->n;
			cnz = taucs_get_nnz_from_supernodal_factor(pC->C);
			OK = 1;
		}
	else
		{
			OK = 0; cnz = 0; n = 0;
		}
  
	CreateVar(2,MATRIX_OF_BOOLEAN_DATATYPE, &one, &one, &ind_OK);   *istk(ind_OK) = OK;
	CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_n);    *stk(ind_n)   = (double) n;
	CreateVar(4,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_cnz);  *stk(ind_cnz) = (double) cnz;

	LhsVar(1) = 2;
	LhsVar(2) = 3;
	LhsVar(3) = 4;
	C2F(putlhsvar)();
	return 0;
}
