/*------------------------------------------------------------------------*/
/* file: TitlePositionerJoGL.cpp                                          */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class positioning X label using JoGL                            */
/*------------------------------------------------------------------------*/

#include "TitlePositionerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
TitlePositionerJoGL::TitlePositionerJoGL(DrawableLabel * label)
  : LabelPositionerJoGL(label)
{
  setJavaMapper(new TitlePositionerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
TitlePositionerJoGL::~TitlePositionerJoGL(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
double TitlePositionerJoGL::getAutoOrientation(void)
{
  // vertical
  return 0.0;
}
/*------------------------------------------------------------------------------------------*/
void TitlePositionerJoGL::getAutoPosition(double pos[3])
{
  int upperXpos;
  int upperYpos;
  int width;
  int height;
  sciGetViewingArea(sciGetParentSubwin(m_pDrawer->getDrawedObject()),
                    &upperXpos, &upperYpos, &width, &height);
  
  upperXpos = (int) ( upperXpos + (width / 2.0) );
  upperYpos = upperYpos + height;

  getTitlePositionerJavaMapper()->setViewingAreaUpperPos(upperXpos, upperYpos);

  LabelPositionerJoGL::getAutoPosition(pos);
}
/*------------------------------------------------------------------------------------------*/
TitlePositionerJavaMapper * TitlePositionerJoGL::getTitlePositionerJavaMapper(void)
{
  return dynamic_cast<TitlePositionerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
