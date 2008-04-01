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

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableText::draw( void )
{
  
  // update might be needed
  updateTextBox();
  if (!checkVisibility() || isTextEmpty() )
  {
    return ;
  }

  initializeDrawing() ;
  
  clip();
  drawBox();
  drawTextContent();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableText::show( void )
{
  // update might be needed
  updateTextBox();
  if (!checkVisibility() || isTextEmpty() )
  {
    return ;
  }
  initializeDrawing();
  clip();
  showBox();
  showTextContent();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/

}
