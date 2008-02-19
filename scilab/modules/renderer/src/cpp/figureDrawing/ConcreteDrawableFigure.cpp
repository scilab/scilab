/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver independent algorithms to draw a figure handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableFigure.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "pixel_mode.h"
#include "SetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ConcreteDrawableFigure::ConcreteDrawableFigure( sciPointObj * pObj )
    : DrawableFigure( pObj )
{
  m_pSingleObject = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableFigure::~ConcreteDrawableFigure(void)
{
  m_pSingleObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFigure::drawSingleObj(sciPointObj * pObj)
{
  // set the single object to draw
  m_pSingleObject = pObj;


  int curPixelMode = sciGetXorMode(m_pDrawed);

  // launch display
  // displaySingleObject will be called
  display();

  sciSetXorMode(m_pDrawed, curPixelMode);

  // restore normal mode
  m_pSingleObject = NULL;
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::checkAutoRedraw( void )
{
  return ( sciGetIsAutoDrawable(m_pDrawed) == TRUE ) ;
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::isDisplayingSingleObject(void)
{
  if (m_pSingleObject == NULL)
  {
    return false;
  }

  if (sciGetEntityType(m_pSingleObject) == SCI_FIGURE)
  {
    // should be itself so it is like a normal draw
    return false;
  }
    
  return true;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFigure::displaySingleObject(void)
{
  // tell the subwin to draw only this object
  getSubwinDrawer(sciGetParentSubwin(m_pSingleObject))->displaySingleObj(m_pSingleObject);
}
/*---------------------------------------------------------------------------------*/

}
