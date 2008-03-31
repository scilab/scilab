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
#include "DrawableSubwin.h"
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
   * Set position of ticks and subTicks
   */
  virtual void setTicksPosition(const double ticksPosX[],
                                const double ticksPosY[],
                                const double ticksPosZ[],
                                int nbTicks);

  /**
   * Sepcify position of subticks
   */
  virtual void setSubticksPosition(const double subticksPosX[],
                                   const double subticksPosY[],
                                   const double subticksPosZ[],
                                   int nbSubticks);

  /**
   * Specify labels to draw in front of ticks
   */
  virtual void setTicksLabels(char * ticksLabels[],
                              char * labelsExponent[],
                              int nbLabels);

  /**
   * Specify axis segment position and ticks segments
   */
  virtual void setAxisPosition(const double axisSegmentStart[3],
                               const double axisSegmentEnd[3],
                               const double ticksDirection[3]);

  /**
   * Draw the ticks using precomputed positions
   * @return distance from ticks to the axis in pixels
   */
  virtual double concreteDrawTicks(void);

  /**
   * @return false if some of the ticks concealed and must be decimated
   *         true if ticks are correct and can be displayed
   */
  virtual bool checkTicks(void);

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

