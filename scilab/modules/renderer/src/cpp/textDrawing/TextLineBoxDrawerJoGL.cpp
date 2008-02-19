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

#include "TextLineBoxDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
TextLineBoxDrawerJoGL::TextLineBoxDrawerJoGL(DrawableText * text)
  : DrawTextBoxStrategy(text), DrawableObjectJoGL(text)
{
  setJavaMapper(new RectangleLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
TextLineBoxDrawerJoGL::~TextLineBoxDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void TextLineBoxDrawerJoGL::drawBox(void)
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  getLineDrawerJavaMapper()->setLineParameters(sciGetGraphicContext(pObj)->foregroundcolor,
                                               (float)sciGetLineWidth(pObj),
                                               sciGetLineStyle(pObj));

  // display the rectangle
  getLineDrawerJavaMapper()->drawRectangle(m_aCorners[0][0], m_aCorners[0][1], m_aCorners[0][2],
                                           m_aCorners[1][0], m_aCorners[1][1], m_aCorners[1][2],
                                           m_aCorners[2][0], m_aCorners[2][1], m_aCorners[2][2],
                                           m_aCorners[3][0], m_aCorners[3][1], m_aCorners[3][2]);

  endDrawing() ;

}
/*---------------------------------------------------------------------------------*/
void TextLineBoxDrawerJoGL::showBox(void)
{
  drawBox();
}
/*---------------------------------------------------------------------------------*/
RectangleLineDrawerJavaMapper * TextLineBoxDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<RectangleLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
