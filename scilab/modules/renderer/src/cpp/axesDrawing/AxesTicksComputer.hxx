/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - Paul Griffiths
 * desc : Compute axes ticks
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _AXES_TICKS_COMPUTER_HXX_
#define _AXES_TICKS_COMPUTER_HXX_

#include "../subwinDrawing/ComputeTicksStrategy.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

class AxesTicksComputer: public ComputeTicksStrategy
{
public:

  AxesTicksComputer(DrawableAxes * axes);

  virtual ~AxesTicksComputer(void);

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
   * Pass NULL to labels and labelsExponents to only determine the ticks position.
   */
  virtual void getTicksPosition(double positions[], char * labels[], char * labelsExponents[]);

  /**
   * To be called when there are too many ticks.
   * Compute a new sequence of ticks with less ticks.
   */
  virtual void reduceTicksNumber(void);

  /**
   * To know if the ticks computer needs decimation.
   */
  virtual bool needTicksDecimation(void) {return false;}

  /**
   * To compute the maximum number of ticks decimation iterations
   */
  virtual int computeMaxNumberOfDecimationIterations(void);

  /**
   * To know if we need to get ticks exponents or not.
   */
  virtual bool isDisplayingLabelsExponents(void) {return false;}

protected:

  /*-----------------------------------------------------*/
  DrawableAxes * m_pAxes;
  /*-----------------------------------------------------*/


};

}

#endif /* _AXES_TICKS_COMPUTER_HXX_ */

