/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains the function to retrieve a the drawer of a graphic handle
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GET_HANDLE_DRAWER_H_
#define _GET_HANDLE_DRAWER_H_

extern "C"
{
#include "ObjectStructure.h"
}
#include "DrawableObject.h"
#include "subwinDrawing/DrawableSubwin.h"
#include "surfaceDrawing/DrawableSurface.h"
#include "segsDrawing/DrawableSegs.h"
#include "axesDrawing/DrawableAxes.h"

namespace sciGraphics
{

/**
 * Wrapping of the polymorph class drawable object.
 * It is not possible to use (void *) to cast polymorph class
 * so we need to wrap it.
 */
struct DrawableObjectWrapper
{
  DrawableObject * drawer ;
} ;

/*------------------------------------------------------------------------------*/

/**
 * Return the drawer of a graphic handle
 * If none exist it is automatically created using the drawable
 * object factory.
 */
DrawableObject * getHandleDrawer( sciPointObj * pObj ) ;

/**
 * Return the pointer on the drawer of a graphic handle
 * Direct access, no allocation is done.
 */
DrawableObject * getHandleDrawerPointer( sciPointObj * pObj ) ;

/**
 * Set the drawer of a sciPointObj.
 * Should not be used directly.
 */
void setHandleDrawerPointer( sciPointObj * pObj, DrawableObject * drawer) ;

/*------------------------------------------------------------------------------*/

/**
 * Return a DrawableSubwin object corresponding to a sciPointObj object.
 * @return the corresponding subwin to pSubwin if pSubwin if of type subwin, NULL otherwise
 */
DrawableSubwin * getSubwinDrawer( sciPointObj * pSubwin );

/**
 * Return a DrawableSurface object corresponding to a sciPointObj object.
 * @return the corresponding surface to pSurface if pSurface if of type surface, NULL otherwise
 */
DrawableSurface * getSurfaceDrawer( sciPointObj * pSurface );

/**
 * Return a DrawableSegs object corresponding to a sciPointObj object.
 * @return the corresponding segs to pSegs if pSegs if of type segs, NULL otherwise
 */
DrawableSegs * getSegsDrawer( sciPointObj * pSegs );

/**
 * Return a DrawableAxes object corresponding to a sciPointObj object.
 * @return the corresponding axes to pAxes if pAxes if of type axes, NULL otherwise
 */
DrawableAxes * getAxesDrawer( sciPointObj * pAxes );

}

#endif /* _GET_HANDLE_DRAWER_H_  */
