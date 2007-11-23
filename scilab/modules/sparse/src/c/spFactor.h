/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __SPFACTOR_H__
#define __SPFACTOR_H__

#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"
#include "spmalloc.h"
#include "spFortran.h"


static int FactorComplexMatrix( MatrixPtr Matrix );
static int CreateInternalVectors( MatrixPtr Matrix );
static int CountMarkowitz( MatrixPtr Matrix, register RealVector RHS, int Step );
static int MarkowitzProducts( MatrixPtr Matrix, int Step );
static ElementPtr SearchForPivot( MatrixPtr Matrix, int Step, int DiagPivoting );
static ElementPtr SearchForSingleton( MatrixPtr Matrix, int Step );
static ElementPtr QuicklySearchDiagonal( MatrixPtr Matrix, int Step );
static ElementPtr SearchDiagonal( MatrixPtr Matrix, register int Step );
static ElementPtr SearchEntireMatrix( MatrixPtr Matrix, int Step );
static RealNumber FindLargestInCol( register  ElementPtr pElement );
static RealNumber FindBiggestInColExclude( MatrixPtr Matrix, register  ElementPtr pElement, register  int Step );
static int ExchangeRowsAndCols( MatrixPtr Matrix,  ElementPtr pPivot, register int Step );
static int ExchangeColElements( MatrixPtr Matrix, int Row1, register  ElementPtr Element1, int Row2, register  ElementPtr Element2, int Column );
static int ExchangeRowElements( MatrixPtr Matrix, int Col1, register ElementPtr Element1, int Col2, register ElementPtr Element2, int Row );
static int RealRowColElimination( MatrixPtr Matrix, register ElementPtr pPivot );
static int ComplexRowColElimination( MatrixPtr Matrix, register ElementPtr pPivot );
static int UpdateMarkowitzNumbers( MatrixPtr Matrix, ElementPtr pPivot );
static ElementPtr CreateFillin( MatrixPtr Matrix, register int Row, int Col );
static int MatrixIsSingular( MatrixPtr Matrix, int Step );
static int ZeroPivot( MatrixPtr Matrix, int Step );


#endif /* __SPFACTOR_H__ */
