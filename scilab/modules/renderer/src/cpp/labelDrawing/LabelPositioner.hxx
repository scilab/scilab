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

class LabelPositioner : public virtual DrawableObjectBridge
{
public:

  LabelPositioner(void);

  virtual ~LabelPositioner(void);

  /**
   * Set position of the label.
   */
  void setLabelPosition(void);

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
   * Compute the automatic position of the label
   */
  virtual void getAutoPosition(double pos[3]) = 0;

  /**
   * Compute the automatic font angle of the label.
   */
  virtual double getAutoOrientation(void) = 0;
  /*----------------------------------------------------------------------*/
  double m_dDistanceToAxis;
  /*----------------------------------------------------------------------*/
};

}

#endif /* _LABEL_POSITIONER_HXX_ */
