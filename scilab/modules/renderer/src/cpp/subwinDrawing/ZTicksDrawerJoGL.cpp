/*------------------------------------------------------------------------*/
/* file: ZTicksDrawerJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for Z axis                   */
/*------------------------------------------------------------------------*/

#include "ZTicksDrawerJoGL.hxx"
#include "ZTicksDrawerJavaMapper.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
ZTicksDrawerJoGL::ZTicksDrawerJoGL(DrawableSubwin * subwin)
  : TicksDrawerJoGL(subwin)
{

  // no choice here, only one position for z ticks
  setJavaMapper(new ZTicksDrawerJavaMapper());

}
/*------------------------------------------------------------------------------------------*/
ZTicksDrawerJoGL::~ZTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * ZTicksDrawerJoGL::getZTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
