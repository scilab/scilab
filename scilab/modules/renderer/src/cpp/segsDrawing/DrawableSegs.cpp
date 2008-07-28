/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a segs object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSegs.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSegs::draw( void )
{
  if(!checkVisibility())
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  reinitMove();
  EDisplayStatus status = drawSegs();
  unClip();
  endDrawing();
  return status;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSegs::show( void )
{
  if(!checkVisibility())
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  translate();
  showSegs();
  endTranslate();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSegs::redraw(void)
{
  if(!checkVisibility())
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  translate();
  redrawSegs();
  endTranslate();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/

}
