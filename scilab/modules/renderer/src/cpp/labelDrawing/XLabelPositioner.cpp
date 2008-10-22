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

#include "XLabelPositioner.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XLabelPositioner::XLabelPositioner(DrawableLabel * label)
  : LabelPositioner(label)
{
  
}
/*------------------------------------------------------------------------------------------*/
XLabelPositioner::~XLabelPositioner(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
bool XLabelPositioner::getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDir[3])
{
  sciPointObj * parentSubwin = sciGetParentSubwin(m_pLabel->getDrawedObject());
  return getSubwinDrawer(parentSubwin)->getXAxisPosition(axisStart, axisEnd, ticksDir);
}
/*------------------------------------------------------------------------------------------*/
double XLabelPositioner::getAutoOrientation(void)
{
  // always horitzontal
  return 0.0;
}
/*------------------------------------------------------------------------------------------*/

}
