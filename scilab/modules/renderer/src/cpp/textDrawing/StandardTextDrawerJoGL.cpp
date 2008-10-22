/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class Drawing the text content of a text object with left
 * alignement
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "StandardTextDrawerJoGL.hxx"
#include "StandardTextDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
#include "StringMatrix.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
StandardTextDrawerJoGL::StandardTextDrawerJoGL(DrawableText * text)
  : TextContentDrawerJoGL(text)
{
  setJavaMapper(new StandardTextDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
StandardTextDrawerJoGL::~StandardTextDrawerJoGL(void)
{
  
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJoGL::setDrawerParameters(void)
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  getStandardTextDrawerJavaMapper()->setTextParameters(sciGetAlignment(pObj), sciGetFontContext(pObj)->foregroundcolor,
                                                       sciGetFontStyle(pObj), sciGetFontSize(pObj), sciGetFontOrientation(pObj),
                                                       sciGetIsUsingFractionalMetrics(pObj) == TRUE);

  StringMatrix * textMatrix = sciGetText(pObj);
  getStandardTextDrawerJavaMapper()->setTextContent(getStrMatData(textMatrix), getMatNbRow(textMatrix), getMatNbCol(textMatrix));
}
/*---------------------------------------------------------------------------------*/
StandardTextDrawerJavaMapper * StandardTextDrawerJoGL::getStandardTextDrawerJavaMapper(void)
{
  return dynamic_cast<StandardTextDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
