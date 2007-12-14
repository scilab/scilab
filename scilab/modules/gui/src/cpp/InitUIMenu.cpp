
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
#include "sciprint.h"
}

using namespace org_scilab_modules_gui_bridge;

void InitUIMenu(sciPointObj* sciObj)
{
  pUIMENU_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newMenu(getScilabJavaVM());
}

int setMenuParent(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{

  int parentFigureIndex = 0; 

  if (nbRow*nbCol != 1) {
    // Parent must be a single value
    return SET_PROPERTY_ERROR;
  }

  if (valueType == sci_handles) {
    if (sciGetEntityType(sciGetPointerFromHandle(getHandleFromStack(stackPointer))) == SCI_FIGURE) {

      // If the parent is a figure
      parentFigureIndex = sciGetNum(sciGetPointerFromHandle(getHandleFromStack(stackPointer)));
      CallScilabBridge::setFigureAsParent(getScilabJavaVM(), parentFigureIndex, pUIMENU_FEATURE(sciObj)->hashMapIndex);

      // Scilab relationship
      sciSetParent(sciObj, sciGetPointerFromHandle(getHandleFromStack(stackPointer)));
      
    } else {
      // If the parent is a menu
      CallScilabBridge::setMenuAsParent(getScilabJavaVM(), pUIMENU_FEATURE(sciGetPointerFromHandle(getHandleFromStack(stackPointer)))->hashMapIndex, pUIMENU_FEATURE(sciObj)->hashMapIndex);
    }
    return SET_PROPERTY_SUCCEED;
  
  } else if (valueType == sci_matrix) {
    // The parent is Scilab Main window (Console Tab)
    // TODO check that value is 0 
    CallScilabBridge::setRootAsParent(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex);
    return SET_PROPERTY_SUCCEED;
  } else {
    // Do not know how to set the parent
    return SET_PROPERTY_ERROR;
  }
  
}

int setMenuCallback(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Callback must be only one character string
  if (valueType != sci_strings) {
    // TODO display a message ?
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    // TODO display a message ?
    return SET_PROPERTY_ERROR;
  }

  if (nbRow == 0) {
    // This case should never happen beacause if nbRow==0 then nbCol is also 0
    // TODO display a message ?
    return SET_PROPERTY_ERROR;
  }

  // Send the callback to Java
  CallScilabBridge::setMenuCallback(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex, getStringFromStack(stackPointer));

  return SET_PROPERTY_SUCCEED;
  
}

void EnableRootMenu(char *name, BOOL status)
{
  CallScilabBridge::setRootMenuEnabled(getScilabJavaVM(), name, status);
}

void EnableFigureMenu(int figurenum, char *name, BOOL status)
{
  CallScilabBridge::setFigureMenuEnabled(getScilabJavaVM(), figurenum, name, status);
}

void DeleteRootMenu(char *name)
{
  CallScilabBridge::removeRootMenu(getScilabJavaVM(), name);
}

void DeleteFigureMenu(int figurenum, char *name)
{
  CallScilabBridge::removeFigureMenu(getScilabJavaVM(), figurenum, name);
}
