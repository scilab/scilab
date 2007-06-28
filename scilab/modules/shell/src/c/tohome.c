/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "tohome.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
BOOL tohome(void)
{
	BOOL bOK = FALSE;

	if ( getScilabMode() == SCILAB_NWNI )
	{
		#ifdef _MSC_VER
			bOK = FALSE;
		#else
			system("clear"); /* @TODO What a crappy code ... !!! Must be changed !! */
			bOK = TRUE;
		#endif
	}
	else
	{
		/* console Java */
		bOK = TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
