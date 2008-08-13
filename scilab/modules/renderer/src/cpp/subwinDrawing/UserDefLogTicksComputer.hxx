/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Compute ticks from ticks given by the user in logarithmic mode
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _USER_DEF_LOG_TICKS_COMPUTER_HXX_
#define _USER_DEF_LOG_TICKS_COMPUTER_HXX_

#include "UserDefinedTicksComputer.hxx"

namespace sciGraphics
{

class UserDefLogTicksComputer: public UserDefinedTicksComputer
{
public:

  UserDefLogTicksComputer(DrawableSubwin * subwin);

  virtual ~UserDefLogTicksComputer(void);

  /**
  * Fill positions parameters
  * with the ticks abscissas, ordinate, ...
  */
  virtual void getTicksPosition(double positions[], char * labels[], char * labelsExponents[]);


protected:

};

}

#endif /* _USER_DEF_LOG_TICKS_COMPUTER_HXX_ */

