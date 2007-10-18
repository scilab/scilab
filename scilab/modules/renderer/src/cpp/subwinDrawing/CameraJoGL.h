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
#include "CameraJavaMapper.hxx"

namespace sciGraphics
{

class DrawableSubwin;

class CameraJoGL : public CameraBridge, public DrawableObjectJoGL
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

  /**
   * Convert user coordinates to pixel coordinates.
   */
  virtual void getPixelCoordinates(const double userCoord[3], int pixCoord[2]);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  CameraJavaMapper * getCameraJavaMapper(void);

};

}

#endif /* _CAMERA_JOGL_H_ */
