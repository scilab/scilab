/*------------------------------------------------------------------------*/
/* file: DrawableAxesFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for axes objects. Automaticaly create an axes           */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableAxesFactory.h"
#include "DrawableAxesBridgeFactory.hxx"
#include "XAxesDrawerJoGL.hxx"
#include "AxesTicksComputer.hxx"
#include "getHandleDrawer.h"
#include "YAxesDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableAxesFactory::create( void )
{
  ConcreteDrawableAxes * newAxes = new ConcreteDrawableAxes( m_pDrawed ) ;
  DrawableAxesBridgeFactory fact;
  fact.setDrawedAxes(newAxes);
  newAxes->setDrawableImp(fact.create());
  setStrategies(newAxes);

  return newAxes;
}
/*---------------------------------------------------------------------------------*/
void DrawableAxesFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableAxes *>(getAxesDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableAxesFactory::setStrategies( ConcreteDrawableAxes * axes )
{
  sciPointObj * pAxes = axes->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // create ticksDrawer
  TicksDrawer * ticksDrawer = NULL;
  
  if (ppAxes->nx > ppAxes->ny)
  {
    ticksDrawer = new XAxesDrawerJoGL(axes);
  } else {
    ticksDrawer = new YAxesDrawerJoGL(axes);
  }
  
  ticksDrawer->setTicksComputer(new AxesTicksComputer(axes));
  
  axes->setTicksDrawer(ticksDrawer);

}
/*---------------------------------------------------------------------------------*/
}
