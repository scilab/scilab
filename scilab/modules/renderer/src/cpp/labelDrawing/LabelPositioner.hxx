/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in positioning labels  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _LABEL_POSITIONER_HXX_ 
#define _LABEL_POSITIONER_HXX_

#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

class DrawableLabel;

class LabelPositioner
{
public:

  LabelPositioner(DrawableLabel * label);

  virtual ~LabelPositioner(void);

  /**
   * Set position of the label.
   */
  bool setLabelPosition(void);

  /**
   * Set font angle for labels.
   */
  void setLabelOrientation(void);

  /**
   * Set distance in pixels from labels to axis
   */
  void setDistanceToAxis(double distance);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Get the height and width vector of the label
   */
  void getTextDirections(double widthVect[3], double heightVect[3]);

  /**
   * Compute the displacement we need to apply according to the text size
   * @param ticksDirection normalized ticks direction
   */
  void getLabelDisplacement(const double ticksDirection[3], double displacement[3]);

  /**
   * Compute the automatic position of the label
   */
  virtual bool getAutoPosition(double pos[3]);

  /**
   * Get bounds of the axis to draw and direction of ticks
   */
  virtual bool getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDir[3]) = 0;

  /**
   * Compute the automatic font angle of the label.
   */
  virtual double getAutoOrientation(void) = 0;
  /*----------------------------------------------------------------------*/
  double m_dDistanceToAxis;

  DrawableLabel * m_pLabel;
  /*----------------------------------------------------------------------*/
};

}

#endif /* _LABEL_POSITIONER_HXX_ */

