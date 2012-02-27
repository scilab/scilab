/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for drawable objects. Automatically create a drawable
 * object with the right algorithms from the graphic handle
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "GetProperty.h"
}

#include "DrawableObjectFactory.h"
#include "subwinDrawing/DrawableSubwinFactory.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableObjectFactory::setGraphicObj( sciPointObj * drawedObj )
{
  m_pDrawed = drawedObj ;
}
/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableObjectFactory::create( void )
{
  DrawableObjectFactory * fact = createRightFactory() ;

  fact->setGraphicObj( m_pDrawed ) ;

  DrawableObject * drawer = fact->create() ;

  delete fact ;

  return drawer ;

}
/*---------------------------------------------------------------------------------*/
void DrawableObjectFactory::update( void )
{
  DrawableObjectFactory * fact = createRightFactory() ;

  fact->setGraphicObj( m_pDrawed ) ;

  fact->update() ;

  delete fact ;

}
/*---------------------------------------------------------------------------------*/
DrawableObjectFactory * DrawableObjectFactory::createRightFactory( void )
{
  switch ( sciGetEntityType(m_pDrawed) )
  {
  case SCI_FIGURE:
    return NULL;
    break ;
  case SCI_SUBWIN:
    return new DrawableSubwinFactory() ;
    break;
  case SCI_UIMENU:
  default:
    return NULL;
  }
}
/*---------------------------------------------------------------------------------*/
}
