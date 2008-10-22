/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw a 
 * text object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_TEXT_JOGL_HXX_
#define _DRAWABLE_TEXT_JOGL_HXX_

#include "DrawableTextBridge.hxx"
#include "DrawableText.h"
#include "DrawableTextJavaMapper.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableTextJoGL : public DrawableTextBridge, public DrawableClippedObjectJoGL
{

public:
  DrawableTextJoGL( DrawableText * drawer ) ;

  virtual ~DrawableTextJoGL( void ) ;

  /**
   * Return the drawn object
   */
  DrawableText * getTextDrawer( void ) ;

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  DrawableTextJavaMapper * getTextJavaMapper(void);

};

}

#endif /* _DRAWABLE_TEXT_JOGL_HXX_ */

