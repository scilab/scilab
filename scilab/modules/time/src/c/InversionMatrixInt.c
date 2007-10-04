/*-----------------------------------------------------------------------------------*/
/* @author Allan CORNET - INRIA 2005 */
/* @author Sylvestre LEDRU - INRIA 2007 (update) */
/*-----------------------------------------------------------------------------------*/
#include "InversionMatrixInt.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
int *InversionMatrixInt(int W,int L,int *Matrix)
{
	int *buffer = (int *)MALLOC( (W*L)*sizeof(int) );

	int i=0;
	int j=0;

	for (i=0; i<W; i++) for (j=0; j<L; j++) buffer[i*L+j] = Matrix[j*W+i];

	return buffer;
}
/*-----------------------------------------------------------------------------------*/