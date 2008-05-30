/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * desc : interface for sci_uiwait routine 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h> 
#include <string.h> 
/*--------------------------------------------------------------------------*/
#include "sci_uiwait.h"
#include "MALLOC.h" /* MALLOC */
#include "ObjectStructure.h"
#include "BuildObjects.h"
#include "gw_gui.h"
#include "localization.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "localization.h"
#include "Scierror.h"
#include "ContextMenu.h"
/*--------------------------------------------------------------------------*/
int sci_uiwait( char *fname,unsigned long fname_len )
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  char * result = NULL;

  long hdl = 0;

  sciPointObj *pObj = NULL;

  CheckRhs(1,1);
  CheckLhs(0,1);

  if (VarType(1) == sci_handles)
    {
      GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
      if (nbRow * nbCol !=1)
        {
          Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
          return FALSE;
        }
      hdl = (unsigned long)*hstk(stkAdr);
      pObj = sciGetPointerFromHandle(hdl);

      if (sciGetEntityType(pObj) == SCI_UICONTEXTMENU)
        {
          result = uiWaitContextMenu(pObj);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Uicontextmenu");
          return FALSE;
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
      return FALSE;
    }
 
  /* Create return variable */
  nbRow = (int)strlen(result);
  nbCol = 1;
  CreateVar(Rhs+1, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
  strcpy(cstk(stkAdr), result);

  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
