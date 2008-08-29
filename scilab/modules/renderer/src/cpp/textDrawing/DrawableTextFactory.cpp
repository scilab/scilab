/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Factory for text objects. Automaticaly create a text
 * drawer with the right algorithms from the garphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableTextFactory.h"
#include "ConcreteDrawableText.hxx"
#include "DrawableTextBridgeFactory.hxx"
#include "StandardTextDrawerJoGL.hxx"
#include "FilledTextDrawerJoGL.hxx"
#include "getHandleDrawer.h"
#include "CenteredTextDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableTextFactory::create( void )
{
  ConcreteDrawableText * newText = new ConcreteDrawableText( m_pDrawed ) ;
  DrawableTextBridgeFactory fact;
  fact.setDrawedText(newText);
  newText->setDrawableImp(fact.create());
  setStrategies(newText);

  return newText;
}
/*---------------------------------------------------------------------------------*/
void DrawableTextFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableText *>(getTextDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableTextFactory::setStrategies(ConcreteDrawableText * text)
{

  sciPointObj * pText = text->getDrawedObject();

  // set the text box strategy
  if ( sciGetAutoSize( pText ) )
  {
    if ( sciGetCenterPos( pText ) )
    {
      text->setTextDrawingStrategy(new CenteredTextDrawerJoGL(text));
    }
    else
    {
      text->setTextDrawingStrategy(new StandardTextDrawerJoGL(text));
    }
  }
  else
  {
    text->setTextDrawingStrategy(new FilledTextDrawerJoGL(text));
  }

}
/*---------------------------------------------------------------------------------*/

}
