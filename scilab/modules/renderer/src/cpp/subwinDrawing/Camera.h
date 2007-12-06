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
#include "../DrawableObject.h"

namespace sciGraphics
{
  
class DrawableSubwin;

// here the drawn object is a subwindow since camera is part of DrawableSubwin
class Camera : public DrawableObject
{
public:
  
  Camera( sciPointObj * pObj ) ;

  virtual ~Camera( void ) ;

  /**
   * Put back camera to default position.
   */
  void replaceCamera( void );

  /**
   * Convert user coordinates to pixel coordinates.
   */
  void getPixelCoordinates(const double userCoord[3], int pixCoord[2]);

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  void get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2]);


  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void );

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void );

protected:

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
   * Specify which axes must be reversed.
   */
  void setAxesReverse(BOOL axesReverse[3]);

  /**
   * Specify the axes box to render.
   * @param bounds [Xmin,Xmax,Ymin,Ymax,Zmin,Zmax] vector.
   */
  void setSubwinBox( double bounds[6] ) ;

  /**
   * Set camera parameters from subwin ones.
   */
  void setCameraParameters(void);

  /**
   * Position the view and view area accordingly to previous calls.
   */
  void renderPosition( void ) ;

  /**
   * Return the real type of implementation object
   */
  CameraBridge * getCameraImp( void ) ;

};

}

#endif /* _CAMERA_H_ */

