/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to create and access a drawer of graphic handles
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
  /* Update everything */
  getHandleDrawer(pObj)->familyHasChanged();

  /* redisplay everything, including this handle */
  getHandleDrawer( sciGetParentFigure( pObj ) )->display() ;
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
