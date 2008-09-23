/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw a
 * fec object with JoGL  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_FEC_JOGL_HXX_
#define _DRAWABLE_FEC_JOGL_HXX_

#include "DrawableFecBridge.hxx"
#include "DrawableFec.h"
#include "DrawableFecJavaMapper.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableFecJoGL : public DrawableFecBridge, public DrawableClippedObjectJoGL
{

public:
  DrawableFecJoGL( DrawableFec * drawer ) ;

  virtual ~DrawableFecJoGL( void ) ;

  /**
  * Return the drawn object
  */
  DrawableFec * getFecDrawer( void ) ;

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  DrawableFecJavaMapper * getFecJavaMapper(void);

};

}

#endif /* _DRAWABLE_FEC_JOGL_HXX_ */

