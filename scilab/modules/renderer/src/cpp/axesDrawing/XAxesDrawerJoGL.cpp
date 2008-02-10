/*------------------------------------------------------------------------*/
/* file: XAxesDrawerJoGL.cpp                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing X axis object                                     */
/*------------------------------------------------------------------------*/

#include "XAxesDrawerJoGL.hxx"
#include "XAxesDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XAxesDrawerJoGL::XAxesDrawerJoGL(DrawableAxes * axes)
  : TicksDrawerJoGL(axes)
{
  // no choice here, only one position for z ticks
  setJavaMapper(new XAxesDrawerJavaMapper());

}
/*------------------------------------------------------------------------------------------*/
XAxesDrawerJoGL::~XAxesDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void XAxesDrawerJoGL::initializeDrawing(void)
{
  TicksDrawerJoGL::initializeDrawing();

  sciPointObj * pAxes = getDrawer()->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // specif ty the coordinates of the axis
  getXAxesDrawerJavaMapper()->setConstantCoordinates(ppAxes->vy[0], 0.0);

  getXAxesDrawerJavaMapper()->setTicksDirection(ppAxes->dir);

  getXAxesDrawerJavaMapper()->setAxisLineDrawing(ppAxes->seg == 1);

}
/*------------------------------------------------------------------------------------------*/
XAxesDrawerJavaMapper * XAxesDrawerJoGL::getXAxesDrawerJavaMapper(void)
{
  return dynamic_cast<XAxesDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
