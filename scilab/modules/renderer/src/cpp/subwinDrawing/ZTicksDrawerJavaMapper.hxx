/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing java methods needed by ZTicksDrawerJoGL  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Z_TICKS_DRAWER_JAVA_MAPPER_HXX_
#define _Z_TICKS_DRAWER_JAVA_MAPPER_HXX_

#include "TicksDrawerJavaMapper.hxx"
#include "ZTicksDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by XTicksDrawerJoGL 
 */
class ZTicksDrawerJavaMapper : public virtual TicksDrawerJavaMapper
{
public:

  ZTicksDrawerJavaMapper(void);

  virtual ~ZTicksDrawerJavaMapper(void);
  /*----------------------------------------------------------------------*/
  // Inherited from DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // Inherited from TicksDrawer
  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[],
                           int nbTicks, const double subticksPositions[], int nbSubticks);

  virtual bool checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks);

  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                           int nbTicks, const double subticksPositions[], int nbSubticks);

  virtual bool checkTicks(const double ticksPositions[], char * ticksLabels[],
                          char * labelsExponents[], int nbTicks);

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax);

  virtual void setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                  int fontType, double fontSize, int fontColor);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::ZTicksDrawerGL * m_pJavaObject;

};

}

#endif /* _Z_TICKS_DRAWER_JAVA_MAPPER_HXX_ */

