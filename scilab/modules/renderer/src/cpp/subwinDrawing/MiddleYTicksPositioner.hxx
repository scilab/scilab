/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _MIDDLE_YTICKS_POSITIONNER_HXX_
#define _MIDDLE_YTICKS_POSITIONNER_HXX_

#include "YTicksPositioner.hxx"

namespace sciGraphics
{

/**
 * Class positionnign ticks for y_location = right
 */
class MiddleYTicksPositioner : public YTicksPositioner
{
public:

  MiddleYTicksPositioner(DrawableSubwin * subwin);

  virtual ~MiddleYTicksPositioner(void);

protected:


  /**
   * Find the Z coordinate of the Y axis segment.
   * Y axis segment is always displayed on the top of the box.
   * @return Z coordinate of the segment to draw
   */
  virtual double findZCoordinate(void);
	
  /**
   * Compute the X coordinate of the Y axis segment
   * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
   * @return X coordinate of the segment to draw
   */
  virtual double findXCoordinate(double zCoordinate);

};


}

#endif /* _MIDDLE_YTICKS_POSITIONNER_HXX_ */
