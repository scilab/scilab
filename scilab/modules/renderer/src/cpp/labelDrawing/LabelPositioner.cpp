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

#include "LabelPositioner.hxx"
#include "DrawableLabel.h"

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
LabelPositioner::LabelPositioner(void)
{
   m_dDistanceToAxis = 0.0;
}
/*------------------------------------------------------------------------------------------*/
LabelPositioner::~LabelPositioner(void)
{
  m_dDistanceToAxis = 0.0;
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::setLabelPosition(void)
{
  sciPointObj * pLabel = getDrawer()->getDrawedObject();
  if (sciGetAutoPosition(pLabel))
  {
    double newPos[3];
    getAutoPosition(newPos);

    sciSetTextPos(pLabel, newPos[0], newPos[1], newPos[2]);
  }
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::setLabelOrientation(void)
{
  sciPointObj * pLabel = getDrawer()->getDrawedObject();
  if (sciGetAutoRotation(pLabel))
  {
    sciSetFontOrientation(pLabel, getAutoOrientation());
  }
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::setDistanceToAxis(double distance)
{
  m_dDistanceToAxis = distance;
}
/*------------------------------------------------------------------------------------------*/
}

