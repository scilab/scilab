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

#ifndef _TITLE_POSITIONER_HXX_ 
#define _TITLE_POSITIONER_HXX_

#include "LabelPositioner.hxx"
#include "DrawableLabel.h"

namespace sciGraphics
{

class TitlePositioner : public LabelPositioner
{
public:

  TitlePositioner(DrawableLabel * label);

  virtual ~TitlePositioner(void);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Compute the automatic position of the label
   */
  virtual bool getAutoPosition(double pos[3]);

  /**
   * Get bounds of the axis to draw and direction of ticks
   */
  virtual bool getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDir[3]);

  /**
   * Compute the automatic font angle of the label.
   */
  virtual double getAutoOrientation(void);
  /*----------------------------------------------------------------------*/
  double m_aPreviousPosition[3];
  /*----------------------------------------------------------------------*/
};

}

#endif /* _TITLE_POSITIONER_HXX_ */

