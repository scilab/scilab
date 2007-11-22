/*------------------------------------------------------------------------*/
/* file: CameraJavaMapper.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by Camera                  */
/*------------------------------------------------------------------------*/

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

  virtual void replaceCamera( void ) = 0;

  virtual void revertXAxis(void) = 0;
  virtual void revertYAxis(void) = 0;
  virtual void revertZAxis(void) = 0;

  virtual void getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]) = 0;
  virtual void get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]) = 0;
  /*----------------------------------------------------------------------*/


};

}

#endif /* _CAMERA_JAVA_MAPPER_HXX_ */

