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
   * Set the scale to fit with the subwin data bounds
   */
  void setAxesScale( double scale[3] ) ;

  /**
   * Set the translation to put the axes into view
   */
  void setAxesTranslation( double translation[3] ) ;

  /**
   * Set the rotation of axes in accordance with viewing angles
   * @param alpha rotation around Y axis
   * @param theta rotation around X axis
   */
  void setAxesRotation( double alpha, double theta ) ;

  /**
   * Set the center of the axes around which the box turns.
   */
  void setAxesCenter( double center[3] ) ;

  /**
   * Specify the farthest distance from the camera in order to specify zNear and zFar
   */
  void setFarthestDistance( double maxDist ) ;

  /**
   * Sepecify the ration used to fit the graphic in the window
   */
  void setFitWindowRation( double ratio ) ;

  /**
   * Convert user coordinates to pixel coordinates.
   */
  virtual void getPixelCoordinates(const double userCoord[3], int pixCoord[2]) = 0;

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

  /** maximum found distance between a graphic object and the viewpoint */
  double m_dFarthestDist;

  /** Scale modification to fit axes coordinates */
  double m_aAxesScale[3];

  /** Translation to put the axes in view */
  double m_aAxesTranslation[3];

  /** Rotation angles */
  double m_dAlpha;
  double m_dTheta;

  /** Center of the axes box */
  double m_aBoxCenter[3];

  /** Reduction Ration to be applied on the graphic in order for it to fit the window */
  double m_dFitWindowRatio;

};

}

#endif /* _CAMERA_BRIDGE_H_ */
