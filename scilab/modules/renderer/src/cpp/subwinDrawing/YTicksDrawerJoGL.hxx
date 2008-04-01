/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks for Y axis  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Y_TICKS_DRAWER_JOGL_HXX_
#define _Y_TICKS_DRAWER_JOGL_HXX_

#include "TicksDrawerJoGL.hxx"
#include "TicksDrawerJavaMapper.hxx"

namespace sciGraphics
{

class YTicksDrawerJoGL: public TicksDrawerJoGL
{
public:

  YTicksDrawerJoGL(DrawableSubwin * subwin);

  virtual ~YTicksDrawerJoGL(void);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  TicksDrawerJavaMapper * getYTicksDrawerJavaMapper(void);

};

}

#endif /* _Y_TICKS_DRAWER_JOGL_HXX_ */

