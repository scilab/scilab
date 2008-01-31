/*------------------------------------------------------------------------*/
/* file: DrawableSegsFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for segs objects. Automaticaly create a segs            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableSegsFactory.h"
#include "DrawableSegs.h"
#include "DrawableSegsBridgeFactory.hxx"
#include "SegsDecomposer.hxx"
#include "ChampDecomposer.hxx"
#include "SegsLineDrawerJoGL.hxx"
#include "ConcreteDrawableSegs.hxx"
#include "getHandleDrawer.h"
#include "SegsArrowDrawerJoGL.hxx"
#include "SegsLengthDependentArrowDrawerJoGL.hxx"
#include "SegsMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableSegsFactory::create( void )
{
  ConcreteDrawableSegs * newSegs = new ConcreteDrawableSegs( m_pDrawed ) ;
  DrawableSegsBridgeFactory fact;
  fact.setDrawedSegs(newSegs);
  newSegs->setDrawableImp(fact.create());
  setStrategies(newSegs);

  return newSegs;
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableSegs *>(getSegsDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsFactory::setStrategies( ConcreteDrawableSegs * segs )
{
  segs->removeDrawingStrategies();
  sciPointObj * pSegs = segs->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);

  if (ppSegs->ptype == 0)
  {
    segs->setDecomposeStrategy(new SegsDecomposer(segs));
  }
  else
  {
    segs->setDecomposeStrategy(new ChampDecomposer(segs));
  }

  segs->addDrawingStrategy(new SegsLineDrawerJoGL(segs));

  if (sciGetArrowSize(pSegs) > 0)
  {
    segs->addDrawingStrategy(new SegsArrowDrawerJoGL(segs));
  }
  else if (sciGetArrowSize(pSegs) < 0)
  {
    segs->addDrawingStrategy(new SegsLengthDependentArrowDrawerJoGL(segs));
  }
  // if 0 no heads are drawn

  if (sciGetIsMark(pSegs))
  {
    segs->addDrawingStrategy(new SegsMarkDrawerJoGL(segs));
  }

}
/*---------------------------------------------------------------------------------*/
}
