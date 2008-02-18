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
#include "../../../graphics/includes/GetProperty.h"
}

using namespace sciGraphics ;

/*---------------------------------------------------------------------------------*/
void createDrawer( sciPointObj * pObj )
{
  getHandleDrawer( pObj ) ;
}
/*---------------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------------*/
void sciDrawObj( sciPointObj * pObj )
{
  /* force total redraw */
  /* we need to recreate a new one */
  getHandleDrawer(pObj)->hasChanged() ;
  /* redisplay everything, including this handle */
  getHandleDrawer( sciGetParentFigure( pObj ) )->display() ;
}
/*---------------------------------------------------------------------------------*/
void sciDrawSingleObj( sciPointObj * pObj )
{
  /* force a redraw of the object */
  getHandleDrawer(pObj)->hasChanged();
  getFigureDrawer(sciGetParentFigure(pObj))->drawSingleObj(pObj);
}
/*---------------------------------------------------------------------------------*/
void displayChildren( sciPointObj * pObj )
{
  getHandleDrawer( pObj )->displayChildren() ;
}
/*---------------------------------------------------------------------------------*/
void redrawHierarchy( sciPointObj * pObj )
{
  getHandleDrawer(pObj)->familyHasChanged();
  if ( sciGetEntityType(pObj) == SCI_FIGURE)
  {
    // don't use display here or we will call Java display again.
    getFigureDrawer(pObj)->drawInContext();
  }
  else
  {
    getHandleDrawer(pObj)->display();
  }
}
/*---------------------------------------------------------------------------------*/
void forceHierarchyRedraw( sciPointObj * pObj )
{
  getHandleDrawer(pObj)->familyHasChanged();
}
/*---------------------------------------------------------------------------------*/
void forceRedraw(sciPointObj * pObj)
{
  getHandleDrawer(pObj)->hasChanged();
}
/*---------------------------------------------------------------------------------*/
