/*------------------------------------------------------------------------*/
/* file: XTicksDrawerJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for X axis                   */
/*------------------------------------------------------------------------*/

#include "XTicksDrawerJoGL.hxx"
#include "TopXTicksDrawerJavaMapper.hxx"
#include "MiddleXTicksDrawerJavaMapper.hxx"
#include "BottomXTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::XTicksDrawerJoGL(DrawableSubwin * subwin)
 : TicksDrawerJoGL(subwin)
{
  sciPointObj * pSubwin = subwin->getDrawedObject();

  if (getJavaMapper() != NULL)
  {
    delete getJavaMapper();
    setJavaMapper(NULL);
  }

  switch(pSUBWIN_FEATURE(pSubwin)->axes.xdir)
  {
  case 'u':
    setJavaMapper(new TopXTicksDrawerJavaMapper());
    break;
  case 'c':
    setJavaMapper(new MiddleXTicksDrawerJavaMapper());
    break;
  case 'd':
    setJavaMapper(new BottomXTicksDrawerJavaMapper());
    break;
  default:
    setJavaMapper(new TopXTicksDrawerJavaMapper());
    break;
  }
  
}
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::~XTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * XTicksDrawerJoGL::getXTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}