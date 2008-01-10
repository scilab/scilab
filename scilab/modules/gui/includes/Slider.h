/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new Slider in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createSlider(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a Slider in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsSliderParent(sciPointObj* sciObj);

#endif /* __SLIDER_H__ */
