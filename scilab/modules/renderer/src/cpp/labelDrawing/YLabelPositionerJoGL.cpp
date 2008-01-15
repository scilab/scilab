/*------------------------------------------------------------------------*/
/* file: YLabelPositionerJoGL.cpp                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning Y label using JoGL                            */
/*------------------------------------------------------------------------*/

#include "YLabelPositionerJoGL.hxx"
#include "YLabelPositionerJavaMapper.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YLabelPositionerJoGL::YLabelPositionerJoGL(DrawableLabel * label)
  : LabelPositionerJoGL(label)
{
  setJavaMapper(new YLabelPositionerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
YLabelPositionerJoGL::~YLabelPositionerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
double YLabelPositionerJoGL::getAutoOrientation(void)
{
  // horizontal
  return 0;
}
/*------------------------------------------------------------------------------------------*/
}
