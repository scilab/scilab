/*------------------------------------------------------------------------*/
/* file: getHandleDrawer.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains the function to retrieve a the drawer of a graphic     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleDrawer.h"
#include "DrawableObjectFactory.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * getHandleDrawer( sciPointObj * pObj )
{
  if ( pObj->pDrawer != NULL )
  {
    return getHandleDrawerPointer( pObj ) ;
  }
  
  /* We need to create the drawer */

  DrawableObjectFactory creator ;
  creator.setGraphicObj( pObj ) ;

  DrawableObject * drawer = creator.create() ;

  setHandleDrawerPointer( pObj, drawer ) ;

  return drawer ;
}
/*------------------------------------------------------------------------------------------*/
DrawableObject * getHandleDrawerPointer( sciPointObj * pObj )
{
  if ( pObj->pDrawer == NULL ) { return NULL ; }
  return ((DrawableObjectWrapper *)(pObj->pDrawer))->drawer ;
}
/*------------------------------------------------------------------------------------------*/
void setHandleDrawerPointer( sciPointObj * pObj, DrawableObject * drawer )
{
  if ( pObj->pDrawer == NULL )
  {
    pObj->pDrawer = new DrawableObjectWrapper ;
  }
  ((DrawableObjectWrapper *)(pObj->pDrawer))->drawer = drawer ;
}
/*------------------------------------------------------------------------------------------*/

}
