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

#include "InitWaitBar.h"
/*--------------------------------------------------------------------------*/
int sci_waitbar(char *fname,unsigned long fname_len)
{
  int waitbarID = 0;
  
  int nbRow = 0, nbCol = 0;
  int nbRowMessage = 0, nbColMessage = 0;

  int fractionAdr = 0;
  char **messageAdr = NULL;
  int handleAdr = 0;
  int stkAdr = 0;

  sciPointObj * pObj = NULL;
  unsigned long GraphicHandle = 0;

  CheckRhs(1,3);
  CheckLhs(1,1);

  if (Rhs == 1)
    {
      if (VarType(1) == sci_matrix) /* Fraction to display */
        {
          GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), "waitbar");
              return FALSE;
            }
        }
      else if (VarType(1) == sci_strings) /* Message to display */
        {
          GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single double or String expected.\n"), "waitbar");
          return FALSE;
        }

      /* Create a new waitbar */
      pObj = InitWaitBar();

      GraphicHandle=sciGetHandle(pObj);

      waitbarID = createWaitBar();
      pWAITBAR_FEATURE(pObj)->hashMapIndex = waitbarID;
      setWaitBarIndeterminateMode(waitbarID, FALSE);

      if (fractionAdr !=0)
        {
          setWaitBarValue(waitbarID, (int)(getDoubleFromStack((int)messageAdr) * 100));
        }
      else if (messageAdr != 0)
        {
          setWaitBarMessage(waitbarID, getStringMatrixFromStack((int)messageAdr), nbColMessage*nbRowMessage);
        }
    }
  else if (Rhs==2)
    {
      if (VarType(1) == sci_matrix && VarType(2) == sci_strings) /* waitbar(x,mes) */
        {
          GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), "waitbar");
              return FALSE;
            }
          GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);

          waitbarID = createWaitBar();
          pWAITBAR_FEATURE(pObj)->hashMapIndex = waitbarID;
          setWaitBarIndeterminateMode(waitbarID, FALSE);

          setWaitBarValue(waitbarID, (int)(getDoubleFromStack(fractionAdr) * 100));
          setWaitBarMessage(waitbarID, getStringMatrixFromStack((int)messageAdr), nbColMessage*nbRowMessage);
       }
      else if (VarType(1) == sci_matrix && VarType(2) == sci_handles) /* waitbar(x,winId) */
        {
          GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), "waitbar");
              return FALSE;
            }

          GetRhsVar(2, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for second input argument: Single handle expected.\n"), "waitbar");
              return FALSE;
            }

          GraphicHandle = (unsigned long)*hstk(handleAdr);
          pObj = sciGetPointerFromHandle(GraphicHandle);

          waitbarID = pWAITBAR_FEATURE(pObj)->hashMapIndex;
          setWaitBarValue(waitbarID, (int)(getDoubleFromStack(fractionAdr) * 100));
      }
      else if (VarType(1) == sci_strings && VarType(2) == sci_handles) /* waitbar(mes,winId) */
        {
          GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);

          GetRhsVar(2, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for second input argument: Single handle expected.\n"), "waitbar");
              return FALSE;
            }

          GraphicHandle = (unsigned long)*hstk(handleAdr);
          pObj = sciGetPointerFromHandle(GraphicHandle);

          waitbarID = pWAITBAR_FEATURE(pObj)->hashMapIndex;
          setWaitBarMessage(waitbarID, getStringMatrixFromStack((int)messageAdr), nbColMessage*nbRowMessage);
        }
      else
        {
          Scierror(999, _("%s: Bad use with two input arguments.\n"), "waitbar");
          return FALSE;
        }
    }
  else
    {
      if (VarType(1) == sci_matrix) /* Fraction */
        {
          GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), "waitbar");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single double expected.\n"), "waitbar");
          return FALSE;
        }

      if (VarType(2) == sci_strings) /* Message */
        {
          GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for second input argument: Single string expected.\n"), "waitbar");
          return FALSE;
        }

      if (VarType(3) == sci_handles) /* Handle */
        {
          GetRhsVar(3, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for third input argument: Single handle expected.\n"), "waitbar");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for third input argument: Single handle expected.\n"), "waitbar");
          return FALSE;
        }

      GraphicHandle = (unsigned long)*hstk(handleAdr);
      pObj = sciGetPointerFromHandle(GraphicHandle);
      waitbarID = pWAITBAR_FEATURE(pObj)->hashMapIndex;

      setWaitBarValue(waitbarID, (int)(getDoubleFromStack(fractionAdr) * 100));
      setWaitBarMessage(waitbarID, getStringMatrixFromStack((int)messageAdr), nbColMessage*nbRowMessage);
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
