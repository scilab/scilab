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

#include <iostream>

#include "FilledTextDrawerJoGL.hxx"
#include "FilledTextDrawerJavaMapper.hxx"
#include "GetJavaProperty.h"

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
#include "StringMatrix.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
FilledTextDrawerJoGL::FilledTextDrawerJoGL(DrawableText * text)
  : TextContentDrawerJoGL(text)
{
  setJavaMapper(new FilledTextDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
FilledTextDrawerJoGL::~FilledTextDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJoGL::setDrawerParameters(void)
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();

  // get box size in user coordinates
  StringMatrix * textMatrix = sciGetText(pObj);
  getFilledTextDrawerJavaMapper()->setTextContent(getStrMatData(textMatrix), getMatNbRow(textMatrix), getMatNbCol(textMatrix));

  getFilledTextDrawerJavaMapper()->setTextParameters(sciGetAlignment(pObj), sciGetFontContext(pObj)->foregroundcolor,
                                                     sciGetFontStyle(pObj), m_pDrawed->getDefaultFontSize(),
                                                     sciGetFontOrientation(pObj),
                                                     sciGetIsUsingFractionalMetrics(pObj) == TRUE);

  // set box size
  double boxWidth;
  double boxHeight;
  getUserSizePix(boxWidth, boxHeight);
  getFilledTextDrawerJavaMapper()->setFilledBoxSize(boxWidth, boxHeight);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJoGL::drawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  TextContentDrawerJoGL::drawTextContent(corner1, corner2, corner3, corner4);
  updateFontSize();
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJoGL::redrawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  // box size may have changed so update it
  double boxWidth;
  double boxHeight;
  getUserSizePix(boxWidth, boxHeight);
  getFilledTextDrawerJavaMapper()->setFilledBoxSize(boxWidth, boxHeight);

  TextContentDrawerJoGL::redrawTextContent(corner1, corner2, corner3, corner4);
  updateFontSize();
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJoGL::updateFontSize(void)
{
  // store the font size to be able te retrieve it the next time we need.
  double newFontSize = getFilledTextDrawerJavaMapper()->getFontSize();
  m_pDrawed->setDefaultFontSize(newFontSize);
}
/*---------------------------------------------------------------------------------*/
FilledTextDrawerJavaMapper * FilledTextDrawerJoGL::getFilledTextDrawerJavaMapper(void)
{
  return dynamic_cast<FilledTextDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
