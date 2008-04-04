/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks using JoGL   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TICKS_DRAWER_JOGL_HXX_
#define _TICKS_DRAWER_JOGL_HXX_

#include "../DrawableObjectJoGL.h"
#include "TicksDrawer.hxx"
#include "TicksDrawerJavaMapper.hxx"

namespace sciGraphics
{

class TicksDrawerJoGL : public TicksDrawer, public DrawableObjectJoGL
{
public:

  TicksDrawerJoGL(DrawableObject * drawer);

  virtual ~TicksDrawerJoGL(void);

protected:

  /**
   * Draw the ticks on the right axis.
   * @return distance from the ticks to axis in pixels
   */
  virtual double drawTicks(double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                           int nbTicks, double subticksPositions[], int nbSubtics);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);

  /**
   * Specify end drawing in setting all useful parameters.
   */
  virtual void endDrawing(void);

  /**
   * Same as initialize drawing but doesn't set constant parameters
   */
  virtual void initializeShowing(void);

  /**
   * Same as end drawing but doesn't set constant parameters
   */
  virtual void endShowing(void);

  /**
   * Get the object performing mapping with Java class.
   */
  TicksDrawerJavaMapper * getTicksDrawerJavaMapper(void);

};

}

#endif /* _TICKS_DRAWER_JOGL_HXX_ */

