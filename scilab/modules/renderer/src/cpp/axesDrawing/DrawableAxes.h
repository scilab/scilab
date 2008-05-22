/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for an axes object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_AXES_H_
#define _DRAWABLE_AXES_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableAxes : public DrawableClippedObject
{

public:

  DrawableAxes( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

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
   * We need to rebuild everything
   */
  virtual EDisplayStatus redraw( void ) { return draw(); }

  /**
   * Actually draw the axes object
   */
  virtual void drawAxes(void) = 0;

  /**
   * Actually show the axes object
   */
  virtual void showAxes(void) = 0;


} ;

}

#endif /* _DRAWABLE_AXES_H_  */
