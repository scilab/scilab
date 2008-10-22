/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Create Java object matching Scilab uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "InitProgressionBar.h"
#include "BuildObjects.h"
#include "ObjectStructure.h"

sciPointObj *InitProgressionBar(void)
{
  sciPointObj *pObj = (sciPointObj *) NULL;
  
  if ((pObj = MALLOC (sizeof (sciPointObj))) == NULL)	return (sciPointObj *) NULL;
  
  sciSetEntityType (pObj, SCI_PROGRESSIONBAR);

  if ((pObj->pfeatures = MALLOC ((sizeof (sciProgressionbar)))) == NULL)
    {
      FREE(pObj);
      return (sciPointObj *) NULL;
    }

  pPROGRESSIONBAR_FEATURE(pObj)->hashMapIndex = 0;
  sciAddNewHandle(pObj);
  return (sciPointObj *) pObj;
}
/*----------------------------------------------------------------------------*/

