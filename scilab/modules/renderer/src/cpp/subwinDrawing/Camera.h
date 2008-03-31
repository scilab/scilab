/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : File used to position viewpoint and rendering zone in the canvas
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


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
   * Convert user coordinates to pixel ones in OpenGL representation
   */
  void project(const double userCoord[3], double pixCoords[3]);

  /**
   * Convert user coordinates to pixel coordinates.
   */
  void getPixelCoordinates(const double userCoord[3], int pixCoord[2]);

  /**
   * Convert user coordinates to pixel coordinates including depth value
   */
  void getPixelCoordinates(const double userCoords[3], double pixCoords[3]);

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  void get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2]);

  /**
   * Unproject a 3 position in pixels into the standard scene coordinates
   */
 void getSceneCoordinates(const double pixCoords[3], double userCoords[3]);

  /**
   * Find coordinates in 2D view from pixel ones
   */
  void get2dViewCoordinates(const int pixCoords[2], double userCoord2D[2]);

  /**
   * Find the projection of user coords on the 2D frame.
   */
  void get2dViewCoordinates(const double userCoords[3], double coord2D[2]);

  /**
   * Switch to pixel coordinate view
   */
  void usePixelCoordinates(void);

  /**
   * Switch back to standard 3D view
   */
  void endPixelCoordinates(void);


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

  /**
   * Apply a zoom square on the axes box
   * @param posX X coordinate of the lower left point of the zoom box in pixels.
   * @param posY Y coordinate of the lower left point of the zoom box in pixels.
   * @param width width of the zooming rectangle.
   * @param height height of the zooming rectangle.
   */
  void zoomRect(int posX, int posY, int width, int height);

  /**
   * Get the position and size of the rectangle in which the axes box must fit
   */
  void getViewingArea(int * xPos, int * yPos, int * width, int * height);

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
   * Apply a zoom square on the axes box
   * @param corners the 4 corners of the zooming rectangle in pixel
   */
  void zoomRect(const double corners[4][2]);

  /**
   * Compute new bounds for one axis.
   * @param corners coordinates of the zooming rectangle in pixels
   * @param axisPoint1 coordinates of a point on the axis in user coordinates.
   * @param axisPoint2 coordinate of an other point on the axis in user coordinates.
   * @param minBound old minimum bound on the axis
   * @param maxNound old maximum bound on the axis
   * @param[out] newMinBound newly computed minimum bound
   * @param[out] newMaxBound newly computed maximum bound.
   */
  void getNewBounds(const double corners[4][2], const double axisPoint1[3], const double axisPoint2[3],
                    double oldMinBound, double oldMaxBound, double * newMinBound, double * newMaxBound);

  /**
   * Compute new bounds for one axis.
   * @param corners coordinates of the zooming rectangle in pixels
   * @param axisPoint1 coordinates of a point on the axis in pixels
   * @param axisPoint2 coordinate of an other point on the axis in pixels
   * @param minBound old minimum bound on the axis
   * @param maxNound old maximum bound on the axis
   * @param[out] newMinBound newly computed minimum bound
   * @param[out] newMaxBound newly computed maximum bound.
   */
  void getNewBoundsPix(const double corners[4][2], const double axisPoint1[2], const double axisPoint2[2],
                       double oldMinBound, double oldMaxBound, double * newMinBound, double * newMaxBound);


  /**
   * Return the real type of implementation object
   */
  CameraBridge * getCameraImp( void ) ;

};

}

#endif /* _CAMERA_H_ */

