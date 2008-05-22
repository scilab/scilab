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
 |   9) Interface code to free the memory                      |
 |      used by the Cholesky factors (C + permutation)         |
 |                                                             |
 |   Scilab call                                               |
 |   -----------                                               |
 |   taucs_chdel(LU_ptr)  or taucs_chdel() for freed all fact. |
 |                                                             |
 +------------------------------------------------------------*/
#include "sciumfpack.h"
#include "taucs_scilab.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"

extern CellAdr *ListCholFactors;

int sci_taucs_chdel(char* fname, unsigned long l)
{

	int mC_ptr, nC_ptr, lC_ptr, it_flag;
	taucs_handle_factors * pC;
	CellAdr * Cell;
	int NbRhsVar;

	/* Check numbers of input/output arguments */
	CheckRhs(0,1); CheckLhs(1,1);

	NbRhsVar = Rhs;

	if (NbRhsVar == 0)      /* destroy all */ 
		while ( ListCholFactors )
			{
				Cell = ListCholFactors;
				ListCholFactors = ListCholFactors->next;
				pC = (taucs_handle_factors *) Cell->adr;
				taucs_supernodal_factor_free(pC->C);  /* free the super nodal struct */
				FREE(pC->p);                          /* free the permutation vector */
				FREE(pC);                             /* free the handle             */
				FREE(Cell);                           
			}
	else
		{
			/* get the pointer to the Cholesky factors */
			GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mC_ptr, &nC_ptr, &lC_ptr);
			pC = (taucs_handle_factors *) ((unsigned long int) *stk(lC_ptr));
      
			/* Check if the pointer is a valid ref to ... */
			if (RetrieveAdrFromList(pC, &ListCholFactors, &it_flag)) 
				/* free the memory of the objects */
				{
					taucs_supernodal_factor_free(pC->C);
					FREE(pC->p);
					FREE(pC);
				}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: not a valid reference to Cholesky factors.\n"),fname,1);
			}
		}
	return 0;
}
