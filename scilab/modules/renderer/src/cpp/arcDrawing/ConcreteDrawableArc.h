/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing implementation of driver independent algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_ARC_HXX_
#define _CONCRETE_DRAWABLE_ARC_HXX_

#include <list>

#include "DrawableArc.h"
#include "DrawArcStrategy.h"

namespace sciGraphics
{

/**
 * Class containing implementation of driver independent algorithms.
 */
class ConcreteDrawableArc : public DrawableArc
{
public:

  ConcreteDrawableArc(sciPointObj * pObj);

  virtual ~ConcreteDrawableArc(void);

  /**
   * Add a rendering algorithm
   */
  virtual void addDrawingStrategy( DrawArcStrategy * strategy );

  /**
   * Remove all drawing strategies.
   */
  virtual void removeDrawingStrategies( void );

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
                                    double & startAngle, double & endAngle);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Actually draw the arc on the screen
   */
  virtual void drawArc(void);

  /**
   * Redraw arc from precomputed data
   */
  virtual void redrawArc(void);

  /**
   * Actually show the arc stored data
   */
  virtual void showArc(void);
  /*----------------------------------------------------------------------*/
  std::list<DrawArcStrategy *> m_oDrawingStrategies;
  /*----------------------------------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_ARC_HXX_ */
