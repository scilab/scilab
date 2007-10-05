/*------------------------------------------------------------------------*/
/* file: StandardTextDrawerJoGL.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class Drawing the text content of a text object with left       */
/*        alignement                                                      */
/*------------------------------------------------------------------------*/

#include "StandardTextDrawerJoGL.hxx"
#include "StandardTextDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
StandardTextDrawerJoGL::StandardTextDrawerJoGL(DrawableText * text)
  : TextContentDrawerJoGL(text)
{
  setJavaMapper(new StandardTextDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
StandardTextDrawerJoGL::~StandardTextDrawerJoGL(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJoGL::setDrawerParameters(void)
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  getStandardTextDrawerJavaMapper()->setTextParameters(sciGetAlignment(pObj), sciGetFontContext(pObj)->foregroundcolor,
                                                       sciGetFontStyle(pObj), sciGetFontSize(pObj), sciGetFontOrientation(pObj) );

  getStandardTextDrawerJavaMapper()->setTextContent("toto est beau", 1, 1);

  double textPos[3];
  sciGetTextPos(pObj, textPos);
  getStandardTextDrawerJavaMapper()->setCenterPosition(textPos[0], textPos[1], textPos[2]);
}
/*------------------------------------------------------------------------------------------*/
StandardTextDrawerJavaMapper * StandardTextDrawerJoGL::getStandardTextDrawerJavaMapper(void)
{
  return dynamic_cast<StandardTextDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
