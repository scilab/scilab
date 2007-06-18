/*------------------------------------------------------------------------*/
/* file: BuildDrawingObserver.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions to add and remove drawing observers from a graphic    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#ifndef _BUILD_DRAWING_OBSERVER_H_
#define _BUILD_DRAWING_OBSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../graphics/includes/ObjectStructure.h"
  

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
