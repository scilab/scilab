/*------------------------------------------------------------------------*/
/* file: XTicksDrawerJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for X axis                   */
/*------------------------------------------------------------------------*/

#include "XTicksDrawerJoGL.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::XTicksDrawerJoGL(DrawableSubwin * subwin)
 : TicksDrawerJoGL(subwin)
{
  setJavaMapper(new XTicksDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::~XTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJavaMapper * XTicksDrawerJoGL::getXTicksDrawerJavaMapper(void)
{
  return dynamic_cast<XTicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}