/*------------------------------------------------------------------------*/
/* file: ZGridDrawerJoGL.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL for Z axis         */
/*------------------------------------------------------------------------*/

#include "ZGridDrawerJoGL.hxx"
#include "ZGridDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
ZGridDrawerJoGL::ZGridDrawerJoGL(DrawableSubwin * subwin)
  : GridDrawerJoGL(subwin)
{
  setJavaMapper(new ZGridDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
ZGridDrawerJoGL::~ZGridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void ZGridDrawerJoGL::initializeDrawing(void)
{
  GridDrawerJoGL::initializeDrawing();

  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  int zStyle;
  sciGetGridStyle(pSubwin, NULL, NULL, &zStyle);

  getGridDrawerJavaMapper()->setGridParameters(zStyle, (float)sciGetLineWidth(pSubwin));

}
/*------------------------------------------------------------------------------------------*/

}
