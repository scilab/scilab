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

#include "TitlePositioner.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
}

#define DEPTH_FRONT 0.01

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
TitlePositioner::TitlePositioner(DrawableLabel * label)
  : LabelPositioner(label)
{
  m_aPreviousPosition[0] = -10.0;
  m_aPreviousPosition[1] = -10.0;
  m_aPreviousPosition[2] = -10.0;
}
/*------------------------------------------------------------------------------------------*/
TitlePositioner::~TitlePositioner(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
bool TitlePositioner::getAutoPosition(double pos[3])
{
  sciPointObj * pLabel = m_pLabel->getDrawedObject();
  sciPointObj * parentSubwin = sciGetParentSubwin(pLabel);
  Camera * camera = getSubwinDrawer(parentSubwin)->getCamera();

  // get axis bounds
  int xPos;
  int yPos;
  int width;
  int height;
  camera->getViewingArea(&xPos, &yPos, &width, &height);

  // get height of the text
  double corners[4][3];
  getTextDrawer(pLABEL_FEATURE(pLabel)->text)->getScreenBoundingBox(corners[0], corners[1],
                                                                    corners[2], corners[3]);

  double textHeight = Abs(corners[0][1] - corners[2][1]);
  double textWidth = Abs(corners[0][0] - corners[2][0]);

  double pixCoords[3];
  pixCoords[0] = xPos + (width - textWidth) / 2.0;
  pixCoords[1] = yPos - textHeight / 2.0;
  pixCoords[2] = DEPTH_FRONT; // put it in front of depth buffer

  // Title may flicker because its position is computed
  // using several projections and truncation, it may move
  // from one pixel to an other. Consequently, we move the title
  // only if the displacement is 2 pixels or more.
  double titleDisplacement[3];
  vectSubstract3D(pixCoords, m_aPreviousPosition, titleDisplacement);
  if (SQUARE_NORM_2D(titleDisplacement) >= 4.0)
  {
    // use new position
    m_aPreviousPosition[0] = round(pixCoords[0]);
    m_aPreviousPosition[1] = round(pixCoords[1]);
    m_aPreviousPosition[2] = pixCoords[2];
  }
  else
  {
    pixCoords[0] = m_aPreviousPosition[0];
    pixCoords[1] = m_aPreviousPosition[1];
    pixCoords[2] = m_aPreviousPosition[2];
  }

  camera->getSceneCoordinates(pixCoords, pos);
  


  return true;

}
/*------------------------------------------------------------------------------------------*/
bool TitlePositioner::getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDir[3])
{
  // not used here
  // we use directly getAutoPosition
  return true;
}
/*------------------------------------------------------------------------------------------*/
double TitlePositioner::getAutoOrientation(void)
{
  // always horizontal
  return 0.0;
}
/*------------------------------------------------------------------------------------------*/

}
