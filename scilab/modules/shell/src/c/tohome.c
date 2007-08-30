/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "tohome.h"
#include "scilabmode.h"
#include "PromptToHome.h"
#ifndef _MSC_VER
#include "others/clrscr_nw.h"
#endif
/*-----------------------------------------------------------------------------------*/
BOOL tohome(void)
{
	BOOL bOK = FALSE;

	if ( getScilabMode() != SCILAB_STD )
	{
		#ifdef _MSC_VER
			/* not on Windows */
			bOK = FALSE;
		#else
			clrscr_nw();
			bOK = TRUE;
		#endif
	}
	else
	{
          /* Java Console */
          bOK = PromptToHome();
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
