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
void DrawableLabel::draw( void )
{
  // draw the enclosed text
  getHandleDrawer(pLABEL_FEATURE(m_pDrawed)->text)->hasChanged();
  getHandleDrawer(pLABEL_FEATURE(m_pDrawed)->text)->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableLabel::show( void )
{
  // show the enclosed text
  getHandleDrawer(pLABEL_FEATURE(m_pDrawed)->text)->display();
}
/*---------------------------------------------------------------------------------*/


}
