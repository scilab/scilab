/*------------------------------------------------------------------------*/
/* file: XGridDrawerJoGL.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL for X axis         */
/*------------------------------------------------------------------------*/

#include "XGridDrawerJoGL.hxx"
#include "XGridDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
XGridDrawerJoGL::XGridDrawerJoGL(DrawableSubwin * subwin)
  : GridDrawerJoGL(subwin)
{
  setJavaMapper(new XGridDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
XGridDrawerJoGL::~XGridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void XGridDrawerJoGL::initializeDrawing(void)
{
  GridDrawerJoGL::initializeDrawing();

  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  int xStyle;
  sciGetGridStyle(pSubwin, &xStyle, NULL, NULL);
  
  getGridDrawerJavaMapper()->setGridParameters(xStyle, (float)sciGetLineWidth(pSubwin));

}
/*------------------------------------------------------------------------------------------*/

}
