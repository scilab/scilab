/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing half of a subwin object bounding box
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "HalfBoxDrawerJoGL.hxx"
#include "HalfBoxDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
HalfBoxDrawerJoGL::HalfBoxDrawerJoGL(DrawableSubwin * subwin)
  : DrawAxesBoxStrategy(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new HalfBoxDrawerJavaMapper());
}
/*--------------------------------------------------------------------------*/
HalfBoxDrawerJoGL::~HalfBoxDrawerJoGL(void)
{

}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJoGL::drawAxesBox(int concealedCornerIndex)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();
  initializeDrawing();

  getHalfBoxDrawerJavaMapper()->setBoxParameters(pSUBWIN_FEATURE(pSubwin)->axes.hiddenAxisColor,
                                                 sciGetGraphicContext(pSubwin)->foregroundcolor,
                                                 sciGetLineStyle(pSubwin),
                                                 (float)sciGetLineWidth(pSubwin));

  // get displayed bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getHalfBoxDrawerJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2],
                                    bounds[3], bounds[4], bounds[5],
                                    concealedCornerIndex);
  endDrawing();
}
/*--------------------------------------------------------------------------*/
HalfBoxDrawerJavaMapper * HalfBoxDrawerJoGL::getHalfBoxDrawerJavaMapper(void)
{
  return dynamic_cast<HalfBoxDrawerJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
}
