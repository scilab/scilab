/*------------------------------------------------------------------------*/
/* file: YAxesDrawerJoGL.cpp                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing Y axis object                                     */
/*------------------------------------------------------------------------*/

#include "YAxesDrawerJoGL.hxx"
#include "YAxesDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YAxesDrawerJoGL::YAxesDrawerJoGL(DrawableAxes * axes)
  : TicksDrawerJoGL(axes)
{
  // no choice here, only one position for z ticks
  setJavaMapper(new YAxesDrawerJavaMapper());

}
/*------------------------------------------------------------------------------------------*/
YAxesDrawerJoGL::~YAxesDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void YAxesDrawerJoGL::initializeDrawing(void)
{
  TicksDrawerJoGL::initializeDrawing();

  sciPointObj * pAxes = getDrawer()->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // specif ty the coordinates of the axis
  getYAxesDrawerJavaMapper()->setConstantCoordinates(ppAxes->vx[0], 0.0);

  getYAxesDrawerJavaMapper()->setTicksDirection(ppAxes->dir);

  getYAxesDrawerJavaMapper()->setAxisLineDrawing(ppAxes->seg == 1);

}
/*------------------------------------------------------------------------------------------*/
YAxesDrawerJavaMapper * YAxesDrawerJoGL::getYAxesDrawerJavaMapper(void)
{
  return dynamic_cast<YAxesDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
