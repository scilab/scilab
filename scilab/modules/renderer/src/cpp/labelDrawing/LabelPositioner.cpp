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
#include "../textDrawing/DrawableText.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
#include "math_graphics.h"
}

namespace sciGraphics
{

#define LABEL_PIXEL_OFFSET 15.0
#define LABEL_RELATIVE_DISTANCE_TO_AXIS 1.2

/*------------------------------------------------------------------------------------------*/
LabelPositioner::LabelPositioner(DrawableLabel * label)
{
  m_pLabel = label;
  m_dDistanceToAxis = 0.0;
}
/*------------------------------------------------------------------------------------------*/
LabelPositioner::~LabelPositioner(void)
{
  m_dDistanceToAxis = 0.0;
  m_pLabel = NULL;
}
/*------------------------------------------------------------------------------------------*/
bool LabelPositioner::setLabelPosition(void)
{
  sciPointObj * pLabel = m_pLabel->getDrawedObject();
  if (sciGetAutoPosition(pLabel))
  {
    double newPos[3];
    if (getAutoPosition(newPos)) {
      // position could be compute succesfully
      sciSetTextPos(pLabel, newPos[0], newPos[1], newPos[2]);
      return true;
    }
    else
    {
      return false;
    }
  }
  return true;
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::setLabelOrientation(void)
{
  sciPointObj * pLabel = m_pLabel->getDrawedObject();
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
void LabelPositioner::getTextDirections(double widthVect[3], double heightVect[3])
{
  double corners[4][3];
  sciPointObj * pLabel = m_pLabel->getDrawedObject();

  // set default position for text
  sciSetTextPos(pLabel, 1.0, 1.0, 1.0);

  getTextDrawer(pLABEL_FEATURE(pLabel)->text)->getBoundingRectangle(corners[0], corners[1],
                                                                    corners[2], corners[3]);

  // corners are given scaled
  // so unscale them
  sciPointObj * parentSubwin = sciGetParentSubwin(pLabel);
  for (int i = 0; i < 4; i++)
  {
    getSubwinDrawer(parentSubwin)->pointScale(corners[i][0], corners[i][1], corners[i][2],
                                              &(corners[i][0]), &(corners[i][1]), &(corners[i][2]));
  }

  vectSubstract3D(corners[3], corners[0], widthVect);
  vectSubstract3D(corners[1], corners[0], heightVect);
}
/*------------------------------------------------------------------------------------------*/
void LabelPositioner::getLabelDisplacement(const double ticksDirection[3], double displacement[3])
{
  // first get width and height of label
  double textWidth[3];
  double textHeight[3];
  getTextDirections(textWidth, textHeight);

  // then find the direction on the screen on which the text is drawn
  // compute ticks direction in pixels
  sciPointObj * pLabel = m_pLabel->getDrawedObject();
  sciPointObj * parentSubwin = sciGetParentSubwin(pLabel);
  Camera * camera = getSubwinDrawer(parentSubwin)->getCamera();

  double origin[3] = {0.0, 0.0, 0.0};
  double originPix[3];
  camera->getPixelCoordinatesRaw(origin, originPix);
  double ticksDirPix[3];
  camera->getPixelCoordinatesRaw(ticksDirection, ticksDirPix);
  vectSubstract3D(ticksDirPix, originPix, ticksDirPix);

  // get rotation angle of the label
  double textAngle = sciGetFontOrientation(pLabel);

  // rotate ticksDirection relatively to label angle
  rotate2D(ticksDirPix, origin, -textAngle, ticksDirPix);

  // compute displacement coordinates within the width, height coordinates
  //   \  top /
  //    \    /
  //     \  /
  // left \/ right
  //      /\
  //     /  \
  //    /    \
  //   /bottom\
 
  double localDisplacement[3];
  if (ticksDirPix[0] > Abs(ticksDirPix[1]))
  {
    // right
    localDisplacement[0] = 0.0;
    localDisplacement[1] = 0.5;
    localDisplacement[2] = 0.0;
  }
  else if (ticksDirPix[0] < -Abs(ticksDirPix[1]))
  {
    // left
    localDisplacement[0] = -1.0;
    localDisplacement[1] = 0.5;
    localDisplacement[2] = 0.0;
  }
  else if (ticksDirPix[1] > Abs(ticksDirPix[0])) {
    // bottom
    localDisplacement[0] = -0.5;
    localDisplacement[1] = 1.0;
    localDisplacement[2] = 0.0;
    
  }
  else
  {
    // top
    localDisplacement[0] = -0.5;
    localDisplacement[1] = 0.0;
    localDisplacement[2] = 0.0;
    
  }

  //rotate2D(localDisplacement, origin, -textAngle, localDisplacement);

  // convert it to user coordinates
  scalarMult3D(textWidth, localDisplacement[0], displacement);
  double tempVect[3];
  scalarMult3D(textHeight, localDisplacement[1], tempVect);
  vectAdd3D(displacement, tempVect, displacement);

  // add a little offset of a certain number of pixels pixels
  // the length in pixels of ticksDirection is norm(ticksDirPix).
  // So 5 a vector of 5 pixels is 5 / ticksDirPix * ticksDirection
  if (m_dDistanceToAxis == 0)
  {
    double offset[3];
    scalarMult3D(ticksDirection, LABEL_PIXEL_OFFSET / NORM_3D(ticksDirPix), offset);
    vectAdd3D(displacement, offset, displacement);
  }
}
/*------------------------------------------------------------------------------------------*/
bool LabelPositioner::getAutoPosition(double pos[3])
{
  double axisStart[3];
  double axisEnd[3];
  double ticksDir[3];
  if (!getAxisPosition(axisStart, axisEnd, ticksDir))
  {
    return false;
  }



  // get middle of axis
  double axisMiddle[3];
  vectAdd3D(axisStart, axisEnd, axisMiddle);
  scalarMult3D(axisMiddle, 0.5, axisMiddle);

  // get ticks direction normalized
  normalize3D(ticksDir);

  // set it with label to distance axis
  //scalarMult3D(ticksDir, m_dDistanceToAxis * 1.5, ticksDir);

  double labelDisplacement[3];
  getLabelDisplacement(ticksDir, labelDisplacement);

  // set it with label to distance axis
  scalarMult3D(ticksDir, m_dDistanceToAxis * LABEL_RELATIVE_DISTANCE_TO_AXIS, ticksDir);

  vectAdd3D(ticksDir, labelDisplacement, ticksDir);


  // compute pos = axisMiddle + (dist + radius).ticksDir
  vectAdd3D(axisMiddle, ticksDir, pos);

  sciPointObj * pLabel = m_pLabel->getDrawedObject();
  sciPointObj * parentSubwin = sciGetParentSubwin(pLabel);
  getSubwinDrawer(parentSubwin)->inversePointScale(pos[0], pos[1], pos[2], &pos[0], &pos[1], &pos[2]);

  // everything ok
  return true;

}
/*------------------------------------------------------------------------------------------*/
}

