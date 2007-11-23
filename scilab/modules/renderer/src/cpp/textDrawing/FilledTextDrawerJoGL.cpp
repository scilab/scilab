/*------------------------------------------------------------------------*/
/* file: FilledTextDrawerJoGL.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class Drawing the text content of a text object which is filled */
/*        inside a text box                                               */
/*------------------------------------------------------------------------*/

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
  double boxWidth;
  double boxHeight;
  sciGetUserSize(pObj, &boxWidth, &boxHeight);

  
  StringMatrix * textMatrix = sciGetText(pObj);
  getFilledTextDrawerJavaMapper()->setTextContent(getStrMatData(textMatrix), getMatNbRow(textMatrix), getMatNbCol(textMatrix));

  // get the center Position
  double textPos[3];
  sciGetTextPos(pObj, textPos);

  // convert the user lengths to pixel ones.
  int pixWidth;
  int pixHeight;
  getPixelLength(sciGetParentSubwin(pObj), textPos, boxWidth, boxHeight, &pixWidth, &pixHeight);

  getFilledTextDrawerJavaMapper()->setTextParameters(sciGetAlignment(pObj), sciGetFontContext(pObj)->foregroundcolor,
                                                     sciGetFontStyle(pObj), sciGetFontOrientation(pObj),
                                                     pixWidth, pixHeight);

  getFilledTextDrawerJavaMapper()->setCenterPosition(textPos[0], textPos[1], textPos[2]);
}
/*---------------------------------------------------------------------------------*/
FilledTextDrawerJavaMapper * FilledTextDrawerJoGL::getFilledTextDrawerJavaMapper(void)
{
  return dynamic_cast<FilledTextDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
