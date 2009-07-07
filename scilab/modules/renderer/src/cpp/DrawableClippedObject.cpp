/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class which represent an object which can be clipped
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableClippedObject.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableClippedObject::DrawableClippedObject( sciPointObj * drawed )
  : DrawableObject( drawed )
{
  m_bXClippingEnable = false ;
  m_bYClippingEnable = false ;
  m_bZClippingEnable = false ;
}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObject::setClipBox( void )
{
  // compute the real clipping of the object
  sciPointObj * pObj = getDrawedObject() ;

  switch(sciGetIsClipping(pObj))
  {
  case 0: // clipgrf state, use axes bounds
    {
      double clipBounds[6] ;
      sciGetRealDataBounds(sciGetParentSubwin(pObj), clipBounds) ;
      setClipBox(clipBounds[0], clipBounds[1], clipBounds[2],
                 clipBounds[3], clipBounds[4], clipBounds[5] ) ;
      m_bXClippingEnable = true ;
      m_bYClippingEnable = true ;
      m_bZClippingEnable = true ;
    }
    break;
  case 1: // on state, use its own bounds
    {
      double * clipBounds = sciGetClipping(pObj) ; // array (x,y,w,h)
			double scaledClipBounds[6];
			// xmin, ymin
			pointScale(clipBounds[0], clipBounds[1] - clipBounds[3], 0.0,
							   &(scaledClipBounds[0]), &(scaledClipBounds[2]), &(scaledClipBounds[4]));
			// xmax, ymax
			pointScale(clipBounds[0] + clipBounds[2], clipBounds[1], 0.0,
							   &(scaledClipBounds[1]), &(scaledClipBounds[3]), &(scaledClipBounds[5]));
      // beware in scilab y is upper left point
      setClipBox( scaledClipBounds[0], scaledClipBounds[1],
                  scaledClipBounds[2], scaledClipBounds[3],
                  scaledClipBounds[4], scaledClipBounds[5] ) ;
      m_bXClippingEnable = true ;
      m_bYClippingEnable = true ;
      m_bZClippingEnable = false ;
    }
    break;
  default: // clipping off
    m_bXClippingEnable = false ;
    m_bYClippingEnable = false ;
    m_bZClippingEnable = false ;
    return;
  }

}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObject::setClipBox( double xMin, double xMax,
                                        double yMin, double yMax,
                                        double zMin, double zMax )
{
  getClippedObjBridge()->setClipBox(xMin, xMax, yMin, yMax, zMin, zMax) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObject::clip( void )
{
  setClipBox();
  if ( m_bXClippingEnable )
  {
    getClippedObjBridge()->clipX();
  }

  if ( m_bYClippingEnable )
  {
    getClippedObjBridge()->clipY();
  }

  if ( m_bZClippingEnable )
  {
    getClippedObjBridge()->clipZ();
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObject::unClip( void )
{
	if (m_bXClippingEnable || m_bYClippingEnable || m_bZClippingEnable)
	{
		getClippedObjBridge()->unClip();
	}
}
/*---------------------------------------------------------------------------------*/
DrawableClippedObjectBridge * DrawableClippedObject::getClippedObjBridge( void )
{
  return dynamic_cast<DrawableClippedObjectBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/

}
