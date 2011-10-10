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
#include "gw_umfpack.h"
#include "stack-c.h"
#include "sciumfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "localization.h"

int sci_res_with_prec(char* fname, unsigned long l)
{
	int mA, nA, mx, nx, lxr, lxi, itx, mb, nb, lbr, lbi, itb, lrr, lri, itr, ln, lni;
	int i, one=1, LastNum;
	SciSparse A;

	/* Check numbers of input/output arguments */
	CheckRhs(3,3);  CheckLhs(1,2);

	/* get A the sparse matrix */ 
	GetRhsVar(1, SPARSE_MATRIX_DATATYPE, &mA, &nA, &A);

	/* get x and b */
	GetRhsCVar(2, MATRIX_OF_DOUBLE_DATATYPE, &itx, &mx, &nx, &lxr, &lxi);
	GetRhsCVar(3, MATRIX_OF_DOUBLE_DATATYPE, &itb, &mb, &nb, &lbr, &lbi);
 
	if ( nx < 1 || nb != nx || mx != nA || mb != mA )
		{
			Scierror(999,_("%s: Wrong size for input arguments: Same sizes expected.\n"),fname);
			return 0;
		};

	if ( A.it == 1  ||  itx == 1  ||  itb == 1 )
		itr = 1;
	else
		itr = 0;

	CreateCVar(4, MATRIX_OF_DOUBLE_DATATYPE, &itr, &mb, &nb, &lrr, &lri);

	CreateVar(5, MATRIX_OF_DOUBLE_DATATYPE, &one, &nb, &ln);
	LastNum = 5;

	if ( itr == 0 )
		for ( i = 0 ; i < nb ; i++ )
			residu_with_prec(&A, stk(lxr+i*mx), stk(lbr+i*mb), stk(lrr+i*mb), stk(ln+i));
	else
		{
			if ( itx == 0 ) 
				{
					CreateVar(LastNum+1, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lxi);
					for ( i = 0 ; i < mx*nx ; i++ ) *stk(lxi+i) = 0.0;
					LastNum++;
				}
			if ( itb == 0 ) 
				{
					CreateVar(LastNum+1, MATRIX_OF_DOUBLE_DATATYPE, &mb, &nb, &lbi);
					for ( i = 0 ; i < mb*nb ; i++ ) *stk(lbi+i) = 0.0;
					LastNum++;
				}
			if ( A.it == 0 ) 
				{
					CreateVar(LastNum+1, MATRIX_OF_DOUBLE_DATATYPE, &one, &nb, &lni);
					for ( i = 0 ; i < nb ; i++ )
						residu_with_prec(&A, stk(lxr+i*mx), stk(lbr+i*mb), stk(lrr+i*mb), stk(ln+i));
					for ( i = 0 ; i < nb ; i++ )
						residu_with_prec(&A, stk(lxi+i*mx), stk(lbi+i*mb), stk(lri+i*mb), stk(lni+i));
					for ( i = 0 ; i < nb ; i++ )
						*stk(ln+i) = sqrt( *stk(ln+i)**stk(ln+i) +  *stk(lni+i)**stk(lni+i) );
				}
			else
				for ( i = 0 ; i < nb ; i++ )
					cmplx_residu_with_prec(&A, stk(lxr+i*mx), stk(lxi+i*mx),
										   stk(lbr+i*mb), stk(lbi+i*mb), 
										   stk(lrr+i*mb), stk(lri+i*mb), stk(ln+i));
		}

	LhsVar(1) = 4;
	LhsVar(2) = 5;
	PutLhsVar();
	return 0;
}
