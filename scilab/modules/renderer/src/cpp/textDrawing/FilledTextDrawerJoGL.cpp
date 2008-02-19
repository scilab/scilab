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

  // get box size in user coordinates
  double boxWidth;
  double boxHeight;
  sciGetUserSize(pObj, &boxWidth, &boxHeight);

  double textPos[3];
  sciGetTextPos(pObj, textPos);

  // convert the user lengths to pixel ones.
  int pixWidth;
  int pixHeight;
  getPixelLength(sciGetParentSubwin(pObj), textPos, boxWidth, boxHeight, &pixWidth, &pixHeight);

  getFilledTextDrawerJavaMapper()->setTextParameters(sciGetAlignment(pObj), sciGetFontContext(pObj)->foregroundcolor,
                                                     sciGetFontStyle(pObj), sciGetFontOrientation(pObj),
                                                     pixWidth, pixHeight);

  getTextDisplayPos(textPos);
  getFilledTextDrawerJavaMapper()->setCenterPosition(textPos[0], textPos[1], textPos[2]);
}
/*---------------------------------------------------------------------------------*/
FilledTextDrawerJavaMapper * FilledTextDrawerJoGL::getFilledTextDrawerJavaMapper(void)
{
  return dynamic_cast<FilledTextDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
