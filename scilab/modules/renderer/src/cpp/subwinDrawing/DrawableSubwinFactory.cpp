/*------------------------------------------------------------------------*/
/* file: DrawableSubwinFactory.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for subwin objects. Automaticaly create a subwin        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableSubwinFactory.h"
#include "DrawableSubwin.h"
#include "CameraFactory.h"
#include "DrawableSubwinBridgeFactory.h"
#include "getHandleDrawer.h"


namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableSubwinFactory::create( void )
{
  DrawableSubwin * newSubwin = new DrawableSubwin( m_pDrawed ) ;
  DrawableSubwinBridgeFactory imp;
  imp.setDrawedSubwin(newSubwin);
  newSubwin->setDrawableImp(imp.create());

  // create the camera
 setNewCamera(newSubwin);

  return newSubwin;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwinFactory::update( void )
{
  DrawableSubwin * curSubwin = getSubwinDrawer(m_pDrawed);
  setNewCamera(curSubwin);
  
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwinFactory::setNewCamera(DrawableSubwin * subwin)
{
  // set a new camera
  CameraFactory cf;
  cf.setCorrespondingSubwin(subwin) ;
  Camera * newCam = cf.create();
  newCam->setViewedSubwin(subwin);
  subwin->setCamera( newCam );
}
/*------------------------------------------------------------------------------------------*/
}
