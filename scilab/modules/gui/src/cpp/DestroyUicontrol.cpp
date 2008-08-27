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

extern "C"
{
#include "DestroyJavaUiobject.h"
#include "DestroyUicontrol.h"
#include "DestroyObjects.h"
#include "MALLOC.h" /* MALLOC */
#include "GetProperty.h"
#include "GraphicSynchronizerInterface.h"
}
/**
 * DestroyUicontrol
 * This function deletes Uicontrol structure.
 * @param sciPointObj * pthis pointer to the object structure
 * @return execution status
 */
int  DestroyUicontrol (sciPointObj * pthis)
{
  /* Destroy Java object */
  disableFigureSynchronization(sciGetParentFigure(pthis));
  DestroyJavaUiobject(pthis);
  enableFigureSynchronization(sciGetParentFigure(pthis));

  if  (pUICONTROL_FEATURE (pthis)->backgroundcolor != NULL)
    {
      delete[] (pUICONTROL_FEATURE (pthis)->backgroundcolor);
    }

  if (pUICONTROL_FEATURE (pthis)->foregroundcolor != NULL)
    {
      delete[] (pUICONTROL_FEATURE (pthis)->foregroundcolor);
    }

  if (pUICONTROL_FEATURE (pthis)->callback != NULL)
    {
      delete[] (pUICONTROL_FEATURE (pthis)->callback);
    }

  if (pUICONTROL_FEATURE (pthis)->tag != NULL)
    {
      delete[] (pUICONTROL_FEATURE (pthis)->tag);
    }

  if (pUICONTROL_FEATURE (pthis)->value != NULL)
    {
      delete[] (pUICONTROL_FEATURE (pthis)->value);
    }

  if (pUICONTROL_FEATURE (pthis)->listboxTop != NULL)
    {
      delete[] (pUICONTROL_FEATURE (pthis)->listboxTop);
    }

  if (pUICONTROL_FEATURE (pthis)->sliderStep != NULL)
    {
      delete[] (pUICONTROL_FEATURE (pthis)->sliderStep);
    }

  return sciStandardDestroyOperations(pthis) ;
}
/*----------------------------------------------------------------------------*/

