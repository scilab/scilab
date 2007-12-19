#ifndef __INIT_UIMENU_H__
#define __INIT_UIMENU_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * @TODO add comment
 *
 * @param sciObj    
 */
void InitUIMenu(sciPointObj* sciObj);

/**
 * @TODO add comment
 *
 * @param sciObj    
 * @param stackPointer  
 * @param valueType 
 * @param nbRow 
 * @param nbCol 
 * @return <ReturnValue>
 */
int setMenuParent(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol);

/**
 * @TODO add comment
 *
 * @param name  
 * @param status    
 */
void EnableRootMenu(char *name, BOOL status);

/**
 * @TODO add comment
 *
 * @param figurenum 
 * @param name  
 * @param status    
 */
void EnableFigureMenu(int figurenum, char *name, BOOL status);

/**
 * @TODO add comment
 *
 * @param name  
 */
void DeleteRootMenu(char *name);

/**
 * @TODO add comment
 *
 * @param figurenum 
 * @param name  
 */
void DeleteFigureMenu(int figurenum, char *name);

#endif
