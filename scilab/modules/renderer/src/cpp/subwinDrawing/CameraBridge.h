/*------------------------------------------------------------------------*/
/* file: CameraBridge.h                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Bridge between camera class and impelementation                 */
/*------------------------------------------------------------------------*/

#ifndef _CAMERA_BRIDGE_H_
#define _CAMERA_BRIDGE_H_

#include "DrawableObject.h"

namespace sciGraphics
{

class CameraBridge
{
public:

  /**
   * Default constructor
   * @param subwin Subwindow containign the camera
   */
  CameraBridge( void ) {}

  virtual ~CameraBridge( void ) {}

  /**
   * Set the viewing position o
   */
  void setViewingArea( double translation[2], double scale[2] ) ;

  /**
  * Set the rotation angles of the camera.
   */
  void setRotationAngles( double alpha, double theta ) ;

  /**
   * Specify the axes box to render.
   */
  void setSubWinBox( double box[6] ) ;

  /**
   * Specify the farthest distance from the camera in order to specify zNear and zFar
   */
  void setFarthestDistance( double maxDist ) ;

  /**
   *  Set the bouding box of axes (the one drawn on the screen)
   */
  void setAxesBox( double bbox[6] ) ;

  /**
   * Position the view and view area accordingly to previous calls.
   */
  virtual void renderPosition( void ) = 0 ;

protected:

  /**
  * Translation between the origin of rendering canvas and the current viewing area.
  * Should be positive since the view is inside the canvas.
  */
  double m_aViewingTranslation[2];

  /**
  * Scaling between the rendering canvas and current viewing area.
  * Should be lower than 1 since the viewing area is smaller than the canvas.
  */
  double m_aViewingScale[2];

  /** Rotation angle around axe Z */
  double m_dTheta;
  /** Rotation around axe Y */
  double m_dAlpha;

  /** specify the axis box */
  double m_aAxesBox[6];

  /** maximum found distance between a graphic object and the viewpoint */
  double m_dFarthestDist;

};

}

#endif /* _CAMERA_BRIDGE_H_ */