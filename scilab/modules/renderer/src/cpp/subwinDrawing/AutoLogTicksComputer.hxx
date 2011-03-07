/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * desc : Compute automatic ticks with logarithmic scaling  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _AUTO_LOG_TICKS_COMPUTER_HXX_
#define _AUTO_LOG_TICKS_COMPUTER_HXX_

#include "AutomaticTicksComputer.hxx"

namespace sciGraphics
{

class AutoLogTicksComputer: public AutomaticTicksComputer
{
public:

  AutoLogTicksComputer(DrawableSubwin * subwin);

  virtual ~AutoLogTicksComputer(void);

  /**
  * Get the number of ticks which will be computed by the strategy.
  */
  virtual int getNbTicks(void);

  /**
   * Fill positions parameters
   * with the ticks abscissas, ordinate, ...
   * If labels or labelsExponents is NULL, then only the positions are computed.
   */
  virtual void getTicksPosition(double positions[], char * labels[], char * labelsExponents[]);

  /**
  * To know if we need to get ticks exposants or not.
  */
  virtual bool isDisplayingLabelsExponents(void) {return true;}

protected:

};

}

#endif /* _AUTO_LOG_TICKS_COMPUTER_HXX_ */

