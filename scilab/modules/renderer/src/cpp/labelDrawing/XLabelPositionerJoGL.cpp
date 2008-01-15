/*------------------------------------------------------------------------*/
/* file: XLabelPositionerJoGL.cpp                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning X label using JoGL                            */
/*------------------------------------------------------------------------*/

#include "XLabelPositionerJoGL.hxx"
#include "XLabelPositionerJavaMapper.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XLabelPositionerJoGL::XLabelPositionerJoGL(DrawableLabel * label)
  : LabelPositionerJoGL(label)
{
  setJavaMapper(new XLabelPositionerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
XLabelPositionerJoGL::~XLabelPositionerJoGL(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
double XLabelPositionerJoGL::getAutoOrientation(void)
{
  // horizontal
  return 0;
}
/*------------------------------------------------------------------------------------------*/
}
