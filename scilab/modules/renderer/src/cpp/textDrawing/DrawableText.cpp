/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a text object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableText.h"
#include "../DrawableObject.h"
#include "../getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

#define DEFAULT_FONT_SIZE 1.0

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableText::DrawableText( sciPointObj * pObj ) : DrawableClippedObject( pObj )
{
  m_dDefaultFontSize = DEFAULT_FONT_SIZE;
}
/*---------------------------------------------------------------------------------*/
DrawableText::~DrawableText( void )
{

}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableText::draw( void )
{ 
  // update might be needed
  if (!checkVisibility() || isTextEmpty() )
  {
    return UNCHANGED;
  }

  initializeDrawing() ;
  
  clip();
  reinitMove();
  drawTextContent();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableText::show( void )
{
  // update might be needed
  if (!checkVisibility() || isTextEmpty() )
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  translate();
  showTextContent();
  endTranslate();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableText::redraw(void)
{
  // force redrawing
  // update might be needed
  if (!checkVisibility() || isTextEmpty() )
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  reinitMove();
  drawTextContent();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
#undef DEFAULT_FONT_SIZE

}
