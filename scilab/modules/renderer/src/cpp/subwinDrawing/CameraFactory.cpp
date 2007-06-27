/*------------------------------------------------------------------------*/
/* file: CameraFactory.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for Camera                                              */
/*------------------------------------------------------------------------*/

#include "CameraFactory.h"
#include "CameraBridgeFactory.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------*/
Camera * CameraFactory::create( void )
{
  CameraBridgeFactory bf ;

  bf.setCorrespondingSubwin( m_pSubwin ) ;

  Camera * res = new Camera() ;

  res->setBridge( bf.create() ) ;

  return res;

}
/*------------------------------------------------------------------------*/

}
