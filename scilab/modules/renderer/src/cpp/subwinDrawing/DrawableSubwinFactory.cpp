/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for subwin objects. Automaticaly create a subwin 
 * drawer with the right algorithms from the garphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSubwinFactory.h"
#include "CameraFactory.h"
#include "DrawableSubwinBridgeFactory.h"
#include "getHandleDrawer.h"
#include "LogarithmicBoundsComputer.hxx"
#include "LinearBoundsComputer.hxx"
#include "BackTrihedronDrawerJoGL.hxx"
#include "FullBoxDrawerJoGL.hxx"
#include "HalfBoxDrawerJoGL.hxx"
#include "TicksDrawerFactory.hxx"
#include "SubwinBackgroundDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableSubwinFactory::create( void )
{
  ConcreteDrawableSubwin * newSubwin = new ConcreteDrawableSubwin( m_pDrawed ) ;
  DrawableSubwinBridgeFactory imp;
  imp.setDrawedSubwin(newSubwin);
  newSubwin->setDrawableImp(imp.create());

  // create the camera
  CameraFactory fact;
  fact.setGraphicObj(m_pDrawed);
  newSubwin->setCamera(fact.create());

  // set strategies
  setStrategies(newSubwin);


  return newSubwin;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwinFactory::update( void )
{
  // update the camera
  CameraFactory fact;
  fact.setGraphicObj(m_pDrawed);
  fact.update();

  // update strategies
  setStrategies(dynamic_cast<ConcreteDrawableSubwin *>(getSubwinDrawer(m_pDrawed)));
  
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwinFactory::setStrategies(ConcreteDrawableSubwin * subwin)
{
  sciPointObj * pSubwin = subwin->getDrawedObject();

  // bounds computation
  char logFlags[3];
  sciGetLogFlags(pSubwin, logFlags);

  if (logFlags[0] == 'l')
  {
    subwin->setXBoundsStrategy(new LogarithmicBoundsComputer());
  }
  else
  {
    subwin->setXBoundsStrategy(new LinearBoundsComputer());
  }

  if (logFlags[1] == 'l')
  {
    subwin->setYBoundsStrategy(new LogarithmicBoundsComputer());
  }
  else
  {
    subwin->setYBoundsStrategy(new LinearBoundsComputer());
  }

  if (logFlags[2] == 'l')
  {
    subwin->setZBoundsStrategy(new LogarithmicBoundsComputer());
  }
  else
  {
    subwin->setZBoundsStrategy(new LinearBoundsComputer());
  }

  // box drawing
  subwin->removeAxesBoxDrawers();
  switch(sciGetBoxType(pSubwin))
  {
  case BT_HIDDEN_AXES:
    subwin->addAxesBoxDrawer(new BackTrihedronDrawerJoGL(subwin));
    break;
  case BT_BACK_HALF:
    subwin->addAxesBoxDrawer(new HalfBoxDrawerJoGL(subwin));
    break;
  case BT_ON:
    subwin->addAxesBoxDrawer(new FullBoxDrawerJoGL(subwin));
    break;
  case BT_OFF:
  default:
    break;
  }

  if (sciGetIsFilled(pSubwin))
  {
    subwin->addAxesBoxDrawer(new SubwinBackgroundDrawerJoGL(subwin));
  }

  // ticks computation
  // update of data bounds needed
  subwin->computeRealDataBounds();
  TicksDrawerFactory tdf(subwin);
  subwin->setXTicksDrawer(tdf.createXTicksDrawer());
  subwin->setYTicksDrawer(tdf.createYTicksDrawer());
  subwin->setZTicksDrawer(tdf.createZTicksDrawer());

}
/*------------------------------------------------------------------------------------------*/

}
