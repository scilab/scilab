/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy for computing ticks position and labels  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _USER_DEFINED_SUBTICKS_COMPUTER_HXX_
#define _USER_DEFINED_SUBTICKS_COMPUTER_HXX_

#include "DrawableSubwin.h"
#include "ComputeSubticksStrategy.hxx"

namespace sciGraphics
{

class UserDefinedSubticksComputer : public ComputeSubticksStrategy
{
public:

  UserDefinedSubticksComputer(DrawableSubwin * subwin);

  virtual ~UserDefinedSubticksComputer(void);

  /**
   * Get the number of subticks which will be computed by the strategy.
   */
  virtual int getNbSubticks(double ticksPositions[], int nbTicks);

  /**
   * Get the subticks positions.
   */
  virtual void getSubticksPosition(const double ticksPositions[], int nbTicks,
                                   double subTickspositions[]);

  /**
   * Set the number of subticks to draw in each interval
   */
  void setUserSubticks(int nbSubticks);

protected:

  int m_iNbSubticks;


};

}

#endif /* _USER_DEFINED_SUBTICKS_COMPUTER_HXX_ */

