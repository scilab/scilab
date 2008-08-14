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
   * Convert user coordinates to pixel coordinates including depth value.
   * Does not take logarithmic scale into account.
   */
  void getPixelCoordinatesRaw(const double userCoords[3], double pixCoords[3]);

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  void get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2]);

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  void get2dViewPixelCoordinates(const double userCoord[3], double pixCoord[3]);

  /**
   * Unproject a 3 position in pixels into the standard scene coordinates
   */
  void getSceneCoordinates(const double pixCoords[3], double userCoords[3]);

  /**
   * Unproject a 3 position in pixels into the standard scene coordinates.
   * Does not take logarithmic coordinates into account
   */
  void getSceneCoordinatesRaw(const double pixCoords[3], double userCoords[3]);

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
  virtual EDisplayStatus draw( void );

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void );

  /**
  * Display the camera using precomputed data
  */
  virtual EDisplayStatus redraw(void);

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

  /**
   * Get viewport of the canvas
   */
  void getViewport(int viewport[4]);

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
   * Return the real type of implementation object
   */
  CameraBridge * getCameraImp( void ) ;


private:
  
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
  void updateXCoordinate(const double intersections[][3],
                         int nbIntersections,
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
  void updateYCoordinate(const double intersections[][3],
                         int nbIntersections,
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
  void updateZCoordinate(const double intersections[][3],
                         int nbIntersections,
                         double oldZmin, double oldZmax,
                         double & newZmin, double & newZmax);

  /**
   * Compute the summits of the intersection of the viewing volume and on of the axes cub face
   * aligned with X axis.
   * @param intersections summits of the intsection between the viewing area and the plane
   *                      (infinite) of the face
   * @param projectedIntersections result, summit (up to 8) of the resulting area
   * @param planeXCoord X coordinate of the face
   * @param yMin bound of the face
   * @param yMax bound of the face
   * @param zMin bound of the face
   * @param zMax bound of the face
   * @return number of summits founds (from 0 to 8), if 0 the viewing area does not intersect the face
   */
  int getProjectedXIntersections(const double intersections[4][3], double projectedIntersections[8][3],
                                 double planeXCoord,
                                 double yMin, double yMax, double zMin, double zMax);

  /**
   * Compute the summits of the intersection of the viewing volume and on of the axes cub face
   * aligned with Y axis.
   * @param intersections summits of the intsection between the viewing area and the plane
   *                      (infinite) of the face
   * @param projectedIntersections result, summit (up to 8) of the resulting area
   * @param planeYCoord Y coordinate of the face
   * @param xMin bound of the face
   * @param xMax bound of the face
   * @param zMin bound of the face
   * @param zMax bound of the face
   * @return number of summits founds (from 0 to 8), if 0 the viewing area does not intersect the face
   */
  int getProjectedYIntersections(const double intersections[4][3], double projectedIntersections[8][3],
                                 double planeYCoord,
                                 double xMin, double xMax, double zMin, double zMax);

  /**
   * Compute the summits of the intersection of the viewing volume and on of the axes cub face
   * aligned with Z axis.
   * @param intersections summits of the intsection between the viewing area and the plane
   *                      (infinite) of the face
   * @param projectedIntersections result, summit (up to 8) of the resulting area
   * @param planeZCoord Z coordinate of the face
   * @param xMin bound of the face
   * @param xMax bound of the face
   * @param yMin bound of the face
   * @param yMax bound of the face
   * @return number of summits founds (from 0 to 8), if 0 the viewing area does not intersect the face
   */
  int getProjectedZIntersections(const double intersections[4][3], double projectedIntersections[8][3],
                                 double planeZCoord,
                                 double xMin, double xMax, double yMin, double yMax);

  /**
   * Get the projection in 2D of a point on a plane aligned with X axis
   */
  void projectOnXPlane(const double point[3], double proj[2]);

  /**
   * Get the 3D coordiates of a point lying on a plane aligned with X axis
   * @param planeXCoord X coordinate of the plane
   */
  void unProjectOnXPlane(const double proj[2], double point[3], double planeXCoord);

  /**
   * Get the projection in 2D of a point on a plane aligned with Y axis
   */
  void projectOnYPlane(const double point[3], double proj[2]);

  /**
   * Get the 3D coordiates of a point lying on a plane aligned with Y axis
   * @param planeYCoord Y coordinate of the plane
   */
  void unProjectOnYPlane(const double proj[2], double point[3], double planeYCoord);

  /**
   * Get the projection in 2D of a point on a plane aligned with Z axis
   */
  void projectOnZPlane(const double point[3], double proj[2]);

  /**
   * Get the 3D coordiates of a point lying on a plane aligned with Z axis
   * @param planeZCoord Z coordinate of the plane
   */
  void unProjectOnZPlane(const double proj[2], double point[3], double planeZCoord);

  /**
   * Compute the summits of the intersection of a quandrangle and an axis aliged rectangle
   * @param intersections summits of the intsection between the quadrangle and the rectangme
   * @param projectedIntersections result, summit (up to 8) of the resulting area
   * @param xMin bound of the rectangle
   * @param xMax bound of the rectangle
   * @param yMin bound of the rectangle
   * @param yMax bound of the rectangle
   * @return number of summits founds (from 0 to 8), if 0 the quadrandle does not intersect the face
   */
  int getProjectedIntersections2D(const double intersections[4][2], double projectedIntersections[8][2],
                                  double xMin, double xMax, double yMin, double yMax);

  /**
   * Compute the two extremities of teh segment resulting in the intersection
   * of a rectangle (filled) and a segment in 2D.
   * @param p1 first end of the segment
   * @param p2 second end of the segment
   * @param xMin bound of the rectangle
   * @param xMax bound of the rectangle
   * @param yMin bound of the rectangle
   * @param yMax bound of the rectangle
   * @param intersections result, if an intersection has been found, contains the two ends
   *                      of the intersection
   * @return if true an intersection has been found, if false there is no intersection between
   *         the segment and the rectangle
   */
  bool computeLineFillRectangleIntersections(const double p1[2], const double p2[2],
                                             double xMin, double xMax, double yMin, double yMax,
                                             double intersections[2][2]);
  /**
   * Check if a point is inside an axis aligned rectangle
   */
  bool isInsideRectangle(const double p[2], double xMin, double xMax, double yMin,
                               double yMax);

  /**
   * Compute the intersection between a segment and the outside of a rectangle
   * @param p1 first end of the segment
   * @param p2 second end of the segment
   * @param xMin bound of the rectangle
   * @param xMax bound of the rectangle
   * @param yMin bound of the rectangle
   * @param yMax bound of the rectangle
   * @param intersection result, positions of the found intersection(s).
   * @return number of found intersections, might be 0, 1 or 2
   */
  int computeLineRectangleIntersections(const double p1[2], const double p2[2],
                                        double xMin, double xMax, double yMin, double yMax,
                                        double intersections[2][2]);

  /**
   * Compute intersection between a segment and an edge a rectangle aligned with X axis
   * @param p1 first end of the segment
   * @param p2 second end of the segment
   * @param res result, position of the intersection if one has been found
   * @param rectXCoord X coordinate of the edge
   * @param yMin bound of the edge
   * @param yMax bound of the edge
   * @return true if the segement intersect the edge, false otherwise
   */
  bool checkXIntersection(const double p1[2], const double p2[2], double res[2],
                          double rectXCoord, double yMin, double yMax);


  /**
   * Compute intersection between a segment and an edge a rectangle aligned with Y axis
   * @param p1 first end of the segment
   * @param p2 second end of the segment
   * @param res result, position of the intersection if one has been found
   * @param rectYCoord Y coordinate of the edge
   * @param xMin bound of the edge
   * @param xMax bound of the edge
   * @return true if the segement intersect the edge, false otherwise
   */
  bool checkYIntersection(const double p1[2], const double p2[2], double res[2],
                          double rectYCoord, double xMin, double xMax);

  /**
   * Compute P1 + a.P1P2
   */
  void p1PlusAP1P2(const double p1[2], const double p2[2], double a, double res[2]);

  /**
   * Check if a point is inside a quad defined by its four summits)
   */
  bool isPointInQuadrangle(const double point[2], const double corners[4][2]);

  /**
   * Create some graphic objects to represent the zooming area
   * For debug urpose only.
   */
  void visualizeZoomingArea(const double intersections[4][2][3]);

  /**
   * Create some objects to help visulazing intersections
   * For debug purpose only.
   */
  void visualizeIntersection(const double intersections[4][3]);

};

}

#endif /* _CAMERA_H_ */

