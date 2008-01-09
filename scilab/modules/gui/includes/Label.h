/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#ifndef __LABEL_H__
#define __LABEL_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new Label in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createLabel(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a Label in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsLabelParent(sciPointObj* sciObj);

#endif /* __LABEL_H__ */
