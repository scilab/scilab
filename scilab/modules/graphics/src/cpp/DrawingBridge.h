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
   * Delete the drawer of a graphic handle
   */
  void destroyHandleDrawer( sciPointObj * pObj ) ;

  /**
   * Draw a graphic handle.
   * Should normally not be called directly. Observers should call it automatically.
   */
  void sciDrawObj( sciPointObj * pObj ) ;

#ifdef __cplusplus
}
#endif

#endif /* _DRAWING_BRIDGE_H_  */
