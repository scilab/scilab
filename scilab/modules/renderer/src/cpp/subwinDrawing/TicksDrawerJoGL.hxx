/*------------------------------------------------------------------------*/
/* file: TicksDrawerJoGL.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks using JoGL                   */
/*------------------------------------------------------------------------*/

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

  TicksDrawerJoGL(DrawableSubwin * subwin);

  virtual ~TicksDrawerJoGL(void);

protected:

  /**
   * Draw the ticks on the right axis.
   */
  virtual void drawTicks(double ticksPositions[], char * ticksLabels[], int nbTicks,
                         double subticksPositions[], int nbSubtics);

  /**
   * Check if the ticks do not conceal each others.
   */
  virtual bool checkTicks(double ticksPositions[], char * ticksLabels[], int nbTicks);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);

  /**
   * Get the object performing mapping with Java class.
   */
  TicksDrawerJavaMapper * getTicksDrawerJavaMapper(void);

};

}

#endif /* _TICKS_DRAWER_JOGL_HXX_ */
