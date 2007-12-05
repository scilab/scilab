/*------------------------------------------------------------------------*/
/* file: DrawAxesBoxStrategy.hxx                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for drawing the subwin surrounding parallelepiped      */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_AXES_BOX_STRATEGY_HXX_
#define _DRAW_AXES_BOX_STRATEGY_HXX_

#include "DrawableSubwin.h"

namespace sciGraphics
{

class DrawAxesBoxStrategy
{
public:

  DrawAxesBoxStrategy(void) {}

  virtual ~DrawAxesBoxStrategy(void) {}

  /**
   * Main funtion which actually draw the box.
   */
  virtual void drawAxesBox(void) = 0;

protected:
  

};

}

#endif /* _DRAW_AXES_BOX_STRATEGY_HXX_ */
