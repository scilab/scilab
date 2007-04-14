/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __SPSOLVE_H__
#define __SPSOLVE_H__
#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"

static void SolveComplexMatrix( MatrixPtr Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS );
static void SolveComplexTransposedMatrix(MatrixPtr Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS );

#endif /* __SPSOLVE_H__ */