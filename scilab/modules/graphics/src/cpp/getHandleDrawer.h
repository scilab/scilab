/*------------------------------------------------------------------------*/
/* file: getHandleDrawer.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains the function to retrieve a the drawer of a graphic     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#ifndef _GET_HANDLE_DRAWER_H_
#define _GET_HANDLE_DRAWER_H_

extern "C"
{
#include "ObjectStructure.h"
}
#include "DrawableObject.h"


/**
 * Wrapping of the polymorph class drawable object.
 * It is not possible to use (void *) to cast polymorph class
 * so we need to wrap it.
 */
struct DrawableObjectWrapper
{
  DrawableObject * drawer ;
} ;

/**
 * Return the drawer of a graphic handle
 * If none exist it is automaticaly created using the drawable
 * object factory.
 */
DrawableObject * getHandleDrawer( sciPointObj * pObj ) ;

/**
 * Return the pointer on the drawer of a graphic handle
 * Direct access, no allocation is done.
 */
DrawableObject * getHandleDrawerPointer( sciPointObj * pObj ) ;

/**
 * Set the drawer of a sciPointObj.
 * Should not be used directly.
 */
void setHandleDrawerPointer( sciPointObj * pObj, DrawableObject * drawer) ;



#endif /* _GET_HANDLE_DRAWER_H_  */
