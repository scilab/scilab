/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for compound objects. Automatically create a compound
 * drawer with the right algorithms from the graphic handle 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableCompoundFactory.h"
#include "DrawableCompound.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableCompoundFactory::create( void )
{
  return new DrawableCompound( m_pDrawed ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableCompoundFactory::update( void )
{
  // nothing for now
}
/*---------------------------------------------------------------------------------*/

}
