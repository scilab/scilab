/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class Drawing the text content of a text object with no text box option
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _STANDARD_TEXT_DAWER_JOGL_HXX_
#define _STANDARD_TEXT_DAWER_JOGL_HXX_

#include "DrawTextContentStrategy.hxx"
#include "TextContentDrawerJoGL.hxx"
#include "StandardTextDrawerJavaMapper.hxx"

namespace sciGraphics
{

class StandardTextDrawerJoGL : public TextContentDrawerJoGL
{
public:

  StandardTextDrawerJoGL(DrawableText * text);

  virtual ~StandardTextDrawerJoGL(void);

protected:

  virtual void setDrawerParameters(void);

  /**
   * Get the object performing mapping with Java class.
   */
  StandardTextDrawerJavaMapper * getStandardTextDrawerJavaMapper(void);

};

}

#endif /* _STANDARD_TEXT_DAWER_JOGL_HXX_ */

