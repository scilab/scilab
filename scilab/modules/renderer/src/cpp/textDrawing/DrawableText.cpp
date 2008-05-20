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
   m_bBoundsUpdateRequested = false;
}
/*---------------------------------------------------------------------------------*/
DrawableText::~DrawableText( void )
{

}
/*---------------------------------------------------------------------------------*/
void DrawableText::updateTextBoxIfRequested(void)
{
  if(needBoundsUpdate()){
    updateTextBoxFromContext();
    m_bBoundsUpdateRequested = false;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableText::requestBoundsUpdate(void)
{
  m_bBoundsUpdateRequested = true;
}
/*---------------------------------------------------------------------------------*/
void DrawableText::draw( void )
{ 
  // update might be needed
  if (!checkVisibility() || isTextEmpty() )
  {
    updateTextBoxIfRequested();
    return ;
  }

  initializeDrawing() ;
  
  clip();
  drawTextContent();
  drawBox();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableText::show( void )
{
  // update might be needed
  if (!checkVisibility() || isTextEmpty() )
  {
    return ;
  }
  initializeDrawing();
  clip();
  showTextContent();
  showBox();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableText::redraw(void)
{
  // force redrawing
  // update might be needed
  if (!checkVisibility() || isTextEmpty() )
  {
    updateTextBoxIfRequested();
    return ;
  }
  initializeDrawing();
  clip();
  drawTextContent();

  // data bound may have changed so we need to redraw bounding box
  drawBox();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
bool DrawableText::needBoundsUpdate(void)
{
  return m_bBoundsUpdateRequested;
}
/*---------------------------------------------------------------------------------*/
#undef DEFAULT_FONT_SIZE

}
