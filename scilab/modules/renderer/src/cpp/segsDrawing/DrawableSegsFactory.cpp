/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for segs objects. Automatically create a segs
 * drawer with the right algorithms from the graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSegsFactory.h"
#include "DrawableSegs.h"
#include "DrawableSegsBridgeFactory.hxx"
#include "SegsDecomposer.hxx"
#include "ChampDecomposer.hxx"
#include "SegsLineDrawerJoGL.hxx"
#include "ConcreteDrawableSegs.hxx"
#include "getHandleDrawer.h"
#include "SegsArrowDrawerJoGL.hxx"
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

	if (sciGetLineWidth(pSegs) > 0)
	{
		segs->addDrawingStrategy(new SegsLineDrawerJoGL(segs));
	}

  if (sciGetArrowSize(pSegs) != 0)
  {
    segs->addDrawingStrategy(new SegsArrowDrawerJoGL(segs));
  }
  // if 0 no heads are drawn

  if (sciGetIsMark(pSegs))
  {
    segs->addDrawingStrategy(new SegsMarkDrawerJoGL(segs));
  }

}
/*---------------------------------------------------------------------------------*/
}
