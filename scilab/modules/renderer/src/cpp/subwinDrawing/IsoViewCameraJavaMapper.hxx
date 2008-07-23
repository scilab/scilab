/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by Camera for isoview renderering
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _ISOVIEW_CAMERA_JAVA_MAPPER_HXX_
#define _ISOVIEW_CAMERA_JAVA_MAPPER_HXX_

#include "CameraJavaMapper.hxx"
#include "IsoViewCameraGL.hxx"

namespace sciGraphics
{

class IsoViewCameraJavaMapper : public virtual CameraJavaMapper
{
public:
  
  IsoViewCameraJavaMapper( void ) ;

  virtual ~IsoViewCameraJavaMapper( void ) ;

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

  virtual void redrawCamera(void);

  virtual void replaceCamera( void );

  virtual void setAxesReverse(bool xAxisRevert, bool yAxisRevert, bool zAxisRevert);

  virtual void getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]);
  virtual void get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ, int pixCoord[2]);

  virtual void getProjectionMatrix(double mat[4][4]);
  virtual void getUnprojectMatrix(double mat[4][4]);
  virtual void get2dViewProjectionMatrix(double mat[4][4]);
  virtual void get2dViewUnprojectMatrix(double mat[4][4]);
  virtual void getViewPort(double viewPort[4]);
  virtual void setAxesBounds(double xMin, double xMax,
	         	     double yMin, double yMax,
			     double zMin, double zMax);
  /*----------------------------------------------------------------------*/

private:
  
  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::IsoViewCameraGL * m_pJavaObject;

};

}

#endif /* _ISOVIEW_CAMERA_JAVA_MAPPER_HXX_ */

