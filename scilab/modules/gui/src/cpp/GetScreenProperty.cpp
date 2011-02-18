/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Get the position of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetScreenProperty.hxx"

using namespace org_scilab_modules_gui_bridge;


int GetScreenProperty(char *propertyName)
{
  double *value = new double[4];

  int flag = SET_PROPERTY_ERROR;

  if(!stricmp(propertyName,"screensize_px"))
    {
      value[0] = 1.0;
      value[1] = 1.0;
      value[2] = CallScilabBridge::getScreenWidth(getScilabJavaVM());
      value[3] = CallScilabBridge::getScreenHeight(getScilabJavaVM());
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
      
    }
  else if(!stricmp(propertyName,"screensize_mm"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = pixelTomm( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
      value[3] = pixelTomm( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screensize_cm"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = pixelTocm( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
      value[3] = pixelTocm( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screensize_in"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = pixelToinch( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
      value[3] = pixelToinch( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screensize_pt"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = pixelTopt( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
      value[3] = pixelTopt( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screensize_norm"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = 1.0;
      value[3] = 1.0;
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screendepth"))
    {
      value[0] = CallScilabBridge::getScreenDepth(getScilabJavaVM());
      
      flag = sciReturnRowVector(value, 1);
      
      delete[] value;

      return flag;
    }
  else
    {
        delete[] value;
      return SET_PROPERTY_ERROR;
    }
}
/*--------------------------------------------------------------------------*/
