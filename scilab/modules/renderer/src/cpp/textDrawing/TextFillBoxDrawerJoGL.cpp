/*------------------------------------------------------------------------*/
/* file: TextFillBoxDrawerJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy filling the surrounding box a text object              */
/*------------------------------------------------------------------------*/

#include "TextFillBoxDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
TextFillBoxDrawerJoGL::TextFillBoxDrawerJoGL(DrawableText * text)
  : DrawTextBoxStrategy(text), DrawableObjectJoGL(text)
{
  setJavaMapper(new RectangleFillDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
TextFillBoxDrawerJoGL::~TextFillBoxDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------*/
void TextFillBoxDrawerJoGL::showBox(void)
{
  // we need to redraw the whole text object since
  // since it is not displayed in the same frame as other objects.
  drawBox();
}
/*------------------------------------------------------------------------------------------*/
RectangleFillDrawerJavaMapper * TextFillBoxDrawerJoGL::getFillDrawerJavaMapper(void)
{
  return dynamic_cast<RectangleFillDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
