/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Set the tag of an uicontrol or uimenu
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUiobjectTag.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectTag(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be only one character string
  if (valueType != sci_strings) {
	  Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: A string expected.\n")), "Tag");
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
	  Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "Tag");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // No test needed about the style because not implemented in Java code

      // Free old tag is exists
      if(pUIMENU_FEATURE(sciObj)->tag != NULL)
        {
          delete[] (pUIMENU_FEATURE(sciObj)->tag);
        }
      // Set the new tag
      pUIMENU_FEATURE(sciObj)->tag = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pUIMENU_FEATURE(sciObj)->tag, getStringFromStack(stackPointer));

      return SET_PROPERTY_SUCCEED;
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // No test needed about the style because not implemented in Java code

      // Free old tag is exists
      if(pUICONTROL_FEATURE(sciObj)->tag != NULL)
        {
          delete[] (pUICONTROL_FEATURE(sciObj)->tag);
        }
      // Set the new tag
      pUICONTROL_FEATURE(sciObj)->tag = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pUICONTROL_FEATURE(sciObj)->tag, getStringFromStack(stackPointer));
      
      return SET_PROPERTY_SUCCEED;
    }
  else if (sciGetEntityType( sciObj ) == SCI_FIGURE)
    {
      // No test needed about the style because not implemented in Java code

      // Free old tag is exists
      if(pFIGURE_FEATURE(sciObj)->tag != NULL)
        {
          delete[] (pFIGURE_FEATURE(sciObj)->tag);
        }
      // Set the new tag
      pFIGURE_FEATURE(sciObj)->tag = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pFIGURE_FEATURE(sciObj)->tag, getStringFromStack(stackPointer));
      
			/* to avoid redraw */
      return SET_PROPERTY_UNCHANGED;
    }
  else
    {
		Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Tag");
      return SET_PROPERTY_ERROR;
    }
}

void destroyUiobjectTag(sciPointObj * pobj) {

	switch(sciGetEntityType(pobj))
	{
	case SCI_UICONTROL:
		if (pUICONTROL_FEATURE(pobj)->tag != NULL)
		{
			delete[] pUICONTROL_FEATURE(pobj)->tag;
			pUICONTROL_FEATURE(pobj)->tag = NULL;
		}
		break;
	case SCI_UIMENU:
		if (pUIMENU_FEATURE(pobj)->tag != NULL)
		{
			delete[] pUIMENU_FEATURE(pobj)->tag;
			pUIMENU_FEATURE(pobj)->tag = NULL;
		}
		break;
	case SCI_FIGURE:
		if (pFIGURE_FEATURE(pobj)->tag != NULL)
		{
			delete[] pFIGURE_FEATURE(pobj)->tag;
			pFIGURE_FEATURE(pobj)->tag = NULL;
		}
		break;
	default:
		Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Tag");
		break;
	}

}
