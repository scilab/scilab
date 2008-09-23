/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for an arc object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_ARC_H_
#define _DRAWABLE_ARC_H_


#include "DrawableClippedObject.h"
#include "DrawArcStrategy.h"
#include "DrawableArcBridge.h"


namespace sciGraphics
{

class DrawableArc : public DrawableClippedObject
{

public:

  DrawableArc( sciPointObj * pObj ) ;

  virtual ~DrawableArc( void ) ;

  /**
   * The arc is a part of an ellipse. Get the representation of the ellipse.
   * The minor and major semiAxes create an orthogonal frame on the ellipse plane.
   * @param center center of the ellipse
   * @param semiAxis1 semi minor axis of the ellipse from the center to the border.
   * @param semiAxis2 semi major axis of the ellipse from the center to the border.
   * @param startAngle lowest angle in radian, relative to the semi-major axis of the ellipse.
   *                   Start of the angular part to draw.
   * @param endAngle highest angle in radian, relative to the semi-major axis of the ellipse.
   *                 End of the angular part to draw
   */
  virtual void getArcRepresentation(double center[3], double semiMinorAxis[3], double semiMajorAxis[3],
                                    double & startAngle, double & endAngle) = 0;

  /**
   * Return the real type of implementation object
   */
  DrawableArcBridge * getArcImp( void ) ;

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Arcs need to be redrawn each time the axes changes
   * since the view may have changed
   */
  virtual EDisplayStatus redraw(void) ;

  /**
   * Actually draw the arc on the screen
   */
  virtual void drawArc(void) = 0;

  /**
   * Actually draw the arc on the screen
   */
  virtual void redrawArc(void) = 0;

  /**
   * Actually show the arc stored data
   */
  virtual void showArc(void) = 0;

  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_ARC_H_  */
