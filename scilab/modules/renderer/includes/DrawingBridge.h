/*------------------------------------------------------------------------*/
/* file: DrawingBridge.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions to create and access a drawer of graphic handles      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWING_BRIDGE_H_
#define _DRAWING_BRIDGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"

  /**
   * Create a render for this object
   */
  void createDrawer( sciPointObj * pObj ) ;

  /**
   * Delete the drawer of a graphic handle
   */
  void destroyHandleDrawer( sciPointObj * pObj ) ;

  /**
   * Draw a graphic handle.
   * Should normally not be called directly. Observers should call it automatically.
   */
  void sciDrawObj( sciPointObj * pObj ) ;

  /**
  * Draw a single graphic handle.
  * Should normally not be called directly. Observers should call it automatically.
  */
  void sciDrawSingleObj( sciPointObj * pObj ) ;

  /**
   * Redraw an object and all its children.
   * To be called on a figure to redraw a whole window
   */
  void redrawHierarchy( sciPointObj * pObj ) ;

  /**
   * Call display function on the children of an object
   */
  void displayChildren( sciPointObj * pObj ) ;

  /**
   * Next call to display will force this object and its children to be drawn.
   */
  void forceHierarchyRedraw( sciPointObj * pObj ) ;

  /**
   * Specify that an object has been modifeied and need to be redrawn.
   * To be used when several objects are modified in the same time.
   */
  void forceRedraw(sciPointObj * pObj);

#ifdef __cplusplus
}
#endif

#endif /* _DRAWING_BRIDGE_H_  */
