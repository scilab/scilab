/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "clc.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
BOOL clc(int nblines)
{
	BOOL bOK = FALSE;
	if ( getScilabMode() == SCILAB_NWNI )
	{
		/* console C */
		if (nblines != -1)
		{
			/* impossible in this mode */
			return FALSE;
		}
		else
		{
#ifdef  _MSC_VER
			system("cls");	
			bOK = TRUE;
#else
			/* only on Windows */
			bOK = FALSE;
#endif
		}
	}
	else
	{
		/* console Java */
		if ( nblines >= -1)
		{
			/* TO DO : java */
			bOK = TRUE;
		}
		else
		{
			/* error */
			bOK = FALSE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
