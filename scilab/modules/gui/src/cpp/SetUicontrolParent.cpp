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
#include "SetUicontrolPosition.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolParent(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{

  int parentFigureIndex = 0; 

  int * returnValues = NULL;

  sciPointObj *figure = NULL;
  sciPointObj *parent = NULL;
  sciPointObj *oldParentFigure = NULL;

  if (nbRow*nbCol != 1)
    {
      Scierror(999, _("Wrong size for '%s' property: A graphic handle expected.\n"), "Parent");
      return SET_PROPERTY_ERROR;
    }
  
  if ((valueType == sci_handles) || (valueType == sci_matrix))
    {
      if (valueType == sci_handles)
        {
          figure = sciGetPointerFromHandle(getHandleFromStack(stackPointer));

          parent = figure;
          
          /* Set a Frame as parent: we need to get the parent figure */
          if (sciGetEntityType(figure) != SCI_FIGURE)
            {
              while (sciGetEntityType(figure) != SCI_FIGURE)
                {
                  figure = sciGetParent(figure);
                }
            }
        }

      if (valueType == sci_matrix)
        {
          figure = getFigureFromIndex((int)getDoubleMatrixFromStack(stackPointer)[0]);
          parent = figure;
        }

      if (figure == NULL)
        {
          // Can not set the parent
          Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
          return SET_PROPERTY_ERROR;
        }

      if (sciGetEntityType(figure) == SCI_FIGURE)
        {
          // Get the position from Java to reset it
          if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
            {
              returnValues = CallScilabBridge::getFramePosition(getScilabJavaVM(),
                                                                pUICONTROL_FEATURE(sciObj)->hashMapIndex);
            }
          else
            {
              returnValues = CallScilabBridge::getWidgetPosition(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex);
            }

          // Remove from previous parent
          if (sciGetParent(sciObj) != NULL)
            {
              oldParentFigure = sciGetParent(sciObj);
              if (sciGetEntityType(oldParentFigure) != SCI_FIGURE)
                {
                  while (sciGetEntityType(oldParentFigure) != SCI_FIGURE)
                    {
                      oldParentFigure = sciGetParent(oldParentFigure);
                    }
                }
              parentFigureIndex = sciGetNum(oldParentFigure);

              sciDelThisToItsParent(sciObj, sciGetParent(sciObj));

              switch(pUICONTROL_FEATURE(sciObj)->style)
                {
                case SCI_PUSHBUTTON:
                  CallScilabBridge::removePushButtonFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_EDIT:
                  CallScilabBridge::removeEditBoxFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_UITEXT:
                  CallScilabBridge::removeLabelFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_CHECKBOX:
                  CallScilabBridge::removeCheckBoxFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_RADIOBUTTON:
                  CallScilabBridge::removeRadioButtonFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_SLIDER:
                  CallScilabBridge::removeSliderFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_POPUPMENU:
                  CallScilabBridge::removePopupMenuFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_LISTBOX:
                  CallScilabBridge::removeListBoxFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                case SCI_UIFRAME:
                  CallScilabBridge::removeFrameFromParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
                  break;
                default:
                  Scierror(999, _("No '%s' property for uicontrols of style: %s.\n"), "Parent", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
				  delete [] returnValues;
                  return SET_PROPERTY_ERROR;
                }
            }
          
          // Scilab relationship
          sciAddThisToItsParent(sciObj, parent);

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
              Scierror(999, _("No '%s' property for uicontrols of style: %s.\n"), "Parent", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
			  delete [] returnValues;
              return SET_PROPERTY_ERROR;
            }

          // Adjust position: position read from java and reset
          if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
            {
              CallScilabBridge::setFramePosition(getScilabJavaVM(), 
                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                 returnValues[0], 
                                                 returnValues[1], 
                                                 returnValues[2], 
                                                 returnValues[3]);
            }
          else /* All other uicontrol styles */
            {
              CallScilabBridge::setWidgetPosition(getScilabJavaVM(), 
                                                  pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                  returnValues[0], 
                                                  returnValues[1], 
                                                  returnValues[2], 
                                                  returnValues[3]);
            }
		  delete [] returnValues;
          return SET_PROPERTY_SUCCEED;
        }
      else
        {
		  delete [] returnValues;
          // Parent is not a figure
          Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
	  delete [] returnValues;
      // Do not know how to set the parent
      Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
      return SET_PROPERTY_ERROR;
    }
}

