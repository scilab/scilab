/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"

static void SolveComplexMatrix( MatrixPtr Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS );
static void SolveComplexTransposedMatrix(MatrixPtr Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS );
