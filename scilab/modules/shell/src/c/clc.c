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
	if ( getScilabMode() == SCILAB_NWNI || getScilabMode() == SCILAB_NW )
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
