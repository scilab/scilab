/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the parent of an uicontrol object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolParent.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolParent(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{

  int parentFigureIndex = 0; 

  sciPointObj *figure = NULL;

  if (nbRow*nbCol != 1)
    {
      sciprint(_("%s property value must be a single handle.\n"), "Parent");
      return SET_PROPERTY_ERROR;
    }
  
  if ((valueType == sci_handles) || (valueType == sci_matrix))
    {
      if (valueType == sci_handles)
        {
          figure = sciGetPointerFromHandle(getHandleFromStack(stackPointer));
        }

      if (valueType == sci_matrix)
        {
          figure = getFigureFromIndex((int)getDoubleMatrixFromStack(stackPointer)[0]);
        }

      if (sciGetEntityType(figure) == SCI_FIGURE)
        {
          // Scilab relationship
          sciAddThisToItsParent(sciObj, figure);

          // The parent is a figure
          parentFigureIndex = sciGetNum(figure);
          
          switch(pUICONTROL_FEATURE(sciObj)->style)
            {
            case SCI_PUSHBUTTON:
              CallScilabBridge::setPushButtonParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_EDIT:
              CallScilabBridge::setEditBoxParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_UITEXT:
              CallScilabBridge::setLabelParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_CHECKBOX:
              CallScilabBridge::setCheckBoxParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_RADIOBUTTON:
              CallScilabBridge::setRadioButtonParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_SLIDER:
              CallScilabBridge::setSliderParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_POPUPMENU:
              CallScilabBridge::setPopupMenuParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_LISTBOX:
              CallScilabBridge::setListBoxParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            case SCI_UIFRAME:
              CallScilabBridge::setFrameParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              break;
            default:
              sciprint(_("No %s property for uicontrols of style: %s.\n"), "Parent", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
              return SET_PROPERTY_ERROR;
            }
          // Scilab default values
          CallScilabBridge::setWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, 20, sciGetHeight(figure) - 80, 40, 20);
          return SET_PROPERTY_SUCCEED;
        }
      else
        {
          // Parent is not a figure
          sciprint(_("Could not set an other object than a figure as parent of an uicontrol.\n"));
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      // Do not know how to set the parent
      sciprint(_("Could not set an other object than a handle as parent of an uicontrol.\n"));
      return SET_PROPERTY_ERROR;
    }
}

