/*------------------------------------------------------------------------*/
/* file: ZLabelPositionerJoGL.cpp                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning Z label using JoGL                            */
/*------------------------------------------------------------------------*/

#include "ZLabelPositionerJoGL.hxx"
#include "ZLabelPositionerJavaMapper.hxx"

extern "C"
{
#include "math_graphics.h"
}


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
ZLabelPositionerJoGL::ZLabelPositionerJoGL(DrawableLabel * label)
  : LabelPositionerJoGL(label)
{
  setJavaMapper(new ZLabelPositionerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
ZLabelPositionerJoGL::~ZLabelPositionerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
double ZLabelPositionerJoGL::getAutoOrientation(void)
{
  // vertical
  return 1.5 * M_PI;
}
/*------------------------------------------------------------------------------------------*/
}
