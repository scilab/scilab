/*------------------------------------------------------------------------*/
/* file: CameraJavaMapper.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by Camera                  */
/*------------------------------------------------------------------------*/

#ifndef _CAMERA_JAVA_MAPPER_HXX_
#define _CAMERA_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "CameraGL.hxx"

namespace sciGraphics
{

class CameraJavaMapper : public virtual  DrawableObjectJavaMapper
{
public:
  
  CameraJavaMapper( void ) ;

  virtual ~CameraJavaMapper( void ) ;

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
  virtual void moveViewingArea(double transX, double transY, double scaleX, double scaleY);

  virtual void moveAxesBox(double scaleX, double scaleY, double scaleZ,
                           double transX, double transY, double transZ);

  virtual void rotateAxesBox(double centerX, double centerY, double centerZ,
                             double alpha, double theta, double reductionRatio) ;

  virtual int * getScreenCoordinates(double userCoordX, double userCoordY, double userCoordZ);
  /*----------------------------------------------------------------------*/

private:
  
  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::CameraGL * m_pJavaObject;

};

}

#endif /* _CAMERA_JAVA_MAPPER_HXX_ */

