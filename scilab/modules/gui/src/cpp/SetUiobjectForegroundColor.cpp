/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the foreground of an uicontrol or an uimenu 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUiobjectForegroundColor.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectForegroundColor(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Color can be [R, G, B] or "R|G|B" */

  int redInt = 0, greenInt = 0, blueInt = 0, nbvalues = 0;

  double * allcolors = NULL;
  
  double redDouble = 0.0, greenDouble = 0.0, blueDouble = 0.0;
  
  if (valueType == sci_strings)
    {
      if(nbCol != 1) /* More than one string */
        {
			Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
          return SET_PROPERTY_ERROR;
        }
      
      nbvalues = sscanf(getStringFromStack(stackPointer), "%lf|%lf|%lf", &redDouble, &greenDouble, &blueDouble);

      if (nbvalues != 3) /* Wrong format string */
        {
			Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
          return SET_PROPERTY_ERROR;
        }

      if (!checkColorRange(redDouble, greenDouble, blueDouble))
        {
          Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: Numbers between 0 and 1 expected.\n")), "BackgroundColor");
          return SET_PROPERTY_ERROR;
        }

      redInt = (int) (redDouble * 255);
      greenInt = (int) (greenDouble * 255);
      blueInt = (int) (blueDouble * 255);

      /* Store the values in Scilab */
      if (sciGetEntityType(sciObj) == SCI_UIMENU)
        {
          if (pUIMENU_FEATURE(sciObj)->foregroundcolor == NULL)
            {
              pUIMENU_FEATURE(sciObj)->foregroundcolor = new double[3];
            }
          pUIMENU_FEATURE(sciObj)->foregroundcolor[0] = redDouble;
          pUIMENU_FEATURE(sciObj)->foregroundcolor[1] = greenDouble;
          pUIMENU_FEATURE(sciObj)->foregroundcolor[2] = blueDouble;

        }
      else if (sciGetEntityType(sciObj) == SCI_UICONTROL)
        {
          if (pUICONTROL_FEATURE(sciObj)->foregroundcolor == NULL)
            {
              pUICONTROL_FEATURE(sciObj)->foregroundcolor = new double[3];
            }
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[0] = redDouble;
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[1] = greenDouble;
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[2] = blueDouble;
        }
      
    }
  else if (valueType == sci_matrix)
    {
      if(nbCol != 3 || nbRow != 1) /* Wrong matrix size */
        {
			Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
          return SET_PROPERTY_ERROR;
        }

       allcolors = getDoubleMatrixFromStack(stackPointer);

       if (!checkColorRange(allcolors[0], allcolors[1], allcolors[2]))
	 {
	   Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: Numbers between 0 and 1 expected.\n")), "BackgroundColor");
	   return SET_PROPERTY_ERROR;
	 }

       redInt = (int) (allcolors[0] * 255);
       greenInt = (int) (allcolors[1] * 255);
       blueInt = (int) (allcolors[2] * 255);
     
      /* Store the values in Scilab */
      if (sciGetEntityType(sciObj) == SCI_UIMENU)
        {
          if (pUIMENU_FEATURE(sciObj)->foregroundcolor == NULL)
            {
              pUIMENU_FEATURE(sciObj)->foregroundcolor = new double[3];
            }
          pUIMENU_FEATURE(sciObj)->foregroundcolor[0] = allcolors[0];
          pUIMENU_FEATURE(sciObj)->foregroundcolor[1] = allcolors[1];
          pUIMENU_FEATURE(sciObj)->foregroundcolor[2] = allcolors[2];

        }
      else if (sciGetEntityType(sciObj) == SCI_UICONTROL)
        {
          if (pUICONTROL_FEATURE(sciObj)->foregroundcolor == NULL)
            {
              pUICONTROL_FEATURE(sciObj)->foregroundcolor = new double[3];
            }
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[0] = allcolors[0];
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[1] = allcolors[1];
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[2] = allcolors[2];
        }
    }
  else
    {
      /* Wrong value type */
		Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
      return SET_PROPERTY_ERROR;
    }

 
  if(sciGetEntityType(sciObj) == SCI_UIMENU)  /* Sets the foreground of an uimenu */
    {
      CallScilabBridge::setWidgetForegroundColor(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex, redInt, greenInt, blueInt);
      return SET_PROPERTY_SUCCEED;
    }
  else if(sciGetEntityType(sciObj) == SCI_UICONTROL)  /* Sets the foreground of an uicontrol */
    {
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          CallScilabBridge::setFrameForegroundColor(getScilabJavaVM(), 
                                                     pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                     redInt, greenInt, blueInt);
        }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetForegroundColor(getScilabJavaVM(), 
                                                     pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                     redInt, greenInt, blueInt);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
		Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "ForegroundColor");
      return SET_PROPERTY_ERROR;
    }
}

