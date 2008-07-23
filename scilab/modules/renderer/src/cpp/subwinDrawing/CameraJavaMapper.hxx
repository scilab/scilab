/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by Camera   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CAMERA_JAVA_MAPPER_HXX_
#define _CAMERA_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "IsoViewCameraGL.hxx"

namespace sciGraphics
{

class CameraJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  CameraJavaMapper( void ) {}

  virtual ~CameraJavaMapper( void ) {}

  /*----------------------------------------------------------------------*/
  // specific for cameras
  virtual void setViewingArea(double transX, double transY, double scaleX, double scaleY) = 0;

  virtual void setNormalizationParameters(double scaleX, double scaleY, double scaleZ,
                                          double transX, double transY, double transZ) = 0;

  virtual void setAxesRotationParameters(double centerX, double centerY, double centerZ,
                                         double alpha, double theta) = 0;

  virtual void setFittingScale(double scaleX, double scaleY, double scaleZ) = 0;

  virtual void placeCamera(void) = 0;

  virtual void redrawCamera(void) = 0;

  virtual void replaceCamera( void ) = 0;

  virtual void setAxesReverse(bool xAxisRevert, bool yAxisRevert, bool zAxisRevert) = 0;

  virtual void getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]) = 0;
  virtual void get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]) = 0;

  virtual void getProjectionMatrix(double mat[4][4]) = 0;
  virtual void getUnprojectMatrix(double mat[4][4]) = 0;
  virtual void get2dViewProjectionMatrix(double mat[4][4]) = 0;
  virtual void get2dViewUnprojectMatrix(double mat[4][4]) = 0;
  virtual void getViewPort(double viewPort[4]) = 0;
  virtual void setAxesBounds(double xMin, double xMax,
	         	     double yMin, double yMax,
			     double zMin, double zMax) = 0;
  /*----------------------------------------------------------------------*/
  void convertMatrixFormat(const double src[16], double dest[4][4]) {
    for (int i = 0; i < 4; i++) {
      dest[i][0] = src[4 * i];
      dest[i][1] = src[4 * i + 1];
      dest[i][2] = src[4 * i + 2];
      dest[i][3] = src[4 * i + 3];
    }
  }
  /*----------------------------------------------------------------------*/


};

}

#endif /* _CAMERA_JAVA_MAPPER_HXX_ */

