/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a 
 * segs object with JoGL  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SEGS_JOGL_HXX_
#define _DRAWABLE_SEGS_JOGL_HXX_


#include "DrawableSegsBridge.hxx"
#include "DrawableSegs.h"
#include "DrawableSegsJavaMapper.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableSegsJoGL : public DrawableSegsBridge, public DrawableClippedObjectJoGL
{

public:

  DrawableSegsJoGL( DrawableSegs * drawer ) ;

  virtual ~DrawableSegsJoGL( void ) ;

  /**
   * Return the drawn object
   */
  DrawableSegs * getSegsDrawer( void ) ;


protected:

  /**
   * Get the object performing mapping with Java class.
   */
  DrawableSegsJavaMapper * getSegsJavaMapper(void);

} ;

}

#endif /* _DRAWABLE_SEGS_JOGL_HXX_  */


