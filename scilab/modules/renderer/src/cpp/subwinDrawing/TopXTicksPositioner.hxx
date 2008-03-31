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

#ifndef _TOP_XTICKS_POSITIONNER_HXX_
#define _TOP_XTICKS_POSITIONNER_HXX_

#include "XTicksPositioner.hxx"

namespace sciGraphics
{

/**
 * Class positionnign ticks for x_location = top
 */
class TopXTicksPositioner : public XTicksPositioner
{
public:

  TopXTicksPositioner(DrawableSubwin * subwin);

  virtual ~TopXTicksPositioner(void);

protected:


  /**
   * Find the Z coordinate of the X axis segment.
   * X axis segment is always displayed on the top of the box.
   * @return Z coordinate of the segment to draw
   */
  virtual double findZCoordinate(void);
	
  /**
   * Compute the Y coordinate of the X axis segment
   * @param zCoordinate Z coordinate of the X axis segment already computed by findZCoordinate
   * @return Y coordinate of the segment to draw
   */
  virtual double findYCoordinate(double zCoordinate);

};


}

#endif /* _TOP_XTICKS_POSITIONNER_HXX_ */
