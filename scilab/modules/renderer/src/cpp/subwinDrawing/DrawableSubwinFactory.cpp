/*------------------------------------------------------------------------*/
/* file: DrawableSubwinFactory.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for subwin objects. Automaticaly create a subwin        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableSubwinFactory.h"
#include "CameraFactory.h"
#include "DrawableSubwinBridgeFactory.h"
#include "getHandleDrawer.h"
#include "LogarithmicBoundsComputer.hxx"
#include "LinearBoundsComputer.hxx"

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
}
/*------------------------------------------------------------------------------------------*/

}
