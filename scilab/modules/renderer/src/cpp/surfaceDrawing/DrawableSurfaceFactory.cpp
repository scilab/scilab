/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceFactory.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for surface objects. Automaticaly create a surface      */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableSurfaceFactory.h"
#include "ConcreteDrawableSurface.hxx"
#include "DrawableSurfaceBridgeFactory.hxx"
#include "getHandleDrawer.h"
#include "SurfaceFacetDrawerJoGL.hxx"
#include "SurfaceLineDrawerJoGL.hxx"
#include "SurfaceMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableSurfaceFactory::create( void )
{
  ConcreteDrawableSurface * newSurf = new ConcreteDrawableSurface( m_pDrawed ) ;
  DrawableSurfaceBridgeFactory fact;
  fact.setDrawedSurface(newSurf);
  newSurf->setDrawableImp(fact.create());
  setStrategies(newSurf);
  return newSurf;

}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableSurface *>(getSurfaceDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceFactory::setStrategies( ConcreteDrawableSurface * surface )
{
  surface->removeDrawingStrategies();

  sciPointObj * pSurface = surface->getDrawedObject();
  sciSurface * ppSurface = pSURFACE_FEATURE(pSurface);

  if (sciGetIsLine(pSurface))
  {

    if (ppSurface->flag[0] >= 0)
    {
      surface->addDrawingStrategy(new SurfaceLineDrawerJoGL(surface));
    }

    // facet actually
    if (ppSurface->flag[0] != 0)
    {
      surface->addDrawingStrategy(new SurfaceFacetDrawerJoGL(surface));
    }
  }

  if (sciGetIsMark(pSurface))
  {
    surface->addDrawingStrategy(new SurfaceMarkDrawerJoGL(surface));
  }

}
/*---------------------------------------------------------------------------------*/
}
