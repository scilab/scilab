/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SUBWIN_BACKGROUND_DRAWER_JOGL_HXX_
#define _SUBWIN_BACKGROUND_DRAWER_JOGL_HXX_

#include "DrawAxesBoxStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "SubwinBackgroundDrawerJavaMapper.hxx"

#ifdef _MSC_VER
#pragma warning(once : 4250)
#endif


namespace sciGraphics
{

class SubwinBackgroundDrawerJoGL: public DrawAxesBoxStrategy, public DrawableObjectJoGL
{
public:
  
   SubwinBackgroundDrawerJoGL(DrawableSubwin * subwin);

   virtual ~SubwinBackgroundDrawerJoGL(void);

  /**
   * Main funtion which actually draw the box.
   * @param concealedCornerIndex index of the concealed corner
   */
  virtual void drawAxesBox(int concealedCornerIndex);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  SubwinBackgroundDrawerJavaMapper * getSubwinBackgroundDrawerJavaMapper(void);

};

}

#endif /* _SUBWIN_BACKGROUND_DRAWER_JOGL_HXX_ */

