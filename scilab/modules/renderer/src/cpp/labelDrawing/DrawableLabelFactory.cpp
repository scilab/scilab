/*------------------------------------------------------------------------*/
/* file: DrawableLabelFactory.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for label objects. Automaticaly create a label          */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableLabelFactory.h"
#include "DrawableLabel.h"
#include "XLabelPositionerJoGL.hxx"
#include "YLabelPositionerJoGL.hxx"
#include "ZLabelPositionerJoGL.hxx"
#include "TitlePositionerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableLabelFactory::create( void )
{
  DrawableLabel * newLabel = new DrawableLabel( m_pDrawed ) ;
  setPositionner(newLabel);
  return newLabel;
}
/*---------------------------------------------------------------------------------*/
void DrawableLabelFactory::update( void )
{
  // nothing for now, label type does not change
}
/*---------------------------------------------------------------------------------*/
void DrawableLabelFactory::setPositionner(DrawableLabel * label)
{
  if (label->getLabelPositioner() != NULL)
  {
    delete label->getLabelPositioner();
  }

  switch (pLABEL_FEATURE(m_pDrawed)->ptype)
  {
  case 1:
    // title
    label->setDrawableImp(new TitlePositionerJoGL(label));
    break;
  case 2:
    // x label
    label->setDrawableImp(new XLabelPositionerJoGL(label));
    break;
  case 3:
    // y label
    label->setDrawableImp(new YLabelPositionerJoGL(label));
    break;
  case 4:
    // z label
    label->setDrawableImp(new ZLabelPositionerJoGL(label));
    break;
  default:
    label->setDrawableImp(NULL);
    break;
  }

}
/*---------------------------------------------------------------------------------*/
}
