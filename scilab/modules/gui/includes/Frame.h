/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#ifndef __FRAME_H__
#define __FRAME_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new Frame in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createFrame(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a Frame in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsFrameParent(sciPointObj* sciObj);

#endif /* !__FRAME_H__ */
