/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * desc : interface for sci_uicontextmenu routine   
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
#include "gw_gui.h"
#include "CreateUIContextMenu.h"
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
/*--------------------------------------------------------------------------*/
int sci_uicontextmenu( char *fname,unsigned long fname_len )
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  unsigned long GraphicHandle = 0;

  CheckRhs(0,0);
  CheckLhs(0,1);

  /* Create a new context menu */
  GraphicHandle=sciGetHandle(CreateUIContextMenu());

  /* Create return variable */
  nbRow = 1;
  nbCol = 1;
  CreateVar(Rhs+1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
  *hstk(stkAdr) = GraphicHandle;

  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
