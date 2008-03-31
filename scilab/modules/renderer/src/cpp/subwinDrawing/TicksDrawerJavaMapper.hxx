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
  virtual double drawTicks(void);

  virtual bool checkTicks(void);

  virtual void setTicksPosition(const double ticksPosX[],
                                const double ticksPosY[],
                                const double ticksPosZ[],
                                int nbTicks);

  virtual void setSubticksPosition(const double subticksPosX[],
                                   const double subticksPosY[],
                                   const double subticksPosZ[],
                                   int nbSubticks);

  virtual void setTicksLabels(char * labels[],char * exponents[], int nbLabels);

  virtual void setTicksLabels(char * labels[], int nbLabels);

  virtual void setTicksDirection(double ticksDirX, double ticksDirY, double ticksDirZ);

  virtual void setAxisBounds(double axisStartX, double axisStartY, double axisStartZ,
                             double axisEndX, double axisEndY, double axisEndZ);
  
  virtual void setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                  int fontType, double fontSize, int fontColor,
                                  bool drawAxisSegment);
  /*----------------------------------------------------------------------*/

protected:

  org_scilab_modules_renderer_subwinDrawing::TicksDrawerGL * m_pJavaObject;

};

}

#endif /* _TICKS_DRAWER_JAVA_MAPPER_HXX_ */

