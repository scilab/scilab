/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Functions to add and remove drawing observers from a graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _BUILD_DRAWING_OBSERVER_H_
#define _BUILD_DRAWING_OBSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"
  

  /**
   * Create a new drawing observer for the graphic handle
   */
  void createDrawingObserver( sciPointObj * pObj ) ;

  /**
   * Delete all observers watching a graphic object
   */
  void deleteObservers( sciPointObj * pObj ) ;

  /**
   * Update all the watching observers
   */
  void notifyObservers( sciPointObj * pObj ) ;

#ifdef __cplusplus
}
#endif

#endif /* _BUILD_DRAWING_OBSERVER_H_  */
