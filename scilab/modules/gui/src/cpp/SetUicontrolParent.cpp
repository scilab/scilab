/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the parent of an uicontrol object */

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
  
  if (valueType == sci_handles)
    {
      figure = sciGetPointerFromHandle(getHandleFromStack(stackPointer));
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
              
              // Scilab default values
              CallScilabBridge::setPushButtonPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, 20, sciGetHeight(figure) - 80, 40, 20);
              
              return SET_PROPERTY_SUCCEED;
            default:
              sciprint(_("No %s property for uicontrols of style: %s.\n"), "Parent", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
              return SET_PROPERTY_ERROR;
            }
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

