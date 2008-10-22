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

#include "ZLabelPositioner.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
ZLabelPositioner::ZLabelPositioner(DrawableLabel * label)
  : LabelPositioner(label)
{
  
}
/*------------------------------------------------------------------------------------------*/
ZLabelPositioner::~ZLabelPositioner(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
bool ZLabelPositioner::getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDir[3])
{
  sciPointObj * parentSubwin = sciGetParentSubwin(m_pLabel->getDrawedObject());
  return getSubwinDrawer(parentSubwin)->getZAxisPosition(axisStart, axisEnd, ticksDir);
}
/*------------------------------------------------------------------------------------------*/
double ZLabelPositioner::getAutoOrientation(void)
{
  // always horizontal
  return 0.0;
}
/*------------------------------------------------------------------------------------------*/

}
