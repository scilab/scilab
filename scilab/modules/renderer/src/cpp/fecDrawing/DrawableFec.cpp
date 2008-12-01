/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a fec object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFec.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableFec::draw( void )
{
  if (!checkVisibility())
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  reinitMove();
  EDisplayStatus status = drawFec();
  unClip();
  endDrawing();
  return status;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableFec::show( void )
{
  if (!checkVisibility())
  {
    return UNCHANGED;
  }
  clip();
  translate();
  showFec();
  endTranslate();
  unClip();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/

}
