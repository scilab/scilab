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
  
class DrawableSubwin;

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
   * @param bounds [Xmin,Xmax,Ymin,Ymax,Zmin,Zmax] vector.
   */
  void setSubwinBox( double bounds[6] ) ;

  /**
   * Position the view and view area accordingly to previous calls.
   */
  void renderPosition( void ) ;

  /**
   * Put back camera to default position.
   */
  void replaceCamera( void );

  /**
   * Set the camera bridge
   */
  void setBridge( CameraBridge * bridge ) { m_pImp = bridge; }

  /**
   * Set the viewed subwindow.
   */
  void setViewedSubwin(DrawableSubwin * subwin) {m_pViewedSubwin = subwin;}

  /**
   * Convert user coordinates to pixel coordinates.
   */
  void getPixelCoordinates(const double userCoord[3], int pixCoord[2]);

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  void get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2]);

protected:

  /*-----------------------------------------------------*/

  /** bridge for driver dependent algorithms */
  CameraBridge * m_pImp;

  /** Viewed subwindow */
  DrawableSubwin * m_pViewedSubwin;
  /*-----------------------------------------------------*/

};

}

#endif /* _CAMERA_H_ */

