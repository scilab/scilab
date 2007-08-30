/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "clc.h"
#include "scilabmode.h"
#include "ClearConsole.h"
#include "ClearConsolePart.h"
#ifdef _MSC_VER
#include "windows/clrscr_nw.h"
#include "windows/clrscrPart_nw.h"
#endif
/*-----------------------------------------------------------------------------------*/
BOOL clc(int nblines)
{
	BOOL bOK = FALSE;
	if ( getScilabMode() != SCILAB_STD )
	{
		/* console C */
		if (nblines != -1)
		{
			#ifdef  _MSC_VER
				clrscrPart_nw(nblines);
				bOK = TRUE;
			#else
				/* impossible in this mode */
				bOK = FALSE;
			#endif
		}
		else
		{
			#ifdef  _MSC_VER
				clrscr_nw();
				bOK = TRUE;
			#else
				/* only on Windows */
				bOK = FALSE;
			#endif
		}
	}
	else
	{
          /* Java Console*/
          if ( nblines == -1) /* Clear the whole console window */
          {
              bOK = ClearConsole();
          }
          else if (nblines >= 0) /* Clear a part of the console window */
          {
              bOK = ClearConsolePart(nblines);
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
