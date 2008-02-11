/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#ifndef __CONTEXTMENU_H__
#define __CONTEXTMENU_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new ContextMenu in Scilab GUIs
 *
 * @param menuLabels labels of the menu
 * @param nbLabels the number of labels
 */
void createContextMenu(char **menuLabels, int nbLabels);

#endif /* __CONTEXTMENU_H__ */
