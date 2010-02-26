/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AxisPositioner.hxx"
#include "getHandleDrawer.h"
#include "Camera.h"

extern "C"
{
#include "math_graphics.h"
}

namespace sciGraphics
{

#define TICKS_PIXEL_LENGTH 0.015

/*------------------------------------------------------------------------------------------*/
void AxisPositioner::setTicksDirectionLength(double ticksDir[3], sciPointObj * pSubwin)
{
  Camera * camera = getSubwinDrawer(pSubwin)->getCamera();
  double origin[3] = {0.0, 0.0, 0.0};
  camera->getPixelCoordinatesRaw(origin, origin);

  double pixDir[3];
  camera->getPixelCoordinatesRaw(ticksDir, pixDir);
  // get length in pixels
  vectSubstract3D(pixDir, origin, pixDir);
  normalize3D(pixDir);

  // get the viewport associated with the subwindow
  int xPos;
  int yPos;
  int width;
  int height;
  camera->getViewingArea(&xPos, &yPos, &width, &height);
		
  // compute sizes wich would apply if axes where along X or Y axis
  // and use at least 3 pixels
  double xSize = Max(width * TICKS_PIXEL_LENGTH, 3);
  double ySize = Max(height * TICKS_PIXEL_LENGTH, 3);
		
  // compute angle between ticks direction and x axis in pixel coordinates, pixDir[0]
  double angle = acos(Abs(pixDir[0]));
		
  // push it between 0 and 1.
  double fact = angle * 2.0 / M_PI;
		
  // apply number of pixels 
  scalarMult3D(pixDir, ((1.0 - fact) * xSize + fact * ySize), pixDir);

  // convert pix direction in 3D
  origin[0] = 0.0;
  origin[1] = 0.0;
  origin[2] = 0.0;
  camera->getSceneCoordinatesRaw(origin, origin);
  camera->getSceneCoordinatesRaw(pixDir, ticksDir);
  vectSubstract3D(ticksDir, origin, ticksDir);
}
/*------------------------------------------------------------------------------------------*/

#undef TICKS_PIXEL_LENGTH 

}
