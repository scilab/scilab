/*------------------------------------------------------------------------*/
/* file: IsometricCameraJavaMapper.hxx                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by Camera                  */
/*------------------------------------------------------------------------*/

#ifndef _ISOMETRIC_CAMERA_JAVA_MAPPER_HXX_
#define _ISOMETRIC_CAMERA_JAVA_MAPPER_HXX_

#include "CameraJavaMapper.hxx"
#include "IsometricCameraGL.hxx"

namespace sciGraphics
{

class IsometricCameraJavaMapper : public virtual CameraJavaMapper
{
public:

  IsometricCameraJavaMapper( void ) ;

  virtual ~IsometricCameraJavaMapper( void ) ;

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for cameras
  virtual void setViewingArea(double transX, double transY, double scaleX, double scaleY);

  virtual void setNormalizationParameters(double scaleX, double scaleY, double scaleZ,
    double transX, double transY, double transZ);

  virtual void setAxesRotationParameters(double centerX, double centerY, double centerZ,
    double alpha, double theta);

  virtual void setFittingScale(double scaleX, double scaleY, double scaleZ);

  virtual void placeCamera(void);

  virtual void replaceCamera( void );

  virtual void revertXAxis(void);
  virtual void revertYAxis(void);
  virtual void revertZAxis(void);

  virtual void getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]);
  virtual void get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]);
  /*----------------------------------------------------------------------*/

private:

  /**
  * Giws generated wrapper
  */
  org_scilab_modules_renderer_subwinDrawing::IsometricCameraGL * m_pJavaObject;

};

}

#endif /* _ISOMETRIC_CAMERA_JAVA_MAPPER_HXX_ */

