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
   * @return true if the axes box has been zoomed, false otherwise
   */
  bool zoomRect(int posX, int posY, int width, int height);

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
   * @return true if the axes box has been zoomed, false otherwise
   */
  bool zoomRect(const double corners[4][2]);

  /**
   * Compute the lines composing the zooming selection area
   * @param areaPixCorners corners of the selction rectangle in pixels
   * @param areaLines 4 line composing the selection area in 3D
   */
  void computeZoomAreaLines(const double areaPixCorners[4][2], double areaLines[4][2][3]);

  /**
   * Compute the 4 intersections of the lines with an x = planeXcoord plane
   * @param areaLines 4 lines composing the selection area in 3D
   * @param planeXcoord either xMin or xMax
   * @return false if no intersections could be computed (lines and plane are parallel)
   */
  bool getXaxisIntersections(const double areaLines[4][2][3], double planeXCoord, double intersections[4][3]);

  /**
   * Compute the 4 intersections of the lines with an y = planeYcoord plane
   * @param areaLines 4 lines composing the selection area in 3D
   * @param planeXcoord either yMin or yMax
   * @return false if no intersections could be computed (lines and plane are parallel)
   */
  bool getYaxisIntersections(const double areaLines[4][2][3], double planeYCoord, double intersections[4][3]);

  /**
   * Compute the 4 intersections of the lines with an z = planeZcoord plane
   * @param areaLines 4 lines composing the selection area in 3D
   * @param planeXcoord either zMin or zMax
   * @return false if no intersections could be computed (lines and plane are parallel)
   */
  bool getZaxisIntersections(const double areaLines[4][2][3], double planeZCoord, double intersections[4][3]);

  /**
   * Compute the intersection of the line defined by p1 and p2 knowing the alpha value
   * @param intersection coordinates of the intersection
   * @param alpha shuld be different than 1
   */
  void getIntersection(const double p1[3], const double p2[3], double alpha, double intersection[3]);

  /**
   * Update the new X bounds with 4 new intersections
   * @param intersections intesection of the selection volume with a plane
   * @param oldXMin previous minimal bound along X axis
   * @param oldXMax previous maximal bound along X axis
   * @param[in/out] newYmin currently computed minimum X bound
   * @param[in/out] newYmax currently computed maximum X bound
   */
  void updateXCoordinate(const double intersections[4][3],
                         double oldXmin, double oldXmax,
                         double & newXmin, double & newXmax);

  /**
   * Update the new Y bounds with 4 new intersections
   * @param intersections intesection of the selection volume with a plane
   * @param oldYMin previous minimal bound along Y axis
   * @param oldYMax previous maximal bound along Y axis
   * @param[in/out] newYmin currently computed minimum Y bound
   * @param[in/out] newYmax currently computed maximum Y bound
   */
  void updateYCoordinate(const double intersections[4][3],
                         double oldYmin, double oldYmax,
                         double & newYmin, double & newYmax);

  /**
   * Update the new z bounds with 4 new intersections
   * @param intersections intesection of the selection volume with a plane
   * @param oldMin previous minimal bound along Z axis
   * @param oldMax previous maximal bound along Z axis
   * @param[in/out] newYmin currently computed minimum Z bound
   * @param[in/out] newYmax currently computed maximum Z bound
   */
  void updateZCoordinate(const double intersections[4][3],
                         double oldZmin, double oldZmax,
                         double & newZmin, double & newZmax);

  /**
   * test if part of the intersections is within a cube side
   * with equatuion x = cst
   * @return true if the intersection are within the side and
   *              that intersections may be used to update data bounds
   */
  bool checkXIntersections(const double intersections[4][3],
                           double oldYmin, double oldYmax,
                           double oldZmin, double oldZmax);

  /**
   * test if part of the intersections is within a cube side
   * with equatuion x = cst
   * @return true if the intersection are within the side and
   *              that intersections may be used to update data bounds
   */
  bool checkYIntersections(const double intersections[4][3],
                           double oldXmin, double oldXmax,
                           double oldZmin, double oldZmax);

  /**
   * test if part of the intersections is within a cube side
   * with equatuion x = cst
   * @return true if the intersection are within the side and
   *              that intersections may be used to update data bounds
   */
  bool checkZIntersections(const double intersections[4][3],
                           double oldXmin, double oldXmax,
                           double oldYmin, double oldYmax);


  /**
   * Return the real type of implementation object
   */
  CameraBridge * getCameraImp( void ) ;

};

}

#endif /* _CAMERA_H_ */

