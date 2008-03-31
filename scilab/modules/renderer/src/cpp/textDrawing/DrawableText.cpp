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
  initializeDrawing() ;
  // update might be needed
  updateTextBox();
  if (!checkVisibility() || isTextEmpty() )
  {
    endDrawing() ;
    return ;
  }
  
  clip();
  drawTextContent();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableText::show( void )
{
  initializeDrawing() ;
  // update might be needed
  updateTextBox();
  if (!checkVisibility() || isTextEmpty() )
  {
    endDrawing();
    return ;
  }
  initializeDrawing();
  clip();
  showTextContent();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/

}
