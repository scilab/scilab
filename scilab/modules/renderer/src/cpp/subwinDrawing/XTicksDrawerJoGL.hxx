/*------------------------------------------------------------------------*/
/* file: XTicksDrawerJoGL.hxx                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for X axis                   */
/*------------------------------------------------------------------------*/

#ifndef _X_TICKS_DRAWER_JOGL_HXX_
#define _X_TICKS_DRAWER_JOGL_HXX_

#include "TicksDrawer.hxx"
#include "../DrawableObjectJoGL.h"
#include "XTicksDrawerJavaMapper.hxx"

namespace sciGraphics
{

class XTicksDrawerJoGL: public TicksDrawer, public DrawableObjectJoGL
{
public:

  XTicksDrawerJoGL(DrawableSubwin * subwin);

  virtual ~XTicksDrawerJoGL(void);

  

protected:

  /**
   * Draw the ticks on the X axis.
   */
  virtual void drawTicks(double ticksPositions[], char * ticksLabels[], int nbTicks,
                         double subticksPositions[], int nbSubtics);

  /**
   * Get the object performing mapping with Java class.
   */
  XTicksDrawerJavaMapper * getXTicksDrawerJavaMapper(void);

};

}

#endif /* _X_TICKS_DRAWER_JOGL_HXX_ */