/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for arc objects. Automaticaly create an arc drawer with the right algorithms from the garphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
  setStrategies(newArc);

  return newArc;

}
/*---------------------------------------------------------------------------------*/
void DrawableArcFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableArc *>(getArcDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableArcFactory::setStrategies( ConcreteDrawableArc * arc )
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
