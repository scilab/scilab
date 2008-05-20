/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class Drawing the text content of a text object which is filled
 * inside a text box 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _FILLED_TEXT_DAWER_JOGL_HXX_
#define _FILLED_TEXT_DAWER_JOGL_HXX_

#include "DrawTextContentStrategy.hxx"
#include "TextContentDrawerJoGL.hxx"
#include "FilledTextDrawerJavaMapper.hxx"

namespace sciGraphics
{

class FilledTextDrawerJoGL : public TextContentDrawerJoGL
{
public:

  FilledTextDrawerJoGL(DrawableText * text);

  virtual ~FilledTextDrawerJoGL(void);

protected:

  virtual void setDrawerParameters(void);

  /**
   * Draw the rectangle surrounding the text. Redefined it to be able to retrieved actual font size.
   */
  virtual void drawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Draw the rectangle surrounding the text using precomputed data.
   * Redefined it to be able to retrieved actual font size.
   */
  virtual void redrawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Each time the object is drawn, its actual font sze may change, so update it.
   */
  void updateFontSize(void);

  /**
   * Get the object performing mapping with Java class.
   */
  FilledTextDrawerJavaMapper * getFilledTextDrawerJavaMapper(void);

};

}

#endif /* _FILLED_TEXT_DAWER_JOGL_HXX_ */

