/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __SPALLOCATE_H__
#define __SPALLOCATE_H__

#include "spDefs.h"

static int InitializeElementBlocks( MatrixPtr  Matrix, int InitialNumberOfElements, int NumberOfFillinsExpected );

static int RecordAllocation( MatrixPtr Matrix, char *AllocatedPtr );

static int AllocateBlockOfAllocationList( MatrixPtr Matrix );

ElementPtr spcGetElement( MatrixPtr Matrix );
   
ElementPtr spcGetFillin( MatrixPtr  Matrix );

#endif /* __SPALLOCATE_H__ */