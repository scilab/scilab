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

#include "TicksDrawerJoGL.hxx"
#include "TicksDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
TicksDrawerJoGL::TicksDrawerJoGL(DrawableObject * drawer)
  : TicksDrawerBridge(), DrawableObjectJoGL(drawer)
{
  setJavaMapper(new TicksDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJoGL::~TicksDrawerJoGL(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
double TicksDrawerJoGL::drawTicks(double relativeTicksPositions[],
                                  char * ticksLabels[],
                                  char * labelsExponents[],
                                  int nbTicks,
                                  double relativeSubticksPositions[],
                                  int nbSubtics,
                                  double axisSegmentStart[3],
                                  double axisSegmentEnd[3],
                                  double ticksDirection[3])
{
  if (labelsExponents == NULL)
  {
    return getTicksDrawerJavaMapper()->drawTicks(relativeTicksPositions, ticksLabels, nbTicks,
                                                 relativeSubticksPositions, nbSubtics,
                                                 axisSegmentStart, axisSegmentEnd, ticksDirection);
  }
  else
  {
    return getTicksDrawerJavaMapper()->drawTicks(relativeTicksPositions, ticksLabels,
                                                 labelsExponents, nbTicks,
                                                 relativeSubticksPositions, nbSubtics,
                                                 axisSegmentStart, axisSegmentEnd, ticksDirection);
  }
  
}
/*------------------------------------------------------------------------------------------*/
double TicksDrawerJoGL::showTicks(void)
{
  return getTicksDrawerJavaMapper()->showTicks();
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::initializeDrawing(void)
{
  // initialize drawing as usual
  DrawableObjectJoGL::initializeDrawing();

  // also set axis drawing parameters
  sciPointObj * pObj = getDrawer()->getDrawedObject();

  // set other parameters
  getTicksDrawerJavaMapper()->setAxisParamerters(sciGetLineStyle(pObj),
                                                 (float) sciGetLineWidth(pObj),
                                                 sciGetGraphicContext(pObj)->foregroundcolor,
                                                 sciGetFontStyle(pObj),
                                                 sciGetFontSize(pObj),
                                                 sciGetFontContext(pObj)->foregroundcolor,
                                                 sciGetIsUsingFractionalMetrics(pObj) == TRUE);

  
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::setNeedTicksDecimation(bool needDecimation)
{
  // specify if we need to check wether ticks are concealing or not.
  getTicksDrawerJavaMapper()->setNeedTicksDecimation(needDecimation);
}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * TicksDrawerJoGL::getTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
