/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "InitUIMenu.hxx"
#include "getHandleDrawer.h"
#include "CallScilabBridge.hxx"
extern "C"{
#include "BOOL.h"
#include "getScilabJavaVM.h"
#include "CurrentObjectsManagement.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "getPropertyAssignedValue.h"
#include "localization.h"
#include "Scierror.h"

}

using namespace org_scilab_modules_gui_bridge;

void InitUIMenu(sciPointObj* sciObj)
{
  pUIMENU_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newMenu(getScilabJavaVM());
}

int setMenuParent(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{

  int parentFigureIndex = 0; 

  /* Special case to set current figure for parent */
  if (stackPointer == -1)
    {
      // Get current figure index
      parentFigureIndex = sciGetNum(sciGetCurrentFigure());
      // Send to Java
      CallScilabBridge::setFigureAsParent(getScilabJavaVM(), parentFigureIndex, pUIMENU_FEATURE(sciObj)->hashMapIndex);
      // Scilab relationship
      sciAddThisToItsParent(sciObj, sciGetCurrentFigure());
    }

  if (nbRow*nbCol != 1) {
    // Parent must be a single value
    return SET_PROPERTY_ERROR;
  }

  if (valueType == sci_handles)
    {
      if (sciGetEntityType(sciGetPointerFromHandle(getHandleFromStack(stackPointer))) == SCI_FIGURE)
        {

          // If the parent is a figure
          parentFigureIndex = sciGetNum(sciGetPointerFromHandle(getHandleFromStack(stackPointer)));
          CallScilabBridge::setFigureAsParent(getScilabJavaVM(), parentFigureIndex, pUIMENU_FEATURE(sciObj)->hashMapIndex);
          
          // Scilab relationship
          sciAddThisToItsParent(sciObj, sciGetPointerFromHandle(getHandleFromStack(stackPointer)));
          
          return SET_PROPERTY_SUCCEED;

        } 
      else if (sciGetEntityType(sciGetPointerFromHandle(getHandleFromStack(stackPointer))) == SCI_UIMENU)
        {

          // If the parent is a menu
          CallScilabBridge::setMenuAsParent(getScilabJavaVM(), pUIMENU_FEATURE(sciGetPointerFromHandle(getHandleFromStack(stackPointer)))->hashMapIndex, pUIMENU_FEATURE(sciObj)->hashMapIndex);
          
          // Scilab relationship
          sciAddThisToItsParent(sciObj, sciGetPointerFromHandle(getHandleFromStack(stackPointer)));
          
          return SET_PROPERTY_SUCCEED;

        } 
      else if (sciGetEntityType(sciGetPointerFromHandle(getHandleFromStack(stackPointer))) == SCI_UICONTEXTMENU)
        {

          // If the parent is a context menu
          CallScilabBridge::setMenuAsParent(getScilabJavaVM(), pUICONTEXTMENU_FEATURE(sciGetPointerFromHandle(getHandleFromStack(stackPointer)))->hashMapIndex, pUIMENU_FEATURE(sciObj)->hashMapIndex);
          
          // Scilab relationship
          sciAddThisToItsParent(sciObj, sciGetPointerFromHandle(getHandleFromStack(stackPointer)));
          
          return SET_PROPERTY_SUCCEED;

        } 
      else
        {
          Scierror(999, _("%s: Wrong type for parent: Figure or uimenu expected.\n"),"SetMenuParent");
          return SET_PROPERTY_ERROR;
        }
    }
  else if (valueType == sci_matrix)
    {
      // The parent is Scilab Main window (Console Tab)
      // TODO check that value is 0 
      CallScilabBridge::setRootAsParent(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex);
      return SET_PROPERTY_SUCCEED;
    } 
  else
    {
      Scierror(999, _("%s: Wrong type for parent: Figure or uimenu expected.\n"),"SetMenuParent");
      return SET_PROPERTY_ERROR;
    }
  
}


void EnableRootMenu(char *name, BOOL status)
{
  CallScilabBridge::setRootMenuEnabled(getScilabJavaVM(),  name, BOOLtobool(status));
}

void EnableRootSubMenu(char *name, int position, BOOL status)
{
  CallScilabBridge::setRootSubMenuEnabled(getScilabJavaVM(), name, position, BOOLtobool(status));
}

void EnableFigureMenu(int figurenum, char *name, BOOL status)
{
  CallScilabBridge::setFigureMenuEnabled(getScilabJavaVM(), figurenum, name, BOOLtobool(status));
}

void EnableFigureSubMenu(int figurenum, char *name, int position, BOOL status)
{
 CallScilabBridge::setFigureSubMenuEnabled(getScilabJavaVM(), figurenum, name, position, BOOLtobool(status));
}

void DeleteRootMenu(char *name)
{
   CallScilabBridge::removeRootMenu(getScilabJavaVM(), name);
}

void DeleteFigureMenu(int figurenum, char *name)
{
  CallScilabBridge::removeFigureMenu(getScilabJavaVM(), figurenum, name);
}
