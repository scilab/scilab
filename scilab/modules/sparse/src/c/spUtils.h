#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"
#include "spmalloc.h"

#if TRANSLATE AND DELETE
void spDeleteRowAndCol( char *eMatrix, int Row, int Col );
#endif

static int CountTwins(MatrixPtr Matrix, int Col, ElementPtr *ppTwin1, ElementPtr *ppTwin2 );


static int SwapCols( MatrixPtr Matrix, ElementPtr pTwin1, ElementPtr pTwin2 );

#if spCOMPLEX AND SCALING
static void ScaleComplexMatrix( MatrixPtr Matrix, register  RealVector  RHS_ScaleFactors, register  RealVector  SolutionScaleFactors );
#endif

#if spCOMPLEX AND MULTIPLICATION
static void ComplexMatrixMultiply( MatrixPtr Matrix, RealVector RHS , RealVector Solution IMAG_VECTORS );
#endif

#if spCOMPLEX AND MULTIPLICATION AND TRANSPOSE
static void
ComplexTransposedMatrixMultiply( MatrixPtr Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS );
#endif

#if spCOMPLEX
static RealNumber ComplexCondition( MatrixPtr Matrix, RealNumber NormOfMatrix, int *pError );
#endif
