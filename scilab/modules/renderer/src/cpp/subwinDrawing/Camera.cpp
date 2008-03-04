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
void Camera::zoomRect(int posX, int posY, int width, int height)
{
  double rectCorners[4][2] = {{posX        , posY + height},
                              {posX        , posY         },
                              {posX + width, posY         },
                              {posX + width, posY + height}};

  zoomRect(rectCorners);

}
/*------------------------------------------------------------------------------------------*/
void Camera::zoomRect(const double corners[4][2])
{

  double oldDataBounds[6];
  double newDataBounds[6];

  sciGetRealDataBounds(m_pDrawed, oldDataBounds);
  
  // x axis

  // (xmin, ymin zmin)
  double xAxisBound1[3] = {oldDataBounds[0], oldDataBounds[2], oldDataBounds[4]};
  // (xmax, ymin zmin)
  double xAxisBound2[3] = {oldDataBounds[1], oldDataBounds[2], oldDataBounds[4]};

  getNewBounds(corners, xAxisBound1, xAxisBound2, oldDataBounds[0], oldDataBounds[1],
               &(newDataBounds[0]), &(newDataBounds[1]));


  // y axis

  // (xmin, ymin zmin)
  double yAxisBound1[3] = {oldDataBounds[0], oldDataBounds[2], oldDataBounds[4]};
  // (xmin, ymax zmin)
  double yAxisBound2[3] = {oldDataBounds[0], oldDataBounds[3], oldDataBounds[4]};

  getNewBounds(corners, yAxisBound1, yAxisBound2, oldDataBounds[2], oldDataBounds[3],
              &(newDataBounds[2]), &(newDataBounds[3]));

  // z axis

  // (xmin, ymin zmin)
  double zAxisBound1[3] = {oldDataBounds[0], oldDataBounds[2], oldDataBounds[4]};
  // (xmin, ymin zmax)
  double zAxisBound2[3] = {oldDataBounds[0], oldDataBounds[2], oldDataBounds[5]};

  getNewBounds(corners, zAxisBound1, zAxisBound2, oldDataBounds[4], oldDataBounds[5],
               &(newDataBounds[4]), &(newDataBounds[5]));

  sciSetZoomBox(m_pDrawed, newDataBounds);

}
/*--------------------------------------------------------------------------*/
void Camera::getViewingArea(int * xPos, int * yPos, int * width, int * height)
{
  getCameraImp()->getViewingArea(xPos, yPos, width, height);
}
/*------------------------------------------------------------------------------------------*/
void Camera::getNewBounds(const double corners[4][2], const double axisPoint1[3], const double axisPoint2[3],
                          double oldMinBound, double oldMaxBound, double * newMinBound, double * newMaxBound)
{
  // convert axis points into pixels
  
  int pixPoint[2];
  getPixelCoordinates(axisPoint1, pixPoint);
  double pixPoint1[2] = {pixPoint[0], pixPoint[1]};

  getPixelCoordinates(axisPoint2, pixPoint);
  double pixPoint2[2] = {pixPoint[0], pixPoint[1]};

  // compute the distance between the two points
  double p1p2[2];
  vectSubstract2D(pixPoint2, pixPoint1, p1p2);
  if (NORM_2D(p1p2) < 5.0)
  {
    // axes too short for projection
    *newMinBound = oldMinBound;
    *newMaxBound = oldMaxBound;
    return;
  }

  getNewBoundsPix(corners, pixPoint1, pixPoint2, oldMinBound, oldMaxBound, newMinBound, newMaxBound);

}
/*------------------------------------------------------------------------------------------*/
void Camera::getNewBoundsPix(const double corners[4][2], const double axisPoint1[2], const double axisPoint2[2],
                             double oldMinBound, double oldMaxBound, double * newMinBound, double * newMaxBound)
{
  double axisVector[2];
  vectSubstract2D(axisPoint2, axisPoint1, axisVector);

  // get the projection of the four pixels on the axis
  // for each compute its abscissa along the axis
  // get the two extreme as new bounds
  *newMinBound = oldMaxBound;
  *newMaxBound = oldMinBound;
  for (int i = 0; i < 4; i++)
  {
    double point1Corner[2];
    vectSubstract2D(corners[i], axisPoint1, point1Corner);
    // get the projection between 0 and 1
    double proj = DOT_PROD_2D(axisVector, point1Corner) / SQUARE_NORM_2D(axisVector);

    // then find its abscissa along the axis
    double abscissa = (oldMaxBound - oldMinBound) * proj + oldMinBound;
    
    if (abscissa > *newMaxBound) { *newMaxBound = abscissa ;}
    if (abscissa < *newMinBound) { *newMinBound = abscissa ;}
  }
}
/*--------------------------------------------------------------------------*/
CameraBridge * Camera::getCameraImp( void )
{
  return dynamic_cast<CameraBridge *>(m_pImp);
}
/*--------------------------------------------------------------------------*/
}
