/*------------------------------------------------------------------------*/
/* file: DrawableFecFactory.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for fec objects. Automaticaly create a fec              */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableFecFactory.h"
#include "DrawableFecBridgeFactory.hxx"
#include "FecLineDrawerJoGL.hxx"
#include "FecFacetDrawerJoGL.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableFecFactory::create( void )
{
  ConcreteDrawableFec * newFec = new ConcreteDrawableFec( m_pDrawed ) ;
  DrawableFecBridgeFactory fact;
  fact.setDrawedFec(newFec);
  newFec->setDrawableImp(fact.create());

  return newFec;
}
/*---------------------------------------------------------------------------------*/
void DrawableFecFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableFec *>(getFecDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableFecFactory::setStrategies(ConcreteDrawableFec * fec)
{
  sciFec * ppFec = pFEC_FEATURE(m_pDrawed);
  fec->removeDrawingStrategies();
  
  if (ppFec->with_mesh)
  {
    fec->addDrawingStrategy(new FecLineDrawerJoGL(fec));
  }

  fec->addDrawingStrategy(new FecFacetDrawerJoGL(fec));

}
/*---------------------------------------------------------------------------------*/

}
