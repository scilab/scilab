/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class drawing the full bounding box of a subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _FULL_BOX_DRAWER_JOGL_HXX_
#define _FULL_BOX_DRAWER_JOGL_HXX_

#include "DrawAxesBoxStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "FullBoxDrawerJavaMapper.hxx"

#ifdef _MSC_VER
#pragma warning(once : 4250)
#endif


namespace sciGraphics
{

class FullBoxDrawerJoGL: public DrawAxesBoxStrategy, public DrawableObjectJoGL
{
public:

  FullBoxDrawerJoGL(DrawableSubwin * subwin);

  virtual ~FullBoxDrawerJoGL(void);

  /**
   * Main funtion which actually draw the box.
   * @param concealedCornerIndex index of the concealed corner
   */
  virtual void drawAxesBox(int concealedCornerIndex);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  FullBoxDrawerJavaMapper * getFullBoxDrawerJavaMapper(void);

};

}

#endif /* _FULL_BOX_DRAWER_JOGL_HXX_ */

