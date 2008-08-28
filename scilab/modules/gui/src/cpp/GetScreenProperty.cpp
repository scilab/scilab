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
      value[2] = ConvertFromPixel((int) CallScilabBridge::getScreenWidth(getScilabJavaVM()), CENTIMETERS_UNITS, NULL, FALSE) * 10;
      value[3] = ConvertFromPixel((int) CallScilabBridge::getScreenHeight(getScilabJavaVM()), CENTIMETERS_UNITS, NULL, FALSE) * 10;
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screensize_cm"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = ConvertFromPixel((int) CallScilabBridge::getScreenWidth(getScilabJavaVM()), CENTIMETERS_UNITS, NULL, FALSE);
      value[3] = ConvertFromPixel((int) CallScilabBridge::getScreenHeight(getScilabJavaVM()), CENTIMETERS_UNITS, NULL, FALSE);
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screensize_in"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = ConvertFromPixel((int) CallScilabBridge::getScreenWidth(getScilabJavaVM()), INCHES_UNITS, NULL, FALSE);
      value[3] = ConvertFromPixel((int) CallScilabBridge::getScreenHeight(getScilabJavaVM()), INCHES_UNITS, NULL, FALSE);
      
      flag = sciReturnRowVector(value, 4);
      
      delete[] value;

      return flag;
    }
  else if(!stricmp(propertyName,"screensize_pt"))
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = ConvertFromPixel((int) CallScilabBridge::getScreenWidth(getScilabJavaVM()), POINTS_UNITS, NULL, FALSE);
      value[3] = ConvertFromPixel((int) CallScilabBridge::getScreenHeight(getScilabJavaVM()), POINTS_UNITS, NULL, FALSE);
      
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
      return SET_PROPERTY_ERROR;
    }
}
/*--------------------------------------------------------------------------*/
