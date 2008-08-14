/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class Drawing the text content of a text object with centered 
 * text box mode option
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CenteredTextDrawerJoGL.hxx"
#include "CenteredTextDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
#include "StringMatrix.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
CenteredTextDrawerJoGL::CenteredTextDrawerJoGL(DrawableText * text)
  : TextContentDrawerJoGL(text)
{
  setJavaMapper(new CenteredTextDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
CenteredTextDrawerJoGL::~CenteredTextDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJoGL::setDrawerParameters(void)
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();

  getCenteredTextDrawerJavaMapper()->setTextParameters(sciGetAlignment(pObj), sciGetFontContext(pObj)->foregroundcolor,
                                                       sciGetFontStyle(pObj), sciGetFontSize(pObj), sciGetFontOrientation(pObj),
                                                       sciGetIsUsingFractionalMetrics(pObj) == TRUE);

  StringMatrix * textMatrix = sciGetText(pObj);
  getCenteredTextDrawerJavaMapper()->setTextContent(getStrMatData(textMatrix), getMatNbRow(textMatrix), getMatNbCol(textMatrix));

  // set box size
  double boxWidth;
  double boxHeight;
  getUserSizePix(boxWidth, boxHeight);
  getCenteredTextDrawerJavaMapper()->setFilledBoxSize(boxWidth, boxHeight);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJoGL::redrawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  // box size may have changed so update it
  double boxWidth;
  double boxHeight;
  getUserSizePix(boxWidth, boxHeight);
  getCenteredTextDrawerJavaMapper()->setFilledBoxSize(boxWidth, boxHeight);

  TextContentDrawerJoGL::redrawTextContent(corner1, corner2, corner3, corner4);
}
/*---------------------------------------------------------------------------------*/
CenteredTextDrawerJavaMapper * CenteredTextDrawerJoGL::getCenteredTextDrawerJavaMapper(void)
{
  return dynamic_cast<CenteredTextDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
