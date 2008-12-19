/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : File used to position viewpoint and rendering zone in the
 * canvas
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Camera.h"
#include "DrawableSubwin.h"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
Camera::Camera( sciPointObj * pObj ) : DrawableObject(pObj)
{
  
}
/*--------------------------------------------------------------------------*/
Camera::~Camera( void )
{
  
}
/*--------------------------------------------------------------------------*/
void Camera::setViewingArea( double axesBounds[4], double margins[4] )
{
  double translation[2] ;
  // T = Taxes + SizeAxes * [marginLeft, marginTop].
  translation[0] = axesBounds[0] + axesBounds[2] * margins[0] ;
  translation[1] = 1.0 - axesBounds[1] - axesBounds[3] * (1.0 - margins[3]) ;

  // S = Saxes - margin.Saxes
  // where Saxes = [w,h] and margin = [margin_left+margin_right, margin_top+margin_bottom].
  double scale[2] ;
  scale[0] = (1.0 - margins[0] - margins[1]) * axesBounds[2] ;
  scale[1] = (1.0 - margins[2] - margins[3]) * axesBounds[3] ;
  getCameraImp()->setViewingArea(translation, scale);
}
/*--------------------------------------------------------------------------*/
void Camera::setRotationAngles( double alpha, double theta )
{
  getCameraImp()->setAxesRotation(alpha, theta);
}
/*--------------------------------------------------------------------------*/
void Camera::setAxesReverse(BOOL axesReverse[3])
{
  getCameraImp()->setAxesReverse(axesReverse[0] == TRUE,
                                 axesReverse[1] == TRUE,
                                 axesReverse[2] == TRUE);
}
/*--------------------------------------------------------------------------*/
void Camera::setSubwinBox( double bounds[6] )
{

  double boxCenter[3];
  boxCenter[0] = (bounds[0] + bounds[1]) / 2.0 ;
  boxCenter[1] = (bounds[2] + bounds[3]) / 2.0 ;
  boxCenter[2] = (bounds[4] + bounds[5]) / 2.0 ;
  getCameraImp()->setAxesCenter(boxCenter) ;
  
  double scale[3] ;
  // 1.0 / ( Xmax - Xmin )
  scale[0] = 1.0 / (bounds[1] - bounds[0]) ;
  scale[1] = 1.0 / (bounds[3] - bounds[2]) ;
  scale[2] = 1.0 / (bounds[5] - bounds[4]) ;
  
  getCameraImp()->setAxesNormalizationScale(scale) ;

	if (sciGetIsCubeScaled(m_pDrawed))
  {
    getCameraImp()->setAxesFittingScale(scale);
  }
  else
  {
    // preserve isometry by applying same scale
		double minScale = 1.0;
    double fittingScale[3] = {minScale, minScale, minScale};
    getCameraImp()->setAxesFittingScale(fittingScale);
  }
  

  double trans[3];
  // put the min bounds to our origin
  trans[0] = -bounds[0] ;
  trans[1] = -bounds[2] ;
  trans[2] = bounds[4] ;
  getCameraImp()->setAxesTranslation(trans) ;

}
/*--------------------------------------------------------------------------*/
void Camera::setCameraParameters(void)
{
  // here m_pDrawed is the subwin
  setViewingArea(sciGetAxesBounds(m_pDrawed), sciGetMargins(m_pDrawed)) ;

  double bounds[6] ;
  sciGetRealDataBounds(m_pDrawed, bounds) ;
  setSubwinBox(bounds) ;

  double alpha;
  double theta;
  sciGetViewingAngles(m_pDrawed, &alpha, &theta);
  setRotationAngles(alpha, theta);

  BOOL axesReverse[3];
  sciGetAxesReverse(m_pDrawed, axesReverse);
  setAxesReverse(axesReverse);
}
/*--------------------------------------------------------------------------*/
void Camera::renderPosition( void )
{
  getCameraImp()->renderPosition();
}
/*--------------------------------------------------------------------------*/
void Camera::replaceCamera( void )
{
  getCameraImp()->replaceCamera();
}
/*--------------------------------------------------------------------------*/
void Camera::getPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  getCameraImp()->getPixelCoordinates(userCoord, pixCoord);
}
/*--------------------------------------------------------------------------*/
void Camera::getPixelCoordinates(const double userCoords[3], double pixCoords[3])
{
  getCameraImp()->getPixelCoordinates(userCoords, pixCoords);
}
/*--------------------------------------------------------------------------*/
void Camera::getPixelCoordinatesRaw(const double userCoords[3], double pixCoords[3])
{
  getCameraImp()->getPixelCoordinatesRaw(userCoords, pixCoords);
}
/*--------------------------------------------------------------------------*/
void Camera::get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  getCameraImp()->get2dViewPixelCoordinates(userCoord, pixCoord);
}
/*--------------------------------------------------------------------------*/
void Camera::get2dViewPixelCoordinates(const double userCoord[3], double pixCoord[3])
{
  getCameraImp()->get2dViewPixelCoordinates(userCoord, pixCoord);
}
/*--------------------------------------------------------------------------*/
void Camera::getSceneCoordinates(const double pixCoords[3], double userCoords[3])
{
  getCameraImp()->getSceneCoordinates(pixCoords, userCoords);
}
/*--------------------------------------------------------------------------*/
void Camera::getSceneCoordinatesRaw(const double pixCoords[3], double userCoords[3])
{
  getCameraImp()->getSceneCoordinatesRaw(pixCoords, userCoords);
}
/*--------------------------------------------------------------------------*/
void Camera::get2dViewCoordinates(const int pixCoords[2], double userCoord2D[2])
{
  getCameraImp()->get2dViewCoordinates(pixCoords, userCoord2D);
}
/*--------------------------------------------------------------------------*/
void Camera::get2dViewCoordinates(const double userCoords[3], double coord2D[2])
{
  int pixCoords[2];
  // convert usetr coords to pixel ones
  getPixelCoordinates(userCoords, pixCoords);

  // convert pixel ones to 2d frame ones
  get2dViewCoordinates(pixCoords, coord2D);
}
/*--------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus Camera::draw( void )
{
  initializeDrawing();
  setCameraParameters();
  renderPosition();
  endDrawing();
  return SUCCESS;
}
/*--------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus Camera::show( void )
{
  getCameraImp()->show();
  return SUCCESS;
}
/*------------------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus Camera::redraw(void)
{
  initializeDrawing();
  getCameraImp()->redraw();
  endDrawing();
  return SUCCESS;
}
/*------------------------------------------------------------------------------------------*/
bool Camera::zoomRect(int posX, int posY, int width, int height)
{
  double rectCorners[4][2] = {{posX        , posY + height},
                              {posX        , posY         },
                              {posX + width, posY         },
                              {posX + width, posY + height}};

  return zoomRect(rectCorners);

}
/*------------------------------------------------------------------------------------------*/
bool Camera::zoomRect(const double corners[4][2])
{

  // the aim of thi salgorithm is to perform zomming inside a 3D axes
  // knowing a rectangle in pixels
  // In 3d the selection rectangle can be extended as a infinite
  // tube with a rectangular section aligned with view
  // Ideally, the viewing area after the zoom should be the intersection
  // of the selection volume and the axes box.
  // However, in Scilab the axes box always remains a box.
  // Consequently to perform a zoom, we compute the extreme bounds of the intersection
  // in the 3 directions and use them as new bounds for the axes box.

  double oldDataBounds[6];

  // get data bounds, already scaled
  sciGetRealDataBounds(m_pDrawed, oldDataBounds);
 
  // first step compute the 4 lines composing the selection area
  // in 3d coordinates
  // the selection area is an infinite cone (tube)
  // of rectangular section.
  double selectionLines[4][2][3]; // contains 4 lines which are actually 2 points
  computeZoomAreaLines(corners, selectionLines);

  // second step project lines on each of the 6 axes cube plane
  // actually we just need the found the bounds of the intersection of
  // the selectionArea and the axes cube. And the bounds are found on the cube.
  // So we just need to focus on the 6 faces of the cube

  // set initial bounds
  // set the otherbound since we wants to found minima
  // and maxima
  double oldXmin = oldDataBounds[0];
  double oldXmax = oldDataBounds[1];
  double oldYmin = oldDataBounds[2];
  double oldYmax = oldDataBounds[3];
  double oldZmin = oldDataBounds[4];
  double oldZmax = oldDataBounds[5];

  double newXmin = oldDataBounds[1]; // oldXmax
  double newXmax = oldDataBounds[0]; // oldXmin
  double newYmin = oldDataBounds[3]; // oldYmax
  double newYmax = oldDataBounds[2]; // oldYmin
  double newZmin = oldDataBounds[5]; // oldZMax
  double newZmax = oldDataBounds[4]; // oldZmin

  // the four intersections with one plane
  double intersections[4][3];

  // edges of the intersection and the axes cube face
  double projectedIntersections[8][3];



  // intersection with x = Xmin axis
  if (getXaxisIntersections(selectionLines, oldXmin, intersections))
  {
    int nbProjectedIntersections = getProjectedXIntersections(intersections, projectedIntersections,
                                                              oldXmin, oldYmin, oldYmax, oldZmin, oldZmax);
    if (nbProjectedIntersections > 0)
    {
      // ok we found points and the selection intersect the side of the cube
      // update Y and Z coordinates
      // don't try to update X here, it's just xMin
      updateYCoordinate(projectedIntersections, nbProjectedIntersections, oldYmin, oldYmax, newYmin, newYmax);
      updateZCoordinate(projectedIntersections, nbProjectedIntersections, oldZmin, oldZmax, newZmin, newZmax);

      newXmin = oldXmin;
    }
  }

 

  // same with x = Xmax axis
  if (getXaxisIntersections(selectionLines, oldXmax, intersections))
  {
    int nbProjectedIntersections = getProjectedXIntersections(intersections, projectedIntersections,
                                                              oldXmax, oldYmin, oldYmax, oldZmin, oldZmax);
    if (nbProjectedIntersections > 0)
    {
      updateYCoordinate(projectedIntersections, nbProjectedIntersections, oldYmin, oldYmax, newYmin, newYmax);
      updateZCoordinate(projectedIntersections, nbProjectedIntersections, oldZmin, oldZmax, newZmin, newZmax);

      newXmax = oldXmax;
    }
  }

  // same with y = Ymin axis
  if (getYaxisIntersections(selectionLines, oldYmin, intersections))
  {
    int nbProjectedIntersections = getProjectedYIntersections(intersections, projectedIntersections,
                                                              oldYmin, oldXmin, oldXmax, oldZmin, oldZmax);
    if (nbProjectedIntersections > 0)
    {
      updateXCoordinate(projectedIntersections, nbProjectedIntersections, oldXmin, oldXmax, newXmin, newXmax);
      updateZCoordinate(projectedIntersections, nbProjectedIntersections, oldZmin, oldZmax, newZmin, newZmax);

      newYmin = oldYmin;
    }
  }

  // same with y = Ymax axis
  if (getYaxisIntersections(selectionLines, oldYmax, intersections))
  {
    int nbProjectedIntersections = getProjectedYIntersections(intersections, projectedIntersections,
                                                              oldYmax, oldXmin, oldXmax, oldZmin, oldZmax);
    if (nbProjectedIntersections > 0)
    {
      updateXCoordinate(projectedIntersections, nbProjectedIntersections, oldXmin, oldXmax, newXmin, newXmax);
      updateZCoordinate(projectedIntersections, nbProjectedIntersections, oldZmin, oldZmax, newZmin, newZmax);

      newYmax = oldYmax;
    }
  }

  // same with z = Zmin axis
  if (getZaxisIntersections(selectionLines, oldZmin, intersections))
  {
    int nbProjectedIntersections = getProjectedZIntersections(intersections, projectedIntersections,
                                                              oldZmin, oldXmin, oldXmax, oldYmin, oldYmax);
    if (nbProjectedIntersections > 0)
    {
      updateXCoordinate(projectedIntersections, nbProjectedIntersections, oldXmin, oldXmax, newXmin, newXmax);
      updateYCoordinate(projectedIntersections, nbProjectedIntersections, oldYmin, oldYmax, newYmin, newYmax);

      newZmin = oldZmin;
    }
  }

  // same with z = Zmax axis
  if (getZaxisIntersections(selectionLines, oldZmax, intersections))
  {
    int nbProjectedIntersections = getProjectedZIntersections(intersections, projectedIntersections,
                                                              oldZmax, oldXmin, oldXmax, oldYmin, oldYmax);
    if (nbProjectedIntersections > 0)
    {
      updateXCoordinate(projectedIntersections, nbProjectedIntersections, oldXmin, oldXmax, newXmin, newXmax);
      updateYCoordinate(projectedIntersections, nbProjectedIntersections, oldYmin, oldYmax, newYmin, newYmax);

      newZmax = oldZmax;
    }
  }

  // check that the view was not outside
  // that would mean that the newBounds were not updated
  if (newXmin >= newXmax && newYmin >= newYmax && newZmin >= newZmax)
  {
    // selection was ousite all this work for nothing
    return false;
  }

  // some of the bounds have been updated, find which ones.
  if (newXmin >= newXmax)
  {
    // no update here
    newXmin = oldXmin;
    newXmax = oldXmax;
  }

  if (newYmin >= newYmax)
  {
    // no update here
    newYmin = oldYmin;
    newYmax = oldYmax;
  }

  if (newZmin >= newZmax)
  {
    // no update here
    newZmin = oldZmin;
    newZmax = oldZmax;
  }


  // ooray we found new bounds
  // switch back to Scilab coordinates
  inversePointScale(newXmin, newYmin, newZmin, &newXmin, &newYmin, &newZmin);
  inversePointScale(newXmax, newYmax, newZmax, &newXmax, &newYmax, &newZmax);
  double newDataBounds[6] = {newXmin, newXmax, newYmin, newYmax, newZmin, newZmax};

  sciSetZoomBox(m_pDrawed, newDataBounds);

  return true;

}
/*--------------------------------------------------------------------------*/
void Camera::getViewingArea(int * xPos, int * yPos, int * width, int * height)
{
  getCameraImp()->getViewingArea(xPos, yPos, width, height);
}
/*--------------------------------------------------------------------------*/
void Camera::getViewport(int viewport[4])
{
  getCameraImp()->getViewport(viewport);
}
/*--------------------------------------------------------------------------*/
void Camera::computeZoomAreaLines(const double areaPixCorners[4][2], double areaLines[4][2][3])
{
  for (int i = 0; i < 4; i++)
  {
    // get two points along the axis line in pixel coordinates
    // in pixel coordinates lines are along Z coordinates
    // so we can specify everything for as Z
    // let say 0 and 1
    double point1[3] = {areaPixCorners[i][0], areaPixCorners[i][1], 0.0};
    double point2[3] = {areaPixCorners[i][0], areaPixCorners[i][1], -1.0};

    // retrieve scene coordinate
    getSceneCoordinates(point1, areaLines[i][0]);
    getSceneCoordinates(point2, areaLines[i][1]);
    pointScale(areaLines[i][0][0], areaLines[i][0][1], areaLines[i][0][2],
               &areaLines[i][0][0], &areaLines[i][0][1], &areaLines[i][0][2]);
    pointScale(areaLines[i][1][0], areaLines[i][1][1], areaLines[i][1][2],
               &areaLines[i][1][0], &areaLines[i][1][1], &areaLines[i][1][2]);
  }
}
/*--------------------------------------------------------------------------*/
bool Camera::getXaxisIntersections(const double areaLines[4][2][3], double planeXCoord, double intersections[4][3])
{
  for (int i = 0; i < 4; i++)
  {
    // for any plane the result of the intersection is is I = (P1 + a.P2) / (a - 1)
    // where P1 and P2 are 2 points on the line
    // and a = ||P1P1'|| / ||P2P2'|| where P1' and P2' are the orthogonal projections
    // of P1 and P2 on the plane

    // It's not needed to care about ||P2P2'|| being 0 with the value we choose for it
    const double * p1 = areaLines[i][0];
    const double * p2 = areaLines[i][1];
    double alpha = (p1[0] - planeXCoord) / (p2[0] - planeXCoord);

    if (alpha == 1.0)
    {
      return false;
    }
    
    getIntersection(p1, p2, alpha, intersections[i]);
  }
  return true;
}
/*--------------------------------------------------------------------------*/
bool Camera::getYaxisIntersections(const double areaLines[4][2][3], double planeYCoord, double intersections[4][3])
{
  for (int i = 0; i < 4; i++)
  {
    // for any plane the result of the intersection is is I = (P1 + a.P2) / (a - 1)
    // where P1 and P2 are 2 points on the line
    // and a = ||P1P1'|| / ||P2P2'|| where P1' and P2' are the orthogonal projections
    // of P1 and P2 on the plane

    // It's not needed to care about ||P2P2'|| being 0 with the value we choose for it
    const double * p1 = areaLines[i][0];
    const double * p2 = areaLines[i][1];
    double alpha = (p1[1] - planeYCoord) / (p2[1] - planeYCoord);

    if (alpha == 1.0)
    {
      return false;
    }
    
    getIntersection(p1, p2, alpha, intersections[i]);
  }
  return true;
}
/*--------------------------------------------------------------------------*/
bool Camera::getZaxisIntersections(const double areaLines[4][2][3], double planeZCoord, double intersections[4][3])
{
  for (int i = 0; i < 4; i++)
  {
    // for any plane the result of the intersection is is I = (a.P2 - P1) / (a - 1)
    // where P1 and P2 are 2 points on the line
    // and a = ||P1P1'|| / ||P2P2'|| where P1' and P2' are the orthogonal projections
    // of P1 and P2 on the plane

    // It's not needed to care about ||P2P2'|| being 0 with the value we choose for it
    const double * p1 = areaLines[i][0];
    const double * p2 = areaLines[i][1];
    double alpha = (p1[2] - planeZCoord) / (p2[2] - planeZCoord);

    if (alpha == 1.0)
    {
      // lines are parallel to the plane
      return false;
    }
    
    getIntersection(p1, p2, alpha, intersections[i]);
  }
  return true;
}
/*--------------------------------------------------------------------------*/
void Camera::getIntersection(const double p1[3], const double p2[3], double alpha, double intersection[3])
{
  scalarMult3D(p2, alpha, intersection); // I = a.P2
  vectSubstract3D(intersection, p1, intersection); // I = a.P2 - P1
  scalarMult3D(intersection, 1.0 / (alpha - 1.0), intersection); // I = (P1 + a.P2) / (a - 1)
}
/*--------------------------------------------------------------------------*/
void Camera::updateXCoordinate(const double intersections[][3],
                               int nbIntersections,
                               double oldXmin, double oldXmax,
                               double & newXmin, double & newXmax)
{
  for (int i = 0; i < nbIntersections; i++)
  {
    if (intersections[i][0] < newXmin)
    {
      // it's a zoom don't set bounds outside of old ones
      newXmin = Max(intersections[i][0], oldXmin);
    }
    else if (intersections[i][0] > newXmax)
    {
      // it's a zoom don't set bounds outside of old ones
      newXmax = Min(intersections[i][0], oldXmax);
    }
  }
}
/*--------------------------------------------------------------------------*/
void Camera::updateYCoordinate(const double intersections[][3],
                               int nbIntersections,
                               double oldYmin, double oldYmax,
                               double & newYmin, double & newYmax)
{
  for (int i = 0; i < nbIntersections; i++)
  {
    if (intersections[i][1] < newYmin)
    {
      // it's a zoom don't set bounds outside of old ones
      newYmin = Max(intersections[i][1], oldYmin);
    }
    else if (intersections[i][1] > newYmax)
    {
      // it's a zoom don't set bounds outside of old ones
      newYmax = Min(intersections[i][1], oldYmax);
    }
  }
}
/*--------------------------------------------------------------------------*/
void Camera::updateZCoordinate(const double intersections[][3],
                               int nbIntersections,
                               double oldZmin, double oldZmax,
                               double & newZmin, double & newZmax)
{
  for (int i = 0; i < nbIntersections; i++)
  {
    if (intersections[i][2] < newZmin)
    {
      // it's a zoom don't set bounds outside of old ones
      newZmin = Max(intersections[i][2], oldZmin);
    }
    else if (intersections[i][2] > newZmax)
    {
      // it's a zoom don't set bounds outside of old ones
      newZmax = Min(intersections[i][2], oldZmax);
    }
  }
}
/*--------------------------------------------------------------------------*/
int Camera::getProjectedXIntersections(const double intersections[4][3], double projectedIntersections[8][3],
                                       double planeXCoord,
                                       double yMin, double yMax, double zMin, double zMax)
{
  int nbIntersections = 0;
  double intersections2D[4][2];
  double projectedIntersections2D[8][2];

  // project on the plane
  for (int i = 0; i < 4; i++)
  {
    projectOnXPlane(intersections[i], intersections2D[i]);
  }

  // get intersections on the plane
  nbIntersections = getProjectedIntersections2D(intersections2D, projectedIntersections2D,
                                                yMin, yMax, zMin, zMax);

  // project back
  for (int i = 0; i < nbIntersections; i++)
  {
    unProjectOnXPlane(projectedIntersections2D[i], projectedIntersections[i], planeXCoord);
  }
  return nbIntersections;
}
/*--------------------------------------------------------------------------*/
int Camera::getProjectedYIntersections(const double intersections[4][3], double projectedIntersections[8][3],
                                       double planeYCoord,
                                       double xMin, double xMax, double zMin, double zMax)
{
  int nbIntersections = 0;
  double intersections2D[4][2];
  double projectedIntersections2D[8][2];

  // project on the plane
  for (int i = 0; i < 4; i++)
  {
    projectOnYPlane(intersections[i], intersections2D[i]);
  }

  // get intersections on the plane
  nbIntersections = getProjectedIntersections2D(intersections2D, projectedIntersections2D,
                                                xMin, xMax, zMin, zMax);

  // project back
  for (int i = 0; i < nbIntersections; i++)
  {
    unProjectOnYPlane(projectedIntersections2D[i], projectedIntersections[i], planeYCoord);
  }
  return nbIntersections;
}
/*--------------------------------------------------------------------------*/
int Camera::getProjectedZIntersections(const double intersections[4][3], double projectedIntersections[8][3],
                                       double planeZCoord,
                                       double xMin, double xMax, double yMin, double yMax)
{
  int nbIntersections = 0;
  double intersections2D[4][2];
  double projectedIntersections2D[8][2];

  // project on the plane
  for (int i = 0; i < 4; i++)
  {
    projectOnZPlane(intersections[i], intersections2D[i]);
  }

  // get intersections on the plane
  nbIntersections = getProjectedIntersections2D(intersections2D, projectedIntersections2D,
                                                xMin, xMax, yMin, yMax);

  // project back
  for (int i = 0; i < nbIntersections; i++)
  {
    unProjectOnZPlane(projectedIntersections2D[i], projectedIntersections[i], planeZCoord);
  }
  return nbIntersections;
}
/*--------------------------------------------------------------------------*/
void Camera::projectOnXPlane(const double point[3], double proj[2])
{
  // only Y and Z coordinates are useful
  proj[0] = point[1];
  proj[1] = point[2];
}
/*--------------------------------------------------------------------------*/
void Camera::unProjectOnXPlane(const double proj[2], double point[3], double planeXCoord)
{
  point[0] = planeXCoord;
  point[1] = proj[0];
  point[2] = proj[1];
}
/*--------------------------------------------------------------------------*/
void Camera::projectOnYPlane(const double point[3], double proj[2])
{
  // only X and Z coordinates are useful
  proj[0] = point[0];
  proj[1] = point[2];
}
/*--------------------------------------------------------------------------*/
void Camera::unProjectOnYPlane(const double proj[2], double point[3], double planeYCoord)
{
  point[0] = proj[0];
  point[1] = planeYCoord;
  point[2] = proj[1];
}
/*--------------------------------------------------------------------------*/
void Camera::projectOnZPlane(const double point[3], double proj[2])
{
  // only X and Y coordinates are useful
  proj[0] = point[0];
  proj[1] = point[1];
}
/*--------------------------------------------------------------------------*/
void Camera::unProjectOnZPlane(const double proj[2], double point[3], double planeZCoord)
{
  point[0] = proj[0];
  point[1] = proj[1];
  point[2] = planeZCoord;
}
/*--------------------------------------------------------------------------*/
int Camera::getProjectedIntersections2D(const double intersections[4][2], double projectedIntersections[8][2],
                                        double xMin, double xMax, double yMin, double yMax)
{
  int nbIntersections = 0;
  for (int i = 0; i < 4; i++)
  {
    if (computeLineFillRectangleIntersections(intersections[i],
                                              intersections[(i + 1) %4],
                                              xMin, xMax, yMin, yMax,
                                              &(projectedIntersections[nbIntersections])))
    {
      // intersection found
      nbIntersections += 2;
    }
  }

  if (nbIntersections == 2)
  {
    // only one edge has been crossed
    // this means that part of the rectangle is inside the quadrangle
    // Two points af the rectangle are inside
    // we need to add them to the intersections
    double rectangleCorner[2];

    // first point
    rectangleCorner[0] = xMin;
    rectangleCorner[1] = yMin;
    if (isPointInQuadrangle(rectangleCorner, intersections))
    {
      // add it
      projectedIntersections[nbIntersections][0] = rectangleCorner[0];
      projectedIntersections[nbIntersections][1] = rectangleCorner[1];
      nbIntersections++;
    }

    // seconf point
    rectangleCorner[0] = xMin;
    rectangleCorner[1] = yMax;
    if (isPointInQuadrangle(rectangleCorner, intersections))
    {
      // add it
      projectedIntersections[nbIntersections][0] = rectangleCorner[0];
      projectedIntersections[nbIntersections][1] = rectangleCorner[1];
      nbIntersections++;
    }

    // first point
    rectangleCorner[0] = xMax;
    rectangleCorner[1] = yMin;
    if (isPointInQuadrangle(rectangleCorner, intersections))
    {
      // add it
      projectedIntersections[nbIntersections][0] = rectangleCorner[0];
      projectedIntersections[nbIntersections][1] = rectangleCorner[1];
      nbIntersections++;
    }

    // first point
    rectangleCorner[0] = xMax;
    rectangleCorner[1] = yMax;
    if (isPointInQuadrangle(rectangleCorner, intersections))
    {
      // add it
      projectedIntersections[nbIntersections][0] = rectangleCorner[0];
      projectedIntersections[nbIntersections][1] = rectangleCorner[1];
      nbIntersections++;
    }
  }

  return nbIntersections;
}
/*--------------------------------------------------------------------------*/
bool Camera::computeLineFillRectangleIntersections(const double p1[2], const double p2[2],
                                                   double xMin, double xMax, double yMin, double yMax,
                                                   double intersections[2][2])
{
  // compute the number of intersections with the rectangle (just boundary not inside)
  int nbIntersectionWithRectangle
    = computeLineRectangleIntersections(p1, p2, xMin, xMax, yMin, yMax, intersections);

  if (nbIntersectionWithRectangle == 1)
  {
    // one of the point lies inside the rectangle
    if (isInsideRectangle(p1, xMin, xMax, yMin, yMax))
    {
      // p1 is inside
      intersections[1][0] = p1[0];
      intersections[1][1] = p1[1];
    }
    else
    {
      // p2 is inside
      intersections[1][0] = p2[0];
      intersections[1][1] = p2[1];
    }
  }
  else if (nbIntersectionWithRectangle == 0)
  {
    // both of the points lies inside the rectangle or any
    if (isInsideRectangle(p1, xMin, xMax, yMin, yMax))
    {
      // p1 and p2 are inside
      intersections[0][0] = p1[0];
      intersections[0][1] = p1[1];
      intersections[1][0] = p2[0];
      intersections[1][1] = p2[1];
    }
    else
    {
      // no intersections
      return false;
    }
  }

  // if the two intersections have been found, just return them
  return true;
}
/*--------------------------------------------------------------------------*/
bool Camera::isInsideRectangle(const double p[2], double xMin, double xMax, double yMin,
                               double yMax)
{
  return (p[0] >= xMin && p[0] <= xMax && p[1] >= yMin && p[1] <= yMax);
}
/*--------------------------------------------------------------------------*/
int Camera::computeLineRectangleIntersections(const double p1[2], const double p2[2],
                                              double xMin, double xMax, double yMin, double yMax,
                                              double intersections[2][2])
{
  int nbIntersections = 0;
  
  // check intersection with x = xMin
  // Let's I be the intersection of p1, p2 with x = xMin
  // I lies on (p1,p2), so I = p1 + a.p1p2
  // then with a projection on x, Ix = p1x + a.(p2x - p1x) = xMin.
  // So a = (xMin - p1x) / (p2x - p1x), if p2x != p1x
  // Consequnently, I is on the rectangle if p2x != p1x and 0 < a < 1
  // so that I is between A and B and also that Iy is between yMin and yMax
  if (p1[0] != p2[0])
  {
    if (checkXIntersection(p1, p2, intersections[nbIntersections],
                           xMin, yMin, yMax))
    {
      nbIntersections++;
    }

    // same for x = xMax
    if (checkXIntersection(p1, p2, intersections[nbIntersections],
                           xMax, yMin, yMax))
    {
      nbIntersections++;
    }

  }

  // same for Y axis
  if (p1[1] != p2[1])
  {
    if (checkYIntersection(p1, p2, intersections[nbIntersections],
                           yMin, xMin, xMax))
    {
      nbIntersections++;
    }

    // same for x = xMax
    if (checkYIntersection(p1, p2, intersections[nbIntersections],
                           yMax, xMin, xMax))
    {
      nbIntersections++;
    }

  }

  return nbIntersections;
}
/*--------------------------------------------------------------------------*/
bool Camera::checkXIntersection(const double p1[2], const double p2[2], double res[2],
                                double rectXCoord, double yMin, double yMax)
{
  double a = (rectXCoord - p1[0]) / (p2[0] - p1[0]);

  if ( a <= 0.0 || a >= 1.0)
  {
    // Intersection is not between p1 and p2
    return false;
  }

  // compute res = p1 + a.p1p2
  p1PlusAP1P2(p1, p2, a, res);

  // check that the intersection is between yMin and yMax
  if (res[1] < yMin || res[1] > yMax)
  {
    return false;
  }

  return true;

}
/*--------------------------------------------------------------------------*/
bool Camera::checkYIntersection(const double p1[2], const double p2[2], double res[2],
                                double rectYCoord, double xMin, double xMax)
{
  double a = (rectYCoord - p1[1]) / (p2[1] - p1[1]);

  if ( a <= 0.0 || a >= 1.0)
  {
    // Intersection is not between p1 and p2
    return false;
  }

  // compute res = p1 + a.p1p2
  p1PlusAP1P2(p1, p2, a, res);

  // check that the intersection is between yMin and yMax
  if (res[0] < xMin || res[0] > xMax)
  {
    return false;
  }

  return true;

}
/*--------------------------------------------------------------------------*/
void Camera::p1PlusAP1P2(const double p1[2], const double p2[2], double a, double res[2])
{
  // compute res = p1 + a.p1p2
  vectSubstract2D(p2, p1, res); // res = p1p2
  scalarMult2D(res, a, res); // res = a.p1p2
  vectAdd2D(res, p1, res); // res = p1 + a.p1p2
}
/*--------------------------------------------------------------------------*/
bool Camera::isPointInQuadrangle(const double point[2], const double corners[4][2])
{
  // decompose the quad into two triangles and find if the point
  // is in one or the other
  return (   isPointInTriangle(point, corners[0], corners[1], corners[2])
          || isPointInTriangle(point, corners[1], corners[2], corners[3]));
}
/*--------------------------------------------------------------------------*/
void Camera::visualizeZoomingArea(const double intersections[4][2][3])
{
  double intersectionsView[4][2][3];

  for (int i = 0; i < 4; i++)
  {
    double lineDir[3];
    vectSubstract3D(intersections[i][0], intersections[i][1], lineDir);
    scalarMult3D(lineDir, 5.0, lineDir);

    double linePoint1[3];
    vectAdd3D(intersections[i][0], lineDir, linePoint1);

    scalarMult3D(lineDir, -1.0, lineDir);

    double linePoint2[3];
    vectAdd3D(intersections[i][0], lineDir, linePoint2);

    for (int j = 0; j < 3; j++)
    {
      intersectionsView[i][0][j] = linePoint1[j];
      intersectionsView[i][1][j] = linePoint2[j];
    }
  }

  int color = 0;
  // draw viewing area
  for (int i = 0; i < 4; i++)
  {
    
    color++;

    double xCoords[4] = {intersectionsView[i][0][0], intersectionsView[i][1][0],
                         intersectionsView[(i + 1)%4][1][0], intersectionsView[(i+1)%4][0][0]};
    double yCoords[4] = {intersectionsView[i][0][1], intersectionsView[i][1][1],
                         intersectionsView[(i + 1)%4][1][1], intersectionsView[(i+1)%4][0][1]};
    double zCoords[4] = {intersectionsView[i][0][2], intersectionsView[i][1][2],
                         intersectionsView[(i + 1)%4][1][2], intersectionsView[(i+1)%4][0][2]};
    sciPointObj * polyline = ConstructPolyline(m_pDrawed, xCoords, yCoords, zCoords,
                                               FALSE, 4, 1, &color, &color, &color,
                                               &color, &color, FALSE, TRUE, FALSE,
                                               FALSE);
    // clipgrf
    sciSetIsClipping(polyline, 0);
    

  }
}
/*--------------------------------------------------------------------------*/
void Camera::visualizeIntersection(const double intersections[4][3])
{
  int color = 0;
  int mark = 0;
  double xCoords[4];
  double yCoords[4];
  double zCoords[4];
  for (int i = 0; i < 4; i++)
  {
    xCoords[i] = intersections[i][0];
    yCoords[i] = intersections[i][1];
    zCoords[i] = intersections[i][2];
  }

  sciPointObj * polyline = ConstructPolyline(m_pDrawed, xCoords, yCoords, zCoords,
                                             TRUE, 4, 1, &color, &color, &mark,
                                             &color, &color, TRUE, FALSE, TRUE,
                                             FALSE);
  // clipgrf
  sciSetIsClipping(polyline, -1);

}
/*--------------------------------------------------------------------------*/
CameraBridge * Camera::getCameraImp( void )
{
  return dynamic_cast<CameraBridge *>(m_pImp);
}
/*--------------------------------------------------------------------------*/
}
