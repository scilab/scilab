/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableAxes.h                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drivers independent routines to draw an    */
/*        axes object                                                     */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawableAxes.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ConcreteDrawableAxes::ConcreteDrawableAxes(sciPointObj * pAxes)
  : DrawableAxes(pAxes)
{
  m_pTicksDrawer = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableAxes::~ConcreteDrawableAxes(void)
{
  setTicksDrawer(NULL);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableAxes::setTicksDrawer(TicksDrawer * ticksDrawer)
{
  if (m_pTicksDrawer != NULL)
  {
    delete m_pTicksDrawer;
  }
  m_pTicksDrawer = ticksDrawer;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableAxes::drawAxes(void)
{
  m_pTicksDrawer->draw();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableAxes::showAxes(void)
{
  m_pTicksDrawer->showTicks();
}
/*---------------------------------------------------------------------------------*/

}
