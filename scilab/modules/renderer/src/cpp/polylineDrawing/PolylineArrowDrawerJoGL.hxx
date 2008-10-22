/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing arrow heads on a polyline object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _POLYLINE_ARROW_DRAWER_JOGL_HXX_
#define _POLYLINE_ARROW_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineArrowDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineArrowDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineArrowDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineArrowDrawerJoGL(void);

  /**
  * Main algorithm
  */
  virtual void drawPolyline( void );

  /**
  * Call the display list of an object
  */
  virtual void showPolyline( void );

  /**
   * Redefine redraw since arrow heads are drawn in pixels.
   */
  virtual void redrawPolyline( void ) ;

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  PolylineArrowDrawerJavaMapper * getArrowDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_ARROW_DRAWER_JOGL_HXX_ */

