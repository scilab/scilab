/*------------------------------------------------------------------------*/
/* file: ComputeTicksStrategy.hxx                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for computing ticks position and labels                */
/*------------------------------------------------------------------------*/

#ifndef _COMPUTE_TICKS_STRATEGY_HXX_
#define _COMPUTE_TICKS_STRATEGY_HXX_

#include "DrawableSubwin.h"

namespace sciGraphics
{

class ComputeTicksStrategy
{
public:

  ComputeTicksStrategy(DrawableSubwin * subwin) {m_pDrawer = subwin;}

  virtual ~ComputeTicksStrategy(void) {m_pDrawer = NULL;}

  /**
   * Get the number of ticks which will be computed by the strategy.
   */
  virtual int getNbTicks(void) = 0;


  /**
   * Fill positions parameters
   * with the ticks abscissas, ordinate, ...
   */
  virtual void getTicksPosition(double positions[]) = 0;

  /**
   * Get the label to display for each ticks.
   */
  virtual void getTicksLabels(char * labels[]) = 0;

  /**
   * Get the number of subticks which will be computed by the strategy.
   */
  virtual int getNbSubticks(void) = 0;

  /**
   * Get the subticks positions.
   */
  virtual void getSubticksPosition(double positions[]) = 0;

protected:

  DrawableSubwin * m_pDrawer;

};

}

#endif /* _COMPUTE_TICKS_STRATEGY_HXX_ */
