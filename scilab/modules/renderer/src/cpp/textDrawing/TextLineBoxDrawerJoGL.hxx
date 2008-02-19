/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing the surrounding box a text object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TEXT_LINE_BOX_DRAWER_JOGL_H_
#define _TEXT_LINE_BOX_DRAWER_JOGL_H_

#include "DrawTextBoxStrategy.hxx"
#include "../rectangleDrawing/RectangleLineDrawerJavaMapper.hxx"
#include "DrawableObjectJoGL.h"

namespace sciGraphics
{

class TextLineBoxDrawerJoGL : public DrawTextBoxStrategy, public DrawableObjectJoGL
{
public:

  TextLineBoxDrawerJoGL(DrawableText * text);

  virtual ~TextLineBoxDrawerJoGL(void);

  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawBox(void);

  /**
   * Display the text using display lists.
   */
  virtual void showBox(void);

protected:

  /**
   * Rectangle drawer do the job.
   */
  RectangleLineDrawerJavaMapper * getLineDrawerJavaMapper(void);
  
};

}

#endif /* _TEXT_LINE_BOX_DRAWER_JOGL_H_ */

