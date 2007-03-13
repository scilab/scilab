/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/


static int InitializeElementBlocks( MatrixPtr  Matrix, int InitialNumberOfElements, int NumberOfFillinsExpected );

static int RecordAllocation( MatrixPtr Matrix, char *AllocatedPtr );

static int AllocateBlockOfAllocationList( MatrixPtr Matrix );

ElementPtr spcGetElement( MatrixPtr Matrix );
   
ElementPtr spcGetFillin( MatrixPtr  Matrix );
