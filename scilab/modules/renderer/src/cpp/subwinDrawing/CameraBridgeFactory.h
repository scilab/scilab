/*------------------------------------------------------------------------*/
/* file: CameraBridgeFactory.h                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for CameraBridge class                                  */
/*------------------------------------------------------------------------*/

#ifndef _CAMERA_BRIDGE_FACTORY_
#define _CAMERA_BRIDGE_FACTORY_

#include "CameraBridge.h"
#include "DrawableSubwin.h"

namespace sciGraphics
{

class CameraBridgeFactory
{

public:

  CameraBridgeFactory( void ) { m_pCamera = NULL; }

  ~CameraBridgeFactory( void ) { m_pCamera = NULL;}

  /**
   * Create a new cameraBridge with the rigth driver
   */
  CameraBridge * create( void ) ;

  /**
  * Set the parent subwin, needed by CameraImpFactory.
  */
  void setCorrespondingCamera( Camera * camera ) { m_pCamera = camera; }

protected:

  Camera * m_pCamera;

};

}

#endif /* _CAMERA_BRIDGE_FACTORY_ */
