/*------------------------------------------------------------------------*/
/* file: CameraJoGL.h                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : JoGL class to draw a camera                                     */
/*------------------------------------------------------------------------*/

#ifndef _CAMERA_JOGL_H_
#define _CAMERA_JOGL_H_

#include "../DrawableObjectJoGL.h"
#include "CameraBridge.h"
#include "DrawableSubwin.h"
#include "CameraJavaMapper.hxx"

namespace sciGraphics
{

class DrawableSubwin;

class CameraJoGL : public CameraBridge, public DrawableObjectJoGL
{

public:

  /**
   * Default constructor
   * @param subwin Subwindow containign the camera
   */
  CameraJoGL( Camera * camera ) ;

  /**
   * Position the view and view area accordingly to previous calls.
   */
  virtual void renderPosition( void ) ;

  /**
   * Put back camera to default position.
   */
  virtual void replaceCamera( void );

  /**
   * Convert user coordinates to pixel coordinates.
   */
  virtual void getPixelCoordinates(const double userCoord[3], int pixCoord[2]);

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  virtual void get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2]);

  /**
   * Find coordinates in 2D view from pixel ones
   */
  virtual void get2dViewCoordinates(const int pixCoords[2], double userCoord2D[2]);

protected:

  /*----------------------------------------------------------------*/

  /**
   * Get the projection of a point on the canvas.
   */
  void project(const double projMatrix[4][4], const double viewPort[4],
               const double sceneCoordinate[3], double pixCoordinate[3]);

  /**
   * Retrieve the position of a point from its coordinates on the canvas.
   */
  void unProject(double unProjMatrix[4][4], double viewPort[4],
                 const double pixCoordinates[3], double sceneCoordinates[3]);

  /**
   * Get the object performing mapping with Java class.
   */
  CameraJavaMapper * getCameraJavaMapper(void);
  /*----------------------------------------------------------------*/
  /** Projection matrix in current coordinates frame */
  double m_aProjMatrix3D[4][4];
  /** inverse of projection matrix in current coordinates frame */
  double m_aUnprojMatrix3D[4][4];

  /** Projection matrix fro 2D projection */
  double m_aProjMatrix2D[4][4];
  /** Inverse of projMatrix2D */
  double m_aUnprojMatrix2D[4][4];

  /** Current viewPort */
  double m_aViewPort[4];
  /*----------------------------------------------------------------*/

};

}

#endif /* _CAMERA_JOGL_H_ */
