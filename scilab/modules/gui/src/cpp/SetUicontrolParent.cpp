/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the parent of an uicontrol object */

#include "SetUicontrolParent.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolParent(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{

  int parentFigureIndex = 0; 

  if (nbRow*nbCol != 1)
    {
      // Parent must be a single value
      return SET_PROPERTY_ERROR;
    }
  
  if (valueType == sci_handles)
    {
      if (sciGetEntityType(sciGetPointerFromHandle(getHandleFromStack(stackPointer))) == SCI_FIGURE)
        {
          // Scilab relationship
          sciSetParent(sciObj, sciGetPointerFromHandle(getHandleFromStack(stackPointer)));

          // The parent is a figure
          parentFigureIndex = sciGetNum(sciGetPointerFromHandle(getHandleFromStack(stackPointer)));
          
          if(strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
            {
              CallScilabBridge::setPushButtonParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
              return SET_PROPERTY_SUCCEED;
            }
          else
            {
              Scierror(999, _("Could not set parent of an uicontrol of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
              return SET_PROPERTY_ERROR;
            }
        }
      else
        {
          // Parent is a figure is not a figure
          Scierror(999, _("Could not set an other object than a figure as parent of an uicontrol.\n"));
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      // Do not know how to set the parent
      Scierror(999, _("Could not set an other object than a handle as parent of an uicontrol.\n"));
      return SET_PROPERTY_ERROR;
    }
}

