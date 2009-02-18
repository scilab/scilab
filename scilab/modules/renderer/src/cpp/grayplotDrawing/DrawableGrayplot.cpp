/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a grayplot object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableGrayplot.h"


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableGrayplot::draw( void )
{
  if(!checkVisibility())
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  reinitMove();
  EDisplayStatus status = drawGrayplot();
  unClip();
  endDrawing();
  return status;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableGrayplot::show( void )
{
  if(!checkVisibility())
  {
    return UNCHANGED;
  }
  clip();
  translate();
  showGrayplot();
  endTranslate();
  unClip();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplotBridge * DrawableGrayplot::getGrayplotImp( void )
{
  return dynamic_cast<DrawableGrayplotBridge *>(getDrawableImp());
}
/*---------------------------------------------------------------------------------*/
}
