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

namespace sciGraphics
{

class CameraFactory
{

public:

  CameraFactory( void ) { m_pSubwin = NULL; }

  virtual ~CameraFactory( void ) { m_pSubwin = NULL; }

  /**
   * Create a new camera instance
   */
  virtual Camera * create( void ) ;

  /**
   * Set the parent subwin, needed by CameraImpFactory.
   */
  void setCorrespondingSubwin( DrawableObject * subwin ) { m_pSubwin = subwin; }

protected:

  DrawableObject * m_pSubwin;

};

}

#endif /* _CAMERA_FACTORY_H_ */
