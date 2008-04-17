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

#include "DestroyUicontrol.h"

/**
 * DestroyUicontrol
 * This function deletes Uicontrol structure.
 * @param sciPointObj * pthis pointer to the object structure
 * @return execution status
 */
int DestroyUicontrol (sciPointObj * pthis)
{
  if (pUICONTROL_FEATURE (pthis)->backgroundcolor != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->backgroundcolor);
    }

  if (pUICONTROL_FEATURE (pthis)->foregroundcolor != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->foregroundcolor);
    }

  if (pUICONTROL_FEATURE (pthis)->callback != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->callback);
    }

  if (pUICONTROL_FEATURE (pthis)->tag != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->tag);
    }

  if (pUICONTROL_FEATURE (pthis)->value != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->value);
    }

  if (pUICONTROL_FEATURE (pthis)->listboxTop != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->listboxTop);
    }

  if (pUICONTROL_FEATURE (pthis)->sliderStep != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->sliderStep);
    }

  if (pUICONTROL_FEATURE (pthis)->user_data != NULL)
    {
      FREE (pUICONTROL_FEATURE (pthis)->user_data);
    }

  return sciStandardDestroyOperations(pthis) ;
}
/*----------------------------------------------------------------------------*/

