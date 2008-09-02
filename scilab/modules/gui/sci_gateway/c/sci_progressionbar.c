/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "CallWaitBar.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"

#include "InitProgressionBar.h"
/*--------------------------------------------------------------------------*/
int sci_progressionbar(char *fname,unsigned long fname_len)
{
  int waitbarID = 0;
  
  int nbRow = 0, nbCol = 0;
  int nbRowMessage = 0, nbColMessage = 0;

  char **messageAdr = NULL;
  int handleAdr = 0;
  int stkAdr = 0;

  sciPointObj * pObj = NULL;
  unsigned long GraphicHandle = 0;

  CheckRhs(1,2);
  CheckLhs(1,1);

  if (Rhs == 1)
    {
      if (VarType(1) == sci_handles) /* ProgressionBar to update */
        {
          GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
              return FALSE;
            }
        }
      else if (VarType(1) == sci_strings) /* Message to display */
        {
          GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle or a string expected.\n"), fname, 1);
          return FALSE;
        }

      if (handleAdr == 0) 
        {
          /* Create a new ProgressionBar */
          pObj = InitProgressionBar();
          
          GraphicHandle=sciGetHandle(pObj);
          
          waitbarID = createWaitBar();
          pPROGRESSIONBAR_FEATURE(pObj)->hashMapIndex = waitbarID;
          setWaitBarIndeterminateMode(waitbarID, TRUE);
          setWaitBarMessage(waitbarID, getStringMatrixFromStack((size_t)messageAdr), nbColMessage*nbRowMessage);
        }
      else
        {
          GraphicHandle = (unsigned long)*hstk(handleAdr);
          pObj = sciGetPointerFromHandle(GraphicHandle);
          
          waitbarID = pPROGRESSIONBAR_FEATURE(pObj)->hashMapIndex;
          setWaitBarValue(waitbarID, 0); /* Update */
        }
    }
  else if (Rhs==2)
    {
      if (VarType(1) == sci_handles && VarType(2) == sci_strings) /* progressionbar(id,mes) */
        {
          GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
              return FALSE;
            }
          GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);

          GraphicHandle = (unsigned long)*hstk(handleAdr);
          pObj = sciGetPointerFromHandle(GraphicHandle);

          waitbarID = pPROGRESSIONBAR_FEATURE(pObj)->hashMapIndex;
          setWaitBarValue(waitbarID, 0); /* Update */
          setWaitBarMessage(waitbarID, getStringMatrixFromStack((size_t)messageAdr), nbColMessage*nbRowMessage);
       }
      else
        {
          Scierror(999, _("%s: Wrong input arguments: '%s' expected.\n"), fname, "(id, mes)");
          return FALSE;
        }
    }

  if (Lhs == 1)
    {
      nbRow = 1;
      nbCol = 1;
      CreateVar(Rhs+1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
      *hstk(stkAdr) = GraphicHandle;
      LhsVar(1) = Rhs+1;
    }
  else
    {
      LhsVar(1) = 0;
    }
  
  PutLhsVar();

  return TRUE;

}
/*--------------------------------------------------------------------------*/
