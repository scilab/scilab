/*------------------------------------------------------------------------*/
/* file: DrawingBridge.cpp                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions to create and access a drawer of graphic handles      */
/*------------------------------------------------------------------------*/

#include "DrawingBridge.h"
#include "getHandleDrawer.h"
#include "DrawableObjectFactory.h"

extern "C"
{
#include "GetProperty.h"
}

using namespace sciGraphics ;

/*------------------------------------------------------------------------------------------*/
void destroyHandleDrawer( sciPointObj * pObj )
{
  /* don't call getHandleDrawer for the test, otherwise a drawer will be created */
  if ( pObj->pDrawer != NULL )
  {
    delete getHandleDrawer( pObj ) ;
    delete (DrawableObjectWrapper *)pObj->pDrawer ;
    pObj->pDrawer = NULL ;
  }
}
/*------------------------------------------------------------------------------------------*/
void sciDrawObj( sciPointObj * pObj )
{
  /* force total redraw */
  /* we need to recreate a new one */
  DrawableObjectFactory updater ;
  updater.setGraphicObj( pObj ) ;
  updater.update() ;
  getHandleDrawer(pObj)->hasChanged() ;
  /* redisplay everything, including this handle */
  getHandleDrawer( sciGetParentFigure( pObj ) )->display() ;
}
/*------------------------------------------------------------------------------------------*/
void displayChildren( sciPointObj * pObj )
{
  getHandleDrawer( pObj )->displayChildren() ;
}
/*------------------------------------------------------------------------------------------*/
void redrawHierarchy( sciPointObj * pObj )
{
  getHandleDrawer(pObj)->familyHasChanged();
  getHandleDrawer(pObj)->display();
}
/*------------------------------------------------------------------------------------------*/