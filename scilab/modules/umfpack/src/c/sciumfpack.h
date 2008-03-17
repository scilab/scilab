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
#include "machine.h"
#ifdef UMFPACK_SUITESPARSE
#include <suitesparse/umfpack.h>
#else
#include <umfpack.h>
#endif
#include "stackTypeVariable.h"

enum {NOT_ENOUGH_PLACE_IN_STK, MAT_IS_NOT_SPD, A_PRIORI_OK};   /* flags for spd_sci_sparse_to_taucs_sparse */

typedef void * Adr;

typedef struct _CellAdr CellAdr ;
struct _CellAdr {
	Adr adr;
	int it;   // added to see if the LU factors comes from a real or complex matrix
	CellAdr *next;
};

typedef struct  /* a type to handle a choleski factorisation */
{
	int * p;      /* for the permutation    */
	void * C;     /* for the factor (lower) */
	int n;        /* to stay the order (in place to read a member of C) */
} taucs_handle_factors;

typedef struct {
	int m;          /* number of rows */
	int n;          /* number of columns */
	int nel;        /* number of non nuls elements */
	int it;         /* flag type : it=-1 (boolean) it=0 (real) it=1 (complex) */
	int *p;         /* n+1 array : ptr_col[n] must be equal to nel */
	int *irow;   
	double *R;
	double *I;
} CcsSparse;

#define SciSparseToCcsSparse(num, A, B) if (! sci_sparse_to_ccs_sparse(num, A, B)) { return 0;}

