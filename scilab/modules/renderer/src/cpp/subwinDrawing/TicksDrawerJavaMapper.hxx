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
#include "TicksDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by TicksDrawerJoGL 
 */
class TicksDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  TicksDrawerJavaMapper(void);

  virtual ~TicksDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited from DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for TicksDrawer
  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[],
                           int nbTicks, const double subticksPositions[], int nbSubticks,
                           double axisSegmentStart[3], double axisSegmentEnd[3], double ticksDirection[3]);

  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                           int nbTicks, const double subticksPositions[], int nbSubticks,
                           double axisSegmentStart[3], double axisSegmentEnd[3], double ticksDirection[3]);

  virtual double showTicks(void);

  virtual void setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                  int fontType, double fontSize, int fontColor,
                                  bool useFractionalMetrics);

  virtual void setAxisLineDrawing(bool drawAxisLine);

  virtual void setNeedTicksDecimation(bool needTicksDecimation);
  /*----------------------------------------------------------------------*/

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::TicksDrawerGL * m_pJavaObject;

protected:


};

}

#endif /* _TICKS_DRAWER_JAVA_MAPPER_HXX_ */

