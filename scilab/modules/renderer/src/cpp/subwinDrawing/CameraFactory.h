/*------------------------------------------------------------------------*/
/* file: CameraFactory.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for Camera                                              */
/*------------------------------------------------------------------------*/

#ifndef _CAMERA_FACTORY_H_
#define _CAMERA_FACTORY_H_

#include "Camera.h"
#include "DrawableSubwin.h"
#include "../DrawableObjectFactory.h"

namespace sciGraphics
{

class CameraFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  Camera * create( void ) ;

  /**
   * Update the drawer of the drawed object.
   * To be used when an object was deeply modified and to avoid to create it again completely.
   */
  virtual void update( void ) ;

protected:

};

}

#endif /* _CAMERA_FACTORY_H_ */
