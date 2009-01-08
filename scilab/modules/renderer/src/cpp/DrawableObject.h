/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class dedicated for the graphic rendering of graphic handles
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_OBJECT_H_
#define _DRAWABLE_OBJECT_H_

#include "DrawableObjectBridge.h"

extern "C"
{
#include "ObjectStructure.h"
}

namespace sciGraphics
{

/**
 * Object dedicated to render a sciPointObj.
 */
class DrawableObject
{

public:

  /**
   * Possible return values for drawing methods
   * FAILURE is returned if the drawing could not complete because of an error.
   * UNCHANGED is returned if the object is not drawn (not visible for instance)
   * SUCCESS if the object has been successfully displayed
   */
  enum EDisplayStatus {SUCCESS, UNCHANGED, FAILURE};

  DrawableObject( sciPointObj * drawed ) ;

  virtual ~DrawableObject( void ) ;

  /**
   * Display a handle on the screen.
   */
  virtual void display( void ) ;

  /**
   * In case you want to retrieve the drawed object
   */
  sciPointObj * getDrawedObject( void ) { return m_pDrawed ; }

  /**
   * To specify that the object was modified and need to be redrawn
   */
  virtual void hasChanged( void ) ;

  /**
   * To specify that an object and its children have changed
   */
  void familyHasChanged( void ) ;

  /**
   * Specify that the parent axis have changed.
   * Children which need to be redrawn must redefine this
   * function.
   */
  void parentSubwinChanged( void );

  /**
   * Common to every object, draw their children
   */
  virtual void displayChildren( void ) ;


  /**
   * For non linear scaling (not supported by OpenGL) we need to modify points.
   * For direction, use direction Scale.
   */
  virtual void pointScale(double xCoord, double yCoord, double zCoord,
                          double * xScaled, double * yScaled, double * zScaled);


  /**
   * Inverse function of pointScale.
   */
  virtual void inversePointScale(double xScaled, double yScaled, double zScaled,
                                 double * xCoord, double * yCoord, double * zCoord);

  /**
   * Apply scale on a vector.
   */
  virtual void pointScale(double vectorX[], double vectorY[], double vectorZ[], int vectorLength);

  /**
   * Apply non linear scaling on direction vector (opposite as point vector).
   * Since scaling is non linear, we need to have a starting point of the vector.
   */
  virtual void directionScale(double xCoord, double yCoord, double zCoord,
                              double startingPointX, double startingPointY, double startingPointZ,
                              double * xScaled, double * yScaled, double * zScaled);

  /**
   * Apply distance scale on a vector.
   */
  virtual void directionScale(double vectorX[], double vectorY[], double vectorZ[],
                              double startingPointsX[], double startingPointsY[],
                              double startingPointsZ[], double vectorLength);

  /**
   * Set the driver dependent implementation of algorithms
   */
  void setDrawableImp( DrawableObjectBridge * imp ) ;

  /**
   * Apply a translation to the object.
   * The next call to show will take it into account
   */
  virtual void move(const double translation[3]);

  /**
   * Reinit the move translation
   */
  void reinitMove(void);

  /**
   * Get the translation
   */
  void getMoveTranslation(double translation[3]);

  /**
   * Get the driver dependent implementation of algorithm
   * Each graphic object must have is own
   */
  DrawableObjectBridge * getDrawableImp( void ) { return m_pImp ; }

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) = 0 ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) = 0 ;

  /**
   * Called when the axes change, so data are still ok
   * but some objects might need to update display lists.
   * By default just call show
   */
  virtual EDisplayStatus redraw(void);

  /**
   * Translate the object with the record move in camera coordinates
   */
  virtual void translate(void);

  /**
   * End translation
   */
  virtual void endTranslate(void);

  /**
   * Common to every object. Check its visibility.
   */
  bool checkVisibility( void ) ;

  /**
   * Initialize the context for drawing
   */
  virtual void initializeDrawing( void ) ;

  /**
   * Close drawing session and display image on the screen
   */
  virtual void endDrawing( void ) ;

	/**
	 * Update the object because it has changed.
	 */
	virtual void update(void);

  /*---------------------------------------------------------------------------------*/

  /**
   * The graphic handle to draw
   */
  sciPointObj * m_pDrawed ;

  /**
   * To know if the object needs to be drawn again.
   */
  bool m_bNeedDraw ;

  /**
   * To know if the object need to be redrawn.
   * Happens when axes changed
   */
  bool m_bNeedRedraw;

	/**
	 * To know if the inner structure of the object need to be reconstructed
	 */
	bool m_bNeedUpdate;

  /**
   *  Contains drivers independent algorithm common to all DrawableObjects
   */
  DrawableObjectBridge * m_pImp ;

  /** Record the translation needed by the move */
  double m_aMoveTranslation[3];
  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_OBJECT_H_  */
