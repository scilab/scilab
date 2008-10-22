/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a surface object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "DrawableSurface.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSurface::draw( void )
{
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  reinitMove();
  drawSurface();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSurface::show( void )
{
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  translate();
  showSurface();
  endTranslate();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSurface::redraw(void)
{
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  translate();
  redrawSurface();
  endTranslate();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
}
