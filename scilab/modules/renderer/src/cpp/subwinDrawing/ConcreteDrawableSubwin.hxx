/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing drivers independent routines for a subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_SUBWIN_HXX_
#define _CONCRETE_DRAWABLE_SUBWIN_HXX_

#include "DrawableSubwin.h"
#include "ComputeBoundsStrategy.hxx"
#include "DrawAxesBoxStrategy.hxx"
#include "TicksDrawer.hxx"
#include "../labelDrawing/LabelPositioner.hxx"

namespace sciGraphics
{

class ConcreteDrawableSubwin : public DrawableSubwin
{
public:

  ConcreteDrawableSubwin(sciPointObj * pObj);

  virtual ~ConcreteDrawableSubwin(void);

  void setXBoundsStrategy(ComputeBoundsStrategy * strategy);
  void setYBoundsStrategy(ComputeBoundsStrategy * strategy);
  void setZBoundsStrategy(ComputeBoundsStrategy * strategy);

  void setXTicksDrawer(TicksDrawer * xTicksDrawer);
  void setYTicksDrawer(TicksDrawer * yTicksDrawer);
  void setZTicksDrawer(TicksDrawer * zTicksDrawer);

  void setAxesBoxDrawer(DrawAxesBoxStrategy * strategy);

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


protected:

  /*---------------------------------------------------------------------*/
  /**
   * Compute real data bounds from the user ones.
   * ie fills Frect.
   */
  virtual void computeRealDataBounds(void);

  /**
   * Draw the surrounding cube of the subwin object.
   */
  virtual void drawBox(void);

  /**
   * Draw the ticks of the subwin object.
   */
  virtual void drawTicks(void);

  /**
   * Draw the surrounding cube of the subwin object from alraedy computed data.
   */
  virtual void showBox(void);

  /**
   * Draw the ticks of the subwin from alraedy computed data.
  */
  virtual void showTicks(void);

  /**
   * Set the distance to use between axis and labels.
   * May vary depending on the ticks drawing, the two should not
   * conceal each other.
   */
  virtual void setLabelsDistanceToAxis(double xLabelDist, double yLabelDist,
                                       double zLabelDist, double titleDist);
  /*---------------------------------------------------------------------*/
  ComputeBoundsStrategy * m_pXBoundsStrategy;
  ComputeBoundsStrategy * m_pYBoundsStrategy;
  ComputeBoundsStrategy * m_pZBoundsStrategy;

  TicksDrawer * m_pXTicksDrawer;
  TicksDrawer * m_pYTicksDrawer;
  TicksDrawer * m_pZTicksDrawer;

  DrawAxesBoxStrategy * m_pAxesBoxDrawer;
  /*---------------------------------------------------------------------*/

  
};

}

#endif /* _CONCRETE_DRAWABLE_SUBWIN_HXX_ */

