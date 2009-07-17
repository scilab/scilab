/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for surface objects. Automaticaly create a surface
 * drawer with the right algorithms from the garphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

		if (ppSurface->flag[0] >= 0 && sciGetLineWidth(pSurface) > 0.0)
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
