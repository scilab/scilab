/*------------------------------------------------------------------------*/
/* file: LabelPositioner.cpp                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in positioning labels                         */
/*------------------------------------------------------------------------*/

#include "LabelPositioner.hxx"
#include "DrawableLabel.h"

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
LabelPositioner::LabelPositioner(void)
{
   m_dDistanceToAxis = 0.0;
}
/*------------------------------------------------------------------------------------------*/
LabelPositioner::~LabelPositioner(void)
{
  m_dDistanceToAxis = 0.0;
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::setLabelPosition(void)
{
  sciPointObj * pLabel = getDrawer()->getDrawedObject();
  if (sciGetAutoPosition(pLabel))
  {
    double newPos[3];
    getAutoPosition(newPos);

    sciSetTextPos(pLabel, newPos[0], newPos[1], newPos[2]);
  }
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::setLabelOrientation(void)
{
  sciPointObj * pLabel = getDrawer()->getDrawedObject();
  if (sciGetAutoRotation(pLabel))
  {
    sciSetFontOrientation(pLabel, getAutoOrientation());
  }
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::setDistanceToAxis(double distance)
{
  m_dDistanceToAxis = distance;
}
/*------------------------------------------------------------------------------------------*/
}

