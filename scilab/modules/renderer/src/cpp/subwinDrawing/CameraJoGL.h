/*------------------------------------------------------------------------*/
/* file: CameraJoGL.h                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : JoGL class to draw a camera                                     */
/*------------------------------------------------------------------------*/

#ifndef _CAMERA_JOGL_H_
#define _CAMERA_JOGL_H_

#include "../DrawableObjectJoGL.h"
#include "CameraBridge.h"
#include "DrawableSubwin.h"

namespace sciGraphics
{

class DrawableSubwin;

class CameraJoGL : public DrawableObjectJoGL, public CameraBridge
{

public:

  /**
   * Default constructor
   * @param subwin Subwindow containign the camera
   */
  CameraJoGL( DrawableSubwin * subwin ) ;

  /**
   * Position the view and view area accordingly to previous calls.
   */
  virtual void renderPosition( void ) ;

protected:

};

}

#endif /* _CAMERA_JOGL_H_ */
