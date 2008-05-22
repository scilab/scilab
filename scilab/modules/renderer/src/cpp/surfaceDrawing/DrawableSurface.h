/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a surface object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SURFACE_H_
#define _DRAWABLE_SURFACE_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableSurface : public DrawableClippedObject
{

public:

  DrawableSurface( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Draw the surface using precomputed data
   */
  virtual EDisplayStatus redraw(void);



  /**
   * Actually draw the surface on the screen
   */
  virtual void drawSurface(void) = 0;

  /**
   * Actually show the stored data.
   */
  virtual void showSurface(void) = 0;

  /**
   * Actually redraw the surface stored data
   */
  virtual void redrawSurface(void) = 0;


} ;

}

#endif /* _DRAWABLE_SURFACE_H_  */
