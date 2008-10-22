/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy filling a polygon with interpolated colors
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _POLYLINE_INTERP_COLOR_DRAWER_JOGL_HXX_
#define _POLYLINE_INTERP_COLOR_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineInterpColorDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineInterpColorDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineInterpColorDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineInterpColorDrawerJoGL(void);

  /**
  * Main algorithm
  */
  virtual void drawPolyline( void );

  /**
  * Call the display list of an object
  */
  virtual void showPolyline( void );

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  PolylineInterpColorDrawerJavaMapper * getInterpColorDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_INTERP_COLOR_DRAWER_JOGL_HXX_ */

