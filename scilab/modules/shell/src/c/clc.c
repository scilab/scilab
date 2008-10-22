/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "clc.h"
#include "scilabmode.h"
#include "ClearConsole.h"
#include "ClearConsolePart.h"
#ifdef _MSC_VER
#include "windows/clrscr_nw.h"
#include "windows/clrscrPart_nw.h"
#endif
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
