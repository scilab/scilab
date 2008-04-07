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
    double minScale = Min(scale[0], Min(scale[1], scale[2]));
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
void Camera::get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  getCameraImp()->get2dViewPixelCoordinates(userCoord, pixCoord);
}
/*--------------------------------------------------------------------------*/
void Camera::getSceneCoordinates(const double pixCoords[3], double userCoords[3])
{
  getCameraImp()->getSceneCoordinates(pixCoords, userCoords);
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
void Camera::draw( void )
{
  initializeDrawing();
  setCameraParameters();
  getCameraImp()->renderPosition();
  endDrawing();
}
/*--------------------------------------------------------------------------*/
void Camera::show( void )
{
  initializeDrawing();
  getCameraImp()->show();
  endDrawing();
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

  // intersection with x = Xmin axis
  if (   getXaxisIntersections(selectionLines, oldXmin, intersections)
      && checkXIntersections(intersections, oldYmin, oldYmax, oldZmin, oldZmax))
  {
    // ok we found points and the selection intersect the side of the cube
    // update Y and Z coordinates
    // don't try to update X here, it's just xMin
    updateYCoordinate(intersections, oldYmin, oldYmax, newYmin, newYmax);
    updateZCoordinate(intersections, oldZmin, oldZmax, newZmin, newZmax);

    newXmin = oldXmin;
  }

 

  // same with x = Xmax axis
  if (   getXaxisIntersections(selectionLines, oldXmax, intersections)
      && checkXIntersections(intersections, oldYmin, oldYmax, oldZmin, oldZmax))
  {
    updateYCoordinate(intersections, oldYmin, oldYmax, newYmin, newYmax);
    updateZCoordinate(intersections, oldZmin, oldZmax, newZmin, newZmax);

    newXmax = newXmax;
  }

  // same with y = Ymin axis
  if (   getYaxisIntersections(selectionLines, oldYmin, intersections)
      && checkYIntersections(intersections, oldXmin, oldXmax, oldZmin, oldZmax))
  {
    updateXCoordinate(intersections, oldXmin, oldXmax, newXmin, newXmax);
    updateZCoordinate(intersections, oldZmin, oldZmax, newZmin, newZmax);

    newYmin = oldYmin;
  }

  // same with y = Ymax axis
  if (   getYaxisIntersections(selectionLines, oldYmax, intersections)
      && checkYIntersections(intersections, oldXmin, oldXmax, oldZmin, oldZmax))
  {
    updateXCoordinate(intersections, oldXmin, oldXmax, newXmin, newXmax);
    updateZCoordinate(intersections, oldZmin, oldZmax, newZmin, newZmax);

    newYmax = oldYmax;
  }

  // same with z = Zmin axis
  if (   getZaxisIntersections(selectionLines, oldZmin, intersections)
      && checkZIntersections(intersections, oldXmin, oldXmax, oldYmin, oldYmax))
  {
    updateXCoordinate(intersections, oldXmin, oldXmax, newXmin, newXmax);
    updateYCoordinate(intersections, oldYmin, oldYmax, newYmin, newYmax);

    newZmin = oldZmin;
  }

  // same with z = Zmax axis
  if (   getZaxisIntersections(selectionLines, oldZmax, intersections)
      && checkZIntersections(intersections, oldXmin, oldXmax, oldYmin, oldYmax))
  {
    updateXCoordinate(intersections, oldXmin, oldXmax, newXmin, newXmax);
    updateYCoordinate(intersections, oldYmin, oldYmax, newYmin, newYmax);

    newZmax = oldZmax;
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
void Camera::updateXCoordinate(const double intersections[4][3],
                               double oldXmin, double oldXmax,
                               double & newXmin, double & newXmax)
{
  for (int i = 0; i < 4; i++)
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
void Camera::updateYCoordinate(const double intersections[4][3],
                               double oldYmin, double oldYmax,
                               double & newYmin, double & newYmax)
{
  for (int i = 0; i < 4; i++)
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
void Camera::updateZCoordinate(const double intersections[4][3],
                               double oldZmin, double oldZmax,
                               double & newZmin, double & newZmax)
{
  for (int i = 0; i < 4; i++)
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
bool Camera::checkXIntersections(const double intersections[4][3],
                                 double oldYmin, double oldYmax,
                                 double oldZmin, double oldZmax)
{
  for (int i = 0; i < 4; i++)
  {
    if (   intersections[i][1] >= oldYmin && intersections[i][1] <= oldYmax
        && intersections[i][2] >= oldZmin && intersections[i][2] <= oldZmax)
    {
      // at least one point in the side
      return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------*/
bool Camera::checkYIntersections(const double intersections[4][3],
                                 double oldXmin, double oldXmax,
                                 double oldZmin, double oldZmax)
{
  for (int i = 0; i < 4; i++)
  {
    if (   intersections[i][0] >= oldXmin && intersections[i][0] <= oldXmax
        && intersections[i][2] >= oldZmin && intersections[i][2] <= oldZmax)
    {
      // at least one point in the side
      return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------*/
bool Camera::checkZIntersections(const double intersections[4][3],
                                 double oldXmin, double oldXmax,
                                 double oldYmin, double oldYmax)
{
  for (int i = 0; i < 4; i++)
  {
    if (   intersections[i][0] >= oldXmin && intersections[i][0] <= oldXmax
        && intersections[i][1] >= oldYmin && intersections[i][1] <= oldYmax)
    {
      // at least one point in the side
      return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------*/
CameraBridge * Camera::getCameraImp( void )
{
  return dynamic_cast<CameraBridge *>(m_pImp);
}
/*--------------------------------------------------------------------------*/
}
