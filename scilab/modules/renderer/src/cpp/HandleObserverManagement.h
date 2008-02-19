/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Set of functions to add observers to a graphic handle 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _HANDLE_OBSERVER_MANAGEMENT_H_
#define _HANDLE_OBSERVER_MANAGEMENT_H_

extern "C"
{
#include "../../../graphics/includes/ObjectStructure.h"
}

#include "HandleObserver.h"

namespace sciGraphics
{

/**
 * Add a new observer to the graphic handle
 */
void attachObserver( sciPointObj * pObj, Observer * obs ) ;

/**
 * Remove an observer to the graphic handle
 */
void detachObserver( sciPointObj * pObj, Observer * obs ) ;

/**
 * Tell every observer attach to pObj to perform their action.
 */
void notifyAllObservers( sciPointObj * pObj ) ;

}

#endif /* _HANDLE_OBSERVER_MANAGEMENT_H_  */
