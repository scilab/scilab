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
  |   5) Interface code for getting some informations           |
  |      about the LU factors                                   |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [OK, n, lnz, unz, it] = umf_luinfo(LU_ptr)                |
  |                                                             |
  |    OK  : a boolean true if LU_ptr is a valid pointer to an  |
  |          umfpack LU numeric handle                          |
  |    n   : size of the matrix (n,n)                           |
  |    lnz : number of non zero elements in L                   |
  |    unz : number of non zero elements in U                   |
  |    it  : 0 the factors are real, 1 the factors are complex  |
  |                                                             |
  +------------------------------------------------------------*/
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"

extern CellAdr *ListNumeric;

int sci_umf_luinfo(char* fname, unsigned long l)
{
	int mLU_ptr, nLU_ptr, lLU_ptr;
	void * Numeric;
	int OK, lnz, unz, nrow, ncol, nz_udiag, it_flag;
	int zero = 0, one = 1, ind_OK, ind_nrow, ind_ncol, ind_lnz, ind_unz, ind_nzu, ind_it ;

	/* Check numbers of input/output arguments */
	CheckRhs(1,1); CheckLhs(1,7);

	/* get the pointer to the LU factors */
	GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mLU_ptr, &nLU_ptr, &lLU_ptr);
	Numeric = (void *) ((unsigned long int) *stk(lLU_ptr));

	/* Check if the pointer is a valid ref to ... */
	if ( IsAdrInList(Numeric, ListNumeric, &it_flag) )
		{
			if ( it_flag == 0 )
				umfpack_di_get_lunz(&lnz, &unz, &nrow, &ncol, &nz_udiag, Numeric);
			else
				umfpack_zi_get_lunz(&lnz, &unz, &nrow, &ncol, &nz_udiag, Numeric);
			OK = 1;
			CreateVar(2,MATRIX_OF_BOOLEAN_DATATYPE, &one, &one, &ind_OK);   *istk(ind_OK) = OK;
			CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_nrow); *stk(ind_nrow)= (double) nrow;
			CreateVar(4,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_ncol); *stk(ind_ncol)= (double) ncol;
			CreateVar(5,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_lnz);  *stk(ind_lnz) = (double) lnz;
			CreateVar(6,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_unz);  *stk(ind_unz) = (double) unz;
			CreateVar(7,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_nzu);  *stk(ind_nzu) = (double) nz_udiag;
			CreateVar(8,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &ind_it);   *stk(ind_it)  = (double) it_flag;
		}
	else
		{
			OK = 0;
			CreateVar(2,MATRIX_OF_BOOLEAN_DATATYPE, &one, &one, &ind_OK);   *istk(ind_OK) = OK;
			CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &ind_nrow);
			CreateVar(4,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &ind_ncol);
			CreateVar(5,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &ind_lnz);
			CreateVar(6,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &ind_unz);
			CreateVar(7,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &ind_nzu); 
			CreateVar(8,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &ind_it);
		}
	LhsVar(1) = 2;
	LhsVar(2) = 3;
	LhsVar(3) = 4;
	LhsVar(4) = 5;
	LhsVar(5) = 6;
	LhsVar(6) = 7;
	LhsVar(7) = 8;
	C2F(putlhsvar)();
	return 0;
}
