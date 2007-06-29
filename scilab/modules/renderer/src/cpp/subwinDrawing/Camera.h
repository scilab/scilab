/*------------------------------------------------------------------------*/
/* file: Camera.h                                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : File used to position viewpoint and rendering zone in the       */
/*        canvas.                                                         */
/*------------------------------------------------------------------------*/


#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "CameraBridge.h"

namespace sciGraphics
{
  
class Camera
{
public:
  
  Camera( void ) ;

  virtual ~Camera( void ) ;
  
  /**
   * Set the viewing zone for a subwin.
   * @param axesBounds axesBounds (WRECT) of the subwin. This a a vector [x_left,y_up,w,h].
   * @param margins inside the axes bounds, set a surface to use.
   *                This is a vector [margin_left, margin_right, margin_top, margin_bottom].
   */
  void setViewingArea( double axesBounds[4], double margins[4] ) ;

  /**
   * Set the rotation angles of the camera.
   */
  void setRotationAngles( double alpha, double theta ) ;

  /**
   * Specify the axes box to render.
   */
  void setSubWinBox( double box[6] ) ;

  /**
   * Specify the farthest distance from the camera in order to specify zNear and zFar
   */
  void setFarthestDistance( double maxDist ) ;

  /**
   * Position the view and view area accordingly to previous calls.
   */
  void renderPosition( void ) ;

  /**
   * Set the camera bridge
   */
  void setBridge( CameraBridge * bridge ) { m_pImp = bridge; }

protected:

  /** bridge for driver dependent algorithms */
  CameraBridge * m_pImp;

};

}

#endif /* _CAMERA_H_ */

