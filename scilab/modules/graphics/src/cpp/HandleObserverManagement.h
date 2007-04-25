/*------------------------------------------------------------------------*/
/* file: HandleObserverManagement.h                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to add observers to a graphic handle           */
/*------------------------------------------------------------------------*/

#ifndef _HANDLE_OBSERVER_MANAGEMENT_H_
#define _HANDLE_OBSERVER_MANAGEMENT_H_

extern "C"
{
#include "ObjectStructure.h"
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
