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
 |   4) Interface code to free the memory                      |
 |      used by the LU factors                                 |
 |                                                             |
 |   Scilab call                                               |
 |   -----------                                               |
 |   umf_ludel(LU_ptr)  or umf_ludel() for freed all fact.     |
 |                                                             |
 +------------------------------------------------------------*/
#include "MALLOC.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "Scierror.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"

extern CellAdr *ListNumeric;

int sci_umf_ludel(char* fname, unsigned long l)
{

  int mLU_ptr, nLU_ptr, lLU_ptr, it_flag;
  void * Numeric;
  CellAdr *Cell;
  
  /* Check numbers of input/output arguments */
  CheckRhs(0,1); CheckLhs(1,1);

  if (Rhs == 0)      /* destroy all */ 
    while ( ListNumeric )
      {
	Cell = ListNumeric;
	ListNumeric = ListNumeric->next;
	if (Cell->it == 0) 
	  umfpack_di_free_numeric(&(Cell->adr));
	else
	  umfpack_zi_free_numeric(&(Cell->adr));
	FREE(Cell);
      }
  else
    {
      /* get the pointer to the LU factors */
      GetRhsVar(1,SCILAB_POINTER_DATATYPE, &mLU_ptr, &nLU_ptr, &lLU_ptr);
      Numeric = (void *) ((unsigned long int) *stk(lLU_ptr));
      
      /* Check if the pointer is a valid ref to ... */
      if (RetrieveAdrFromList(Numeric, &ListNumeric, &it_flag)) 
	  {
		  /* free the memory of the numeric object */
		  if ( it_flag == 0 )
		  {
			umfpack_di_free_numeric(&Numeric);
		  }
		  else
		  {
			umfpack_zi_free_numeric(&Numeric);
		  }
	  }
	  else
	  {
		  Scierror(999,_("%s: Wrong value for input argument #%d: Must be a valid reference to (umf) LU factors.\n"),fname,1);
	  }
    }
  
  return 0;
}
