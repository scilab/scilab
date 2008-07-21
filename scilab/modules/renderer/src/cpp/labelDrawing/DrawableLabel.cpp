/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a label object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableLabel.h"
#include "getHandleDrawer.h"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableLabel::DrawableLabel( sciPointObj * pObj )
  : DrawableObject(pObj)
{
  m_pPositioner = NULL;
}
/*---------------------------------------------------------------------------------*/
DrawableLabel::~DrawableLabel(void)
{
  setLabelPositioner(NULL);
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::setLabelPositioner(LabelPositioner * positioner)
{
  if (m_pPositioner != NULL)
  {
    delete m_pPositioner;
  }
  m_pPositioner = positioner;
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::setDistanceToAxis(double dist)
{
  m_pPositioner->setDistanceToAxis(dist);
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableLabel::draw( void )
{
  DrawableText * drawnText = getTextDrawer(pLABEL_FEATURE(m_pDrawed)->text);
  if (drawnText->isTextEmpty())
  {
    return UNCHANGED;
  }
  // place label if possible
  if (setLabelLocation())
  {
    // draw the enclosed text
    drawnText->hasChanged();
    drawnText->display();
    return SUCCESS;
  }
  return FAILURE;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableLabel::show( void )
{
  DrawableText * drawnText = getTextDrawer(pLABEL_FEATURE(m_pDrawed)->text);
  if (drawnText->isTextEmpty())
  {
    return UNCHANGED;
  }

  // place label if possible
  if (setLabelLocation())
  {
    // draw the enclosed text
    drawnText->hasChanged();
    drawnText->display();
    return SUCCESS;
  }
  
  return FAILURE;
}
/*---------------------------------------------------------------------------------*/
bool DrawableLabel::setLabelLocation(void)
{
  // set orientation before to get the most information when
  // positioning the label
  setLabelOrientation();

  return setLabelPosition();
}
/*---------------------------------------------------------------------------------*/
bool DrawableLabel::setLabelPosition(void)
{
  return m_pPositioner->setLabelPosition();
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::setLabelOrientation(void)
{
  m_pPositioner->setLabelOrientation();
}
/*---------------------------------------------------------------------------------*/

}
