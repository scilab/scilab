/*------------------------------------------------------------------------*/
/* file: DrawingBridge.cpp                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions to create and access a drawer of graphic handles      */
/*------------------------------------------------------------------------*/

#include "DrawingBridge.h"
#include "getHandleDrawer.h"

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
  getHandleDrawer(pObj)->draw() ;
}
/*------------------------------------------------------------------------------------------*/
