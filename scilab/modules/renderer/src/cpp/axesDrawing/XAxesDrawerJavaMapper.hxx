/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing java methods needed by XAxesDrawerJoGL 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _X_AXES_DRAWER_JAVA_MAPPER_HXX_
#define _X_AXES_DRAWER_JAVA_MAPPER_HXX_

#include "../subwinDrawing/TicksDrawerJavaMapper.hxx"
#include "XAxesDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by XTicksDrawerJoGL 
 */
class XAxesDrawerJavaMapper : public virtual TicksDrawerJavaMapper
{
public:

  XAxesDrawerJavaMapper(void);

  virtual ~XAxesDrawerJavaMapper(void);
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
  // specific for XAxesDrawer
  virtual void setConstantCoordinates(double yCoordinate, double zCoordinate);

  virtual void setTicksDirection(char dir);

  virtual void setAxisLineDrawing(bool drawAxisLine);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_axesDrawing::XAxesDrawerGL * m_pJavaObject;

};

}

#endif /* _X_AXES_DRAWER_JAVA_MAPPER_HXX_ */

