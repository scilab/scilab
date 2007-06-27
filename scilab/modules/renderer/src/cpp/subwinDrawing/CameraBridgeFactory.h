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

  CameraBridgeFactory( void ) { m_pSubwin = NULL; }

  ~CameraBridgeFactory( void ) { m_pSubwin = NULL;}

  /**
   * Create a new cameraBridge with the rigth driver
   */
  CameraBridge * create( void ) ;

  /**
  * Set the parent subwin, needed by CameraImpFactory.
  */
  void setCorrespondingSubwin( DrawableObject * subwin ) { m_pSubwin = subwin; }

protected:

  DrawableObject * m_pSubwin;

};

}

#endif /* _CAMERA_BRIDGE_FACTORY_ */
