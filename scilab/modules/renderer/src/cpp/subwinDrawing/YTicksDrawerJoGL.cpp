/*------------------------------------------------------------------------*/
/* file: YTicksDrawerJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for X axis                   */
/*------------------------------------------------------------------------*/

#include "YTicksDrawerJoGL.hxx"
#include "LeftYTicksDrawerJavaMapper.hxx"
#include "MiddleYTicksDrawerJavaMapper.hxx"
#include "RightYTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YTicksDrawerJoGL::YTicksDrawerJoGL(DrawableSubwin * subwin)
  : TicksDrawerJoGL(subwin)
{
  sciPointObj * pSubwin = subwin->getDrawedObject();

  if (getJavaMapper() != NULL)
  {
    delete getJavaMapper();
    setJavaMapper(NULL);
  }

  switch(pSUBWIN_FEATURE(pSubwin)->axes.ydir)
  {
  case 'l':
    // special case for 2D, y labels is drawn on the opposite side
    if(sciGetIs3d(pSubwin))
    {
      setJavaMapper(new LeftYTicksDrawerJavaMapper());
    }
    else
    {
      setJavaMapper(new RightYTicksDrawerJavaMapper());
    }
    break;
  case 'c':
    setJavaMapper(new MiddleYTicksDrawerJavaMapper());
    break;
  case 'r':
    // special case for 2D, y labels is drawn on the opposite side
    if(sciGetIs3d(pSubwin))
    {
      setJavaMapper(new RightYTicksDrawerJavaMapper());
    }
    else
    {
      setJavaMapper(new LeftYTicksDrawerJavaMapper());
    }
    break;
  default:
    setJavaMapper(new LeftYTicksDrawerJavaMapper());
    break;
  }

}
/*------------------------------------------------------------------------------------------*/
YTicksDrawerJoGL::~YTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * YTicksDrawerJoGL::getYTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
