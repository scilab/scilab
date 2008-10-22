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

#ifndef _USER_DEF_LOG_SUBTICKS_COMPUTER_HXX_
#define _USER_DEF_LOG_SUBTICKS_COMPUTER_HXX_

#include "DrawableSubwin.h"
#include "UserDefinedSubticksComputer.hxx"

namespace sciGraphics
{

class UserDefLogSubticksComputer : public UserDefinedSubticksComputer
{
public:

  UserDefLogSubticksComputer(DrawableSubwin * subwin);

  virtual ~UserDefLogSubticksComputer(void);

  /**
   * Get the subticks positions.
   */
  virtual void getSubticksPosition(const double ticksPositions[], int nbTicks,
                                   double subTickspositions[]);

protected:


};

}

#endif /* _USER_DEF_LOG_SUBTICKS_COMPUTER_HXX_ */

