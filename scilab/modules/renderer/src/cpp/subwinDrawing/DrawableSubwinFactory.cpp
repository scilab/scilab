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

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableSubwinFactory::create( void )
{
  DrawableSubwin * res = new DrawableSubwin( m_pDrawed ) ;
  DrawableSubwinBridgeFactory dsbf;
  dsbf.setDrawedSubwin(res);
  res->setDrawableImp(dsbf.create());

  // create the camera
  //CameraFactory cf;
  //cf.setCorrespondingSubwin(res) ;
  //res->setCamera( cf.create() ) ;

  return res;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwinFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
