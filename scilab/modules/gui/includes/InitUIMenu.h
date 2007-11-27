#ifndef __INIT_UIMENU_H__
#define __INIT_UIMENU_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

void InitUIMenu(sciPointObj* sciObj);
int set_label_property(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol);
int setMenuParent(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol);
int setMenuCallback(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol);
int get_label_property(sciPointObj* sciObj);
void EnableRootMenu(char *name, BOOL status);
void EnableFigureMenu(int figurenum, char *name, BOOL status);
void DeleteRootMenu(char *name);
void DeleteFigureMenu(int figurenum, char *name);

#endif
