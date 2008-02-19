/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy filling the surrounding box a text object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TextFillBoxDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
TextFillBoxDrawerJoGL::TextFillBoxDrawerJoGL(DrawableText * text)
  : DrawTextBoxStrategy(text), DrawableObjectJoGL(text)
{
  setJavaMapper(new RectangleFillDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
TextFillBoxDrawerJoGL::~TextFillBoxDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void TextFillBoxDrawerJoGL::drawBox(void)
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  getFillDrawerJavaMapper()->setBackColor(sciGetGraphicContext(pObj)->backgroundcolor) ;

  // display the rectangle
  getFillDrawerJavaMapper()->drawRectangle(m_aCorners[0][0], m_aCorners[0][1], m_aCorners[0][2],
                                           m_aCorners[1][0], m_aCorners[1][1], m_aCorners[1][2],
                                           m_aCorners[2][0], m_aCorners[2][1], m_aCorners[2][2],
                                           m_aCorners[3][0], m_aCorners[3][1], m_aCorners[3][2]);

  endDrawing() ;

}
/*---------------------------------------------------------------------------------*/
void TextFillBoxDrawerJoGL::showBox(void)
{
  // we need to redraw the whole text object since
  // since it is not displayed in the same frame as other objects.
  drawBox();
}
/*---------------------------------------------------------------------------------*/
RectangleFillDrawerJavaMapper * TextFillBoxDrawerJoGL::getFillDrawerJavaMapper(void)
{
  return dynamic_cast<RectangleFillDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
