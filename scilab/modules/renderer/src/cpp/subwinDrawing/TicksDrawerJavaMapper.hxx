/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing java methods needed by TicksDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TICKS_DRAWER_JAVA_MAPPER_HXX_
#define _TICKS_DRAWER_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by TicksDrawerJoGL 
 */
class TicksDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  TicksDrawerJavaMapper(void) : DrawableObjectJavaMapper() {}

  virtual ~TicksDrawerJavaMapper(void) {}

  /*----------------------------------------------------------------------*/
  // specific for TicksDrawer
  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[],
                           int nbTicks, const double subticksPositions[], int nbSubticks) = 0;

  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                         int nbTicks, const double subticksPositions[], int nbSubticks) = 0;

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax) = 0;

  virtual void setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                  int fontType, double fontSize, int fontColor) = 0;
  /*----------------------------------------------------------------------*/

protected:


};

}

#endif /* _TICKS_DRAWER_JAVA_MAPPER_HXX_ */

