/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing grid using JoGL for Y axis
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Y_GRID_DRAWER_JOGL_HXX_
#define _Y_GRID_DRAWER_JOGL_HXX_

#include "DrawableObjectJoGL.h"
#include "GridDrawerJoGL.hxx"

namespace sciGraphics
{

class YGridDrawerJoGL : public GridDrawerJoGL
{
public:

  YGridDrawerJoGL(DrawableSubwin * subwin);

  virtual ~YGridDrawerJoGL(void);

protected:

  /**
   * Retrieve the style of the grid to draw
   */
  virtual int getGridStyle(void);
};

}

#endif /* _Y_GRID_DRAWER_JOGL_HXX_ */

