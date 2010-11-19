/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the units of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolUnits.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolUnits(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Units can be points, normalized, inches, centimeters or pixels */

  char * units = NULL; 

  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Units");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Units", "points", "normalized", "inches", "centimeters", "pixels");
          return SET_PROPERTY_ERROR;
        }
      
      units = getStringFromStack(stackPointer);

      if (strcmp(units, "points") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = POINTS_UNITS;
        }
      else if(strcmp(units, "normalized") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = NORMALIZED_UNITS;
        }
      else if(strcmp(units, "inches") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = INCHES_UNITS;
        }
      else if(strcmp(units, "centimeters") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = CENTIMETERS_UNITS;
        }
      else if(strcmp(units, "pixels") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = PIXELS_UNITS;
        }
      else
        {
          /* Wrong string format */
          Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Units", "points", "normalized", "inches", "centimeters", "pixels");
          return SET_PROPERTY_ERROR;
        }

      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Units", "points", "normalized", "inches", "centimeters", "pixels");
      return SET_PROPERTY_ERROR;
    }

}

