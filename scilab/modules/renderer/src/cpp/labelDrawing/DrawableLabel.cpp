/*------------------------------------------------------------------------*/
/* file: DrawableLabel.cpp                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a label object         */
/*------------------------------------------------------------------------*/

#include "DrawableLabel.h"
#include "getHandleDrawer.h"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableLabel::DrawableLabel( sciPointObj * pObj )
  : DrawableObject(pObj)
{
  
}
/*---------------------------------------------------------------------------------*/
DrawableLabel::~DrawableLabel(void)
{

}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::setDistanceToAxis(double dist)
{
  getLabelPositioner()->setDistanceToAxis(dist);
}
/*---------------------------------------------------------------------------------*/
LabelPositioner * DrawableLabel::getLabelPositioner( void )
{
  return dynamic_cast<LabelPositioner *>(m_pImp);
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::draw( void )
{
  DrawableText * drawnText = getTextDrawer(pLABEL_FEATURE(m_pDrawed)->text);
  if (drawnText->isTextEmpty())
  {
    return;
  }
  // place label
  setLabelLocation();

  // draw the enclosed text
  drawnText->hasChanged();
  drawnText->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::show( void )
{
  DrawableText * drawnText = getTextDrawer(pLABEL_FEATURE(m_pDrawed)->text);
  if (drawnText->isTextEmpty())
  {
    return;
  }
  // place label
  setLabelLocation();

  // draw the enclosed text
  drawnText->hasChanged();
  drawnText->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::setLabelLocation(void)
{
  // set orientation before to get the most information when
  // positioning the label
  setLabelOrientation();

  setLabelPosition();
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::setLabelPosition(void)
{
  getLabelPositioner()->setLabelPosition();
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::setLabelOrientation(void)
{
  getLabelPositioner()->setLabelOrientation();
}
/*---------------------------------------------------------------------------------*/

}
