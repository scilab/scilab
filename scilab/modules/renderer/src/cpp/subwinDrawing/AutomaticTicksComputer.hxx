/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * desc : Compute automatic ticks
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _AUTOMATIC_TICKS_COMPUTER_HXX_
#define _AUTOMATIC_TICKS_COMPUTER_HXX_

#include "ComputeTicksStrategy.hxx"

namespace sciGraphics
{

class AutomaticTicksComputer: public ComputeTicksStrategy
{
public:

  AutomaticTicksComputer(DrawableSubwin * subwin);

  virtual ~AutomaticTicksComputer(void);

  /**
   * Reinitialize ticks computer data.
   */
  virtual void reinit(void);

  /**
   * Get the number of ticks which will be computed by the strategy.
   */
  virtual int getNbTicks(void);

  /**
   * Fill positions parameters
   * with the ticks abscissas, ordinate, ...
   */
  virtual void getTicksPosition(double positions[], char * labels[], char * labelsExponents[]);

  /**
   * To be called when there are too many ticks.
   * Compute a new sequence of ticks with less ticks.
   */
  virtual void reduceTicksNumber(void);

  /**
   * Performs the actual reduction
   */
  int reduceTicksNumber(int numberOfTicks);

  /**
   * To know if the ticks computer needs decimation.
   */
  virtual bool needTicksDecimation(void) {return true;}

  /**
   * To compute the maximum number of ticks decimation iterations
   * Assumes that m_iNbTicks is set to the number of undecimated ticks
   */
  virtual int computeMaxNumberOfDecimationIterations(void);

  /**
   * Set bounds between which we must compute ticks.
   */
  virtual void setAxisBounds(double min, double max);

  /**
   * To know if we need to get ticks exponents or not.
   */
  virtual bool isDisplayingLabelsExponents(void) {return false;}

protected:

  /** Current number of ticks */
  int m_iNbTicks;

  double m_dMinBounds;
  double m_dMaxBounds;

};

}

#endif /* _AUTOMATIC_TICKS_COMPUTER_HXX_ */

