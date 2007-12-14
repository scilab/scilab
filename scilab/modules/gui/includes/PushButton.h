#ifndef __INIT_UIMENU_H__
#define __INIT_UIMENU_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new PushButton in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createPushButton(sciPointObj* sciObj);

/**
 * Set the parent for a Push Button in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @param stackPointer position in stack of the ID of the parent 
 * @param valueType type of the value in stack
 * @param nbRow number of rows of the value in stack
 * @param nbCol number of cols of the value in stack
 * @return true parent setting has be done without error
 */
int setPushButtonParent(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol);

/**
 * Set the current figure as parent for a Push Button in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsPushButtonParent(sciPointObj* sciObj);

#endif
