/*------------------------------------------------------------------------*/
/* file: TicksDrawer.hxx                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks                              */
/*------------------------------------------------------------------------*/

#ifndef _TICKS_DRAWER_HXX_ 
#define _TICKS_DRAWER_HXX_

#include "../DrawableObjectBridge.h"
#include "ComputeTicksStrategy.hxx"
#include "DrawableSubwin.h"

namespace sciGraphics
{

class TicksDrawer : public virtual DrawableObjectBridge
{
public:
  
  TicksDrawer(DrawableSubwin * subwin);

  virtual ~TicksDrawer(void);

  void setTicksComputer(ComputeTicksStrategy * ticksComputer);

  /**
   * Main function which draw ticks.
   */
  void draw(void);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Draw the ticks on the right axis.
   */
  virtual void drawTicks(double ticksPositions[], char * ticksLabels[], int nbTicks,
                         double subticksPositions[], int nbSubtics) = 0;
  /*----------------------------------------------------------------------*/
  ComputeTicksStrategy * m_pTicksComputer;

  DrawableSubwin * m_pDrawer;
  /*----------------------------------------------------------------------*/
};

}

#endif /* _TICKS_DRAWER_HXX_ */