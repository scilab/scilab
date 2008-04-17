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

#include "DestroyUimenu.h"

/**
 * DestroyUimenu
 * This function deletes Uimenu structure.
 * @param sciPointObj * pthis pointer to the object structure
 * @return execution status
 */
int DestroyUimenu (sciPointObj * pthis)
{
  if (pUIMENU_FEATURE (pthis)->foregroundcolor != NULL)
    {
      FREE (pUIMENU_FEATURE (pthis)->foregroundcolor);
    }

  if (pUIMENU_FEATURE (pthis)->callback != NULL)
    {
      FREE (pUIMENU_FEATURE (pthis)->callback);
    }

  if (pUIMENU_FEATURE (pthis)->tag != NULL)
    {
      FREE (pUIMENU_FEATURE (pthis)->tag);
    }

  if (pUIMENU_FEATURE (pthis)->user_data != NULL)
    {
      FREE (pUIMENU_FEATURE (pthis)->user_data);
    }

  return sciStandardDestroyOperations(pthis) ;
}
/*----------------------------------------------------------------------------*/

