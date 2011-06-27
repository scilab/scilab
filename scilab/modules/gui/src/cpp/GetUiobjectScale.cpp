/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the Scale property of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectScale.hxx"

int GetUiobjectScale(char *sciObjUID)
{
  double *scale;
  int status = FALSE;

  getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_SCALE__), jni_double_vector, (void**) &scale);

  if (scale != NULL)
  {
      status = sciReturnRowVector(scale, 2);
      delete[] scale;
      return status;
  }

  Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Scale");
  return FALSE;

}
