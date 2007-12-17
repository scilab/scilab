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
   * Reinitialize ticks computer data.
   */
  virtual void reinit(void) = 0;

  /**
   * Get the number of ticks which will be computed by the strategy.
   */
  virtual int getNbTicks(void) = 0;

  /**
   * Fill positions parameters
   * with the ticks abscissas, ordinate, ...
   */
  virtual void getTicksPosition(double positions[], char * labels[], char * labelsExponents[]) = 0;

  /**
   * Get the number of subticks which will be computed by the strategy.
   */
  virtual int getNbSubticks(double ticksPositions[], int nbTicks) = 0;

  /**
   * Get the subticks positions.
   */
  virtual void getSubticksPosition(const double ticksPositions[], int nbTicks,
                                   double subTickspositions[]) = 0;

  /**
   * To be called when there are too many ticks.
   * Compute a new sequence of ticks with less ticks.
   */
  virtual void reduceTicksNumber(void) = 0;

  /**
   * To know if the ticks computer needs decimation.
   */
  virtual bool needTicksDecimation(void) = 0;

  /**
   * To know if we need to get ticks exposants or not.
   */
  virtual bool isDisplayingLabelsExponents(void) = 0;

protected:

  DrawableSubwin * m_pDrawer;

};

}

#endif /* _COMPUTE_TICKS_STRATEGY_HXX_ */
