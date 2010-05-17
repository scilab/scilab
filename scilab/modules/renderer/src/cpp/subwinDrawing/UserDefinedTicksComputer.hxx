/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * desc : Compute ticks from ticks given by the user
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _USER_DEFINED_TICKS_COMPUTER_HXX_
#define _USER_DEFINED_TICKS_COMPUTER_HXX_

#include "ComputeTicksStrategy.hxx"

namespace sciGraphics
{

class UserDefinedTicksComputer: public ComputeTicksStrategy
{
public:

  UserDefinedTicksComputer(DrawableSubwin * subwin);

  virtual ~UserDefinedTicksComputer(void);

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
   * To know if the ticks computer needs decimation.
   */
  virtual bool needTicksDecimation(void) {return false;}

  /**
   * To compute the maximum number of ticks decimation iterations
   */
  virtual int computeMaxNumberOfDecimationIterations();

  /**
  * To know if we need to get ticks exponents or not.
  */
  virtual bool isDisplayingLabelsExponents(void) {return false;}

  /**
   * Set the parameter for drawing user ticks.
   * @param nbSubticks number of subticks between each ticks.
   */
  void setUserTicks(double * userTicksPos, char ** userLabels,
                    int nbUserTicks, int nbSubticks);

protected:

  double * m_aUserTicksPositions;
  char ** m_aUserTicksLabels;
  int m_iNbUserTicks;
  int m_iNbSubticks;

};

}

#endif /* _USER_DEFINED_TICKS_COMPUTER_HXX_ */

