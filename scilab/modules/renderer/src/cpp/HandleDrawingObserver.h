/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Observer to automatically draw graphic handles
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _HANDLE_DRAWING_OBSERVER_H_
#define _HANDLE_DRAWING_OBSERVER_H_

#include "HandleObserver.h"

namespace sciGraphics
{

/**
 * Class observing handles (sciPointObj *).
 */
class HandleDrawingObserver : public HandleObserver
{
public:

  HandleDrawingObserver( sciPointObj * subject ) : HandleObserver( subject ) {}

  /**
   * Here the update action is to redraw the graphic handle
   */
  void update( void ) ;
  
} ;

}

#endif /* _HANDLE_DRAWING_OBSERVER_H_ */
