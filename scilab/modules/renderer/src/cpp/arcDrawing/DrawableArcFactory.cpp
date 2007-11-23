/*------------------------------------------------------------------------*/
/* file: DrawableArcFactory.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for arc objects. Automaticaly create an arc             */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableArcFactory.h"
#include "ConcreteDrawableArc.h"
#include "DrawableArcBridgeFactory.hxx"
#include "ArcLineDrawerJoGL.hxx"
#include "ArcLineDrawerJavaMapper.hxx"
#include "ArcFillDrawerJoGL.hxx"
#include "ArcFillDrawerJavaMapper.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableArcFactory::create( void )
{
  ConcreteDrawableArc * newArc = new ConcreteDrawableArc(m_pDrawed);
  DrawableArcBridgeFactory fact;
  fact.setDrawedArc(newArc);
  newArc->setDrawableImp(fact.create());

  return newArc;

}
/*---------------------------------------------------------------------------------*/
void DrawableArcFactory::update( void )
{
  setStrategies(getArcDrawer(m_pDrawed));
}
/*---------------------------------------------------------------------------------*/
void DrawableArcFactory::setStrategies( DrawableArc * arc )
{
  arc->removeDrawingStrategies();

  if ( sciGetIsFilled(arc->getDrawedObject()) )
  {
    ArcFillDrawerJoGL * strategy = new ArcFillDrawerJoGL(arc);
    strategy->setJavaMapper(new ArcFillDrawerJavaMapper());
    arc->addDrawingStrategy(strategy);
  }

  if ( sciGetIsLine(arc->getDrawedObject()) )
  {
    ArcLineDrawerJoGL * strategy = new ArcLineDrawerJoGL(arc);
    strategy->setJavaMapper(new ArcLineDrawerJavaMapper());
    arc->addDrawingStrategy(strategy);
  }
}
/*---------------------------------------------------------------------------------*/

}
