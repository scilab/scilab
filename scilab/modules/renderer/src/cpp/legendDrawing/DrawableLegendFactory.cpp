/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for legend objects. Automatically create a legend 
 * drawer with the right algorithms from the graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableLegendFactory.h"
#include "ConcreteDrawableLegend.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableLegendFactory::create( void )
{
  return new ConcreteDrawableLegend( m_pDrawed ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableLegendFactory::update( void )
{
  // nothing for now
}
/*---------------------------------------------------------------------------------*/

}
