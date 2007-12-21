/*------------------------------------------------------------------------*/
/* file: YGridDrawerJoGL.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL for Y axis         */
/*------------------------------------------------------------------------*/

#include "YGridDrawerJoGL.hxx"
#include "YGridDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
YGridDrawerJoGL::YGridDrawerJoGL(DrawableSubwin * subwin)
  : GridDrawerJoGL(subwin)
{
  setJavaMapper(new YGridDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
YGridDrawerJoGL::~YGridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void YGridDrawerJoGL::initializeDrawing(void)
{
  GridDrawerJoGL::initializeDrawing();

  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  int yStyle;
  sciGetGridStyle(pSubwin, NULL, &yStyle, NULL);

  getGridDrawerJavaMapper()->setGridParameters(yStyle, (float)sciGetLineWidth(pSubwin));

}
/*------------------------------------------------------------------------------------------*/

}
