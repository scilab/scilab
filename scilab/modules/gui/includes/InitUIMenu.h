/* Vincent COUVERT -  Copyright INRIA 2007 */

#ifndef __INIT_UIMENU_H__
#define __INIT_UIMENU_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a Menu
 *
 * @param sciObj Scilab object for this Menu    
 */
void InitUIMenu(sciPointObj* sciObj);

/**
 * Set the parent of a Menu
 *
 * @param sciObj the menu object   
 * @param stackPointer adress in stack for the parent
 * @param valueType type of the parent in stack
 * @param nbRow number of row of the parent
 * @param nbCol number of columns of the parent
 * @return execution status
 */
int setMenuParent(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol);

/**
 * Disable/Enable a menu attached to the console
 *
 * @param name name of the menu 
 * @param status is TRUE to enable the menu, false else
 */
void EnableRootMenu(char *name, BOOL status);

/**
 * Disable/Enable a submenu attached to the console
 *
 * @param name name of the menu 
 * @param position the position of the menu in its parent
 * @param status is TRUE to enable the menu, false else
 */
void EnableRootSubMenu(char *name, int position, BOOL status);

/**
 * Disable/Enable a menu attached to a graphics window
 *
 * @param figurenum index of the window
 * @param name name of the menu 
 * @param status is TRUE to enable the menu, false else
 */
void EnableFigureMenu(int figurenum, char *name, BOOL status);

/**
 * Disable/Enable a submenu attached to a graphics window
 *
 * @param figurenum index of the window
 * @param name name of the menu 
 * @param position the position of the menu in its parent
 * @param status is TRUE to enable the menu, false else
 */
void EnableFigureSubMenu(int figurenum, char *name, int position, BOOL status);

/**
 * Delete a menu attached to the console
 *
 * @param name name of the menu 
 */
void DeleteRootMenu(char *name);

/**
 * Delete a menu attached to a graphics window
 *
 * @param figurenum index of the window
 * @param name name of the menu 
 */
void DeleteFigureMenu(int figurenum, char *name);

#endif /* !__INIT_UIMENU_H__ */
