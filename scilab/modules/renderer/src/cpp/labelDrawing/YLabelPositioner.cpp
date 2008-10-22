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

#include "YLabelPositioner.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YLabelPositioner::YLabelPositioner(DrawableLabel * label)
  : LabelPositioner(label)
{
  
}
/*------------------------------------------------------------------------------------------*/
YLabelPositioner::~YLabelPositioner(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
bool YLabelPositioner::getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDir[3])
{
  sciPointObj * parentSubwin = sciGetParentSubwin(m_pLabel->getDrawedObject());
  return getSubwinDrawer(parentSubwin)->getYAxisPosition(axisStart, axisEnd, ticksDir);
}
/*------------------------------------------------------------------------------------------*/
double YLabelPositioner::getAutoOrientation(void)
{
  // special case for 2D, Y labels is drawn vertically insttead of horizontally
  sciPointObj * parentSubwin = sciGetParentSubwin(m_pLabel->getDrawedObject());
  if (sciGetIs3d(sciGetParentSubwin(parentSubwin)))
  {
    // horizontal
    return 0.0;
  }
  else
  {
    // vertical
    return 3.0 * M_PI / 2.0;
  }
}
/*------------------------------------------------------------------------------------------*/

}
