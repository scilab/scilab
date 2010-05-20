/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * desc : Strategy for computing ticks position and labels
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
   * To be called when there are too many ticks.
   * Compute a new sequence of ticks with less ticks.
   */
  virtual void reduceTicksNumber(void) = 0;

  /**
   * To know if the ticks computer needs decimation.
   */
  virtual bool needTicksDecimation(void) = 0;

  /**
   * To compute the maximum number of ticks decimation iterations
   */
  virtual int computeMaxNumberOfDecimationIterations(void) = 0;

  /**
   * To know if we need to get ticks exponents or not.
   */
  virtual bool isDisplayingLabelsExponents(void) = 0;

protected:

  DrawableSubwin * m_pDrawer;

};

}

#endif /* _COMPUTE_TICKS_STRATEGY_HXX_ */

