 /*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolStyle.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

/**
 ** \brief Get the style of an uicontrol object
 */

int GetUicontrolStyle(char *sciObjUID)
{
    char *pstrStyle = NULL;

    getGraphicObjectProperty(sciObjUID, __GO_STYLE__, jni_string, (void **) &pstrStyle);

  if (pstrStyle != NULL)
  {
      return sciReturnString(pstrStyle);
  }
  else
  {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Style");
      return FALSE;
  }

}
