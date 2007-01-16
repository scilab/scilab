#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"
#include "spmalloc.h"


void spDeleteRowAndCol( char *eMatrix, int Row, int Col );

static int CountTwins(MatrixPtr Matrix, int Col, ElementPtr *ppTwin1, ElementPtr *ppTwin2 );


static int SwapCols( MatrixPtr Matrix, ElementPtr pTwin1, ElementPtr pTwin2 );

static void ScaleComplexMatrix( MatrixPtr Matrix, register  RealVector  RHS_ScaleFactors, register  RealVector  SolutionScaleFactors );


static void ComplexMatrixMultiply( MatrixPtr Matrix, RealVector RHS , RealVector Solution IMAG_VECTORS );

static void
ComplexTransposedMatrixMultiply( MatrixPtr Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS );
	 

static RealNumber ComplexCondition( MatrixPtr Matrix, RealNumber NormOfMatrix, int *pError );
