/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains the function to retrieve a the drawer of a graphic handle
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "getHandleDrawer.h"
#include "DrawableObjectFactory.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------------*/
DrawableObject * getHandleDrawerPointer( sciPointObj * pObj )
{
  return ((DrawableObjectWrapper *)(pObj->pDrawer))->drawer ;
}
/*---------------------------------------------------------------------------------*/
void setHandleDrawerPointer( sciPointObj * pObj, DrawableObject * drawer )
{
  if ( pObj->pDrawer == NULL )
  {
    pObj->pDrawer = new DrawableObjectWrapper() ;
  }
  ((DrawableObjectWrapper *)(pObj->pDrawer))->drawer = drawer ;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
DrawableSubwin * getSubwinDrawer( sciPointObj * pSubwin )
{
  return dynamic_cast<DrawableSubwin *>(getHandleDrawer(pSubwin));
}
/*---------------------------------------------------------------------------------*/
}
