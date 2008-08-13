/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy for computing ticks position and labels  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _COMPUTE_SUBTICKS_STRATEGY_HXX_
#define _COMPUTE_SUBTICKS_STRATEGY_HXX_

#include "DrawableSubwin.h"

namespace sciGraphics
{

class ComputeSubticksStrategy
{
public:

  ComputeSubticksStrategy(DrawableSubwin * subwin) {m_pDrawer = subwin;}

  virtual ~ComputeSubticksStrategy(void) {m_pDrawer = NULL;}

  /**
   * Get the number of subticks which will be computed by the strategy.
   */
  virtual int getNbSubticks(double ticksPositions[], int nbTicks) = 0;

  /**
   * Get the subticks positions.
   */
  virtual void getSubticksPosition(const double ticksPositions[], int nbTicks,
                                   double subTickspositions[]) = 0;

protected:

  DrawableSubwin * m_pDrawer;

};

}

#endif /* _COMPUTE_SUBTICKS_STRATEGY_HXX_ */

