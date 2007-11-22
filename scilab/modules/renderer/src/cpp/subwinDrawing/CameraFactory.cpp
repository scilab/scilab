/*------------------------------------------------------------------------*/
/* file: CameraFactory.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for Camera                                              */
/*------------------------------------------------------------------------*/

#include "CameraFactory.h"
#include "CameraBridgeFactory.h"
#include "getHandleDrawer.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------*/
Camera * CameraFactory::create( void )
{
  CameraBridgeFactory bf ;
  Camera * res = new Camera(m_pDrawed) ;

  bf.setCorrespondingCamera( res ) ;
  
  res->setDrawableImp( bf.create() ) ;

  return res;

}
/*------------------------------------------------------------------------*/
void CameraFactory::update( void )
{
  // replace the camera bridge

  // camera to update
  Camera * cam = getSubwinDrawer(m_pDrawed)->getCamera();
  CameraBridgeFactory bf ;
  bf.setCorrespondingCamera( cam ) ;
  cam->setDrawableImp(bf.create());
}
/*------------------------------------------------------------------------*/
}
