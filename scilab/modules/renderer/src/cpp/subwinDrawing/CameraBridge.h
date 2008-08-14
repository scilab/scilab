/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Bridge between camera class and impelementation  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CAMERA_BRIDGE_H_
#define _CAMERA_BRIDGE_H_

#include <list>

#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

class CameraBridge : public virtual DrawableObjectBridge
{
public:

  /**
   * Default constructor
   */
  CameraBridge( void );

  virtual ~CameraBridge( void );

  /**
   * Set the viewing position o
   */
  void setViewingArea( double translation[2], double scale[2] ) ;

  /**
   * Set the scale to fit with the subwin data bounds
   */
  void setAxesFittingScale( double scale[3] ) ;

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
   * Set scale to mormalize values between 0 and 1.
   */
  void setAxesNormalizationScale(double scale[3]);

  /**
   * Convert user coordinates to pixel coordinates.
   */
  virtual void getPixelCoordinates(const double userCoord[3], int pixCoord[2]) = 0;

  /**
   * Convert user coordinates to pixel coordinates including depth value
   */
  virtual void getPixelCoordinates(const double userCoords[3], double pixCoords[3]) = 0;

  /**
   * Convert user coordinates to pixel coordinates including depth value.
   * Does not take pixel coordinates into account.
   */
  virtual void getPixelCoordinatesRaw(const double userCoords[3], double pixCoords[3]) = 0;

  /**
   * Unproject a 3 position in pixels into the standard scene coordinates
   */
  virtual void getSceneCoordinates(const double pixCoords[3], double userCoords[3]) = 0;

  /**
   * Unproject a 3 position in pixels into the standard scene coordinates
   * Does not take logarithmic sclae into account.
   */
  virtual void getSceneCoordinatesRaw(const double pixCoords[3], double userCoords[3]) = 0;

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  virtual void get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2]) = 0;

  /**
   * Convert user coordinates to pixel coordinated using the 2d view.
   */
  virtual void get2dViewPixelCoordinates(const double userCoord[3], double pixCoord[3]) = 0;

  /**
   * Find coordinates in 2D view from pixel ones
   */
  virtual void get2dViewCoordinates(const int pixCoords[2], double userCoord2D[2]) = 0;

  /**
   * Position the view and view area accordingly to previous calls.
   */
  virtual void renderPosition( void ) = 0 ;

  /**
   * Put back camera to default position.
   */
  virtual void replaceCamera( void ) = 0;

  /**
  * Display the camera using precomputed data
  */
  virtual void redraw(void) = 0;

  /**
   * Get the position and size of the rectangle in which the axes box must fit
   */
  virtual void getViewingArea(int * xPos, int * yPos, int * width, int * height) = 0;

  /**
   * Get viewport of the canvas
   */
  virtual void getViewport(int viewport[4]) = 0;

  /**
   * Add an axes reverse strategy.
   */
  void setAxesReverse(bool axisReverseX, bool axisReverseY, bool axisReverseZ);


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

  /** Scale modification to fit axes coordinates */
  double m_aAxesFittingScale[3];

  /** Translation to put the axes in view */
  double m_aAxesTranslation[3];

  /** Scale modification to map data on the [0,1]x[0,1]x[0,1] cube. */
  double m_aAxesNormalizationScale[3];

  /** Rotation angles */
  double m_dAlpha;
  double m_dTheta;

  /** Center of the axes box */
  double m_aBoxCenter[3];

  /** list of axes reverse strategies */
  bool m_aAxesReverse[3];

};

}

#endif /* _CAMERA_BRIDGE_H_ */
