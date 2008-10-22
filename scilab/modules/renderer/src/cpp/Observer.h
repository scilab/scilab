/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : interface for observers   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _OBSERVER_H_
#define _OBSERVER_H_

namespace sciGraphics
{

/**
 * Interface for every observer objects
 */
class Observer
{
public:

  virtual ~Observer( void ) {}

  /**
   * Function called when the subject call notify
   */
  virtual void update( void ) = 0 ;

} ;

}

#endif /* _OBSERVER_H_ */

