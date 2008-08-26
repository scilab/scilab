/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class drawing only the backTrihedron of a subwin object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _BACK_TRIHEDRON_DRAWER_JOGL_HXX_
#define _BACK_TRIHEDRON_DRAWER_JOGL_HXX_

#include "DrawAxesBoxStrategy.hxx"
#include "../DrawableObjectJoGL.h"
#include "BackTrihedronDrawerJavaMapper.hxx"

namespace sciGraphics
{

class BackTrihedronDrawerJoGL: public DrawAxesBoxStrategy, public DrawableObjectJoGL
{
public:
  
   BackTrihedronDrawerJoGL(DrawableSubwin * subwin);

   virtual ~BackTrihedronDrawerJoGL(void);

  /**
   * Main funtion which actually draw the box.
   * @param concealedCornerIndex index of the concealed corner
   */
  virtual void drawAxesBox(int concealedCornerIndex);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  BackTrihedronDrawerJavaMapper * getBackTrihedronDrawerJavaMapper(void);

};

}

#endif /* _BACK_TRIHEDRON_DRAWER_JOGL_HXX_ */

