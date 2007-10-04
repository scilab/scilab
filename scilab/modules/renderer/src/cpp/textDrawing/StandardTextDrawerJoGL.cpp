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
: DrawTextContentStrategy(text), DrawableObjectJoGL(text)
{
  setJavaMapper(new StandardTextDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
StandardTextDrawerJoGL::~StandardTextDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJoGL::getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  // array of size 12.
  double * cornersPos = getStandardTextDrawerJavaMapper()->getBoundingRectangle();
  corner1[0] = cornersPos[0];
  corner1[1] = cornersPos[1];
  corner1[2] = cornersPos[2];

  corner1[0] = cornersPos[3];
  corner1[1] = cornersPos[4];
  corner1[2] = cornersPos[5];

  corner1[0] = cornersPos[6];
  corner1[1] = cornersPos[7];
  corner1[2] = cornersPos[8];

  corner1[0] = cornersPos[9];
  corner1[1] = cornersPos[10];
  corner1[2] = cornersPos[11];

  delete[] cornersPos;
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJoGL::getScreenBoundingBox(int * xPos, int * yPos, int * width, int * height)
{
  // array of size 4.
  long * bbox = getStandardTextDrawerJavaMapper()->getScreenBoundingBox();
  
  *xPos = bbox[0];
  *yPos = bbox[1];
  *width = bbox[2];
  *height = bbox[3];

  delete[] bbox;
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJoGL::drawTextContent(void)
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing();
  int foreground =  sciGetFontContext(pObj)->foregroundcolor;
  getStandardTextDrawerJavaMapper()->setTextParameters(sciGetAlignment(pObj), foreground,
                                                       sciGetFontStyle(pObj), sciGetFontSize(pObj) );

  getStandardTextDrawerJavaMapper()->setTextContent("toto est beau", 1, 1);

  double textPos[3];
  sciGetTextPos(pObj, textPos);
  getStandardTextDrawerJavaMapper()->drawTextContent(textPos[0], textPos[1], textPos[2]);

  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJoGL::showTextContent(void)
{
  show();
}
/*------------------------------------------------------------------------------------------*/
StandardTextDrawerJavaMapper * StandardTextDrawerJoGL::getStandardTextDrawerJavaMapper(void)
{
  return dynamic_cast<StandardTextDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}