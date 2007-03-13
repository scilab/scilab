/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/

static void Translate( MatrixPtr Matrix, int *Row, int *Col );

static int EnlargeMatrix( MatrixPtr Matrix, register int NewSize );

static int ExpandTranslationArrays( MatrixPtr Matrix, register int NewSize );
	 
ElementPtr spcFindElementInCol( MatrixPtr Matrix, register ElementPtr *LastAddr, register int Row, int Col, SPBOOLEAN CreateIfMissing );

ElementPtr spcCreateElement( MatrixPtr Matrix, int Row, register int Col, register ElementPtr  * LastAddr, SPBOOLEAN Fillin );

int spcLinkRows( MatrixPtr Matrix );
