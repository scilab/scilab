/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for axes objects. Automatically create an axes 
 * drawer with the right algorithms from the graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableAxesFactory.h"
#include "DrawableAxesBridgeFactory.hxx"
#include "AxesTicksDrawerJoGL.hxx"
#include "AxesTicksComputer.hxx"
#include "AxesSubticksComputer.hxx"
#include "getHandleDrawer.h"
#include "AxesPositioner.hxx"
#include "../subwinDrawing/TicksDrawer.hxx"
#include "../subwinDrawing/TicksDrawerJoGL.hxx"

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

  // create ticksDrawer
  TicksDrawer * ticksDrawer = new TicksDrawer();
  ticksDrawer->setTicksDrawer(new AxesTicksDrawerJoGL(axes));
  ticksDrawer->setTicksComputer(new AxesTicksComputer(axes));
  ticksDrawer->setSubticksComputer(new AxesSubticksComputer(axes));
  ticksDrawer->setAxisPositioner(new AxesPositioner(axes));
  
  axes->setTicksDrawer(ticksDrawer);

}
/*---------------------------------------------------------------------------------*/
}
