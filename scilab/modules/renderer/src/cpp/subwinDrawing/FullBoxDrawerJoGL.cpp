/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing the full bounding box of a subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FullBoxDrawerJoGL.hxx"
#include "FullBoxDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
FullBoxDrawerJoGL::FullBoxDrawerJoGL(DrawableSubwin * subwin)
  : DrawAxesBoxStrategy(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new FullBoxDrawerJavaMapper());
}
/*--------------------------------------------------------------------------*/
FullBoxDrawerJoGL::~FullBoxDrawerJoGL(void)
{

}
/*--------------------------------------------------------------------------*/
void FullBoxDrawerJoGL::drawAxesBox(int concealedCornerIndex)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();
  initializeDrawing();

  getFullBoxDrawerJavaMapper()->setBoxParameters(pSUBWIN_FEATURE(pSubwin)->axes.hiddenAxisColor,
                                                 sciGetGraphicContext(pSubwin)->foregroundcolor,
                                                 sciGetLineStyle(pSubwin),
                                                 (float)sciGetLineWidth(pSubwin));

  // get displayed bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getFullBoxDrawerJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2],
                                        bounds[3], bounds[4], bounds[5],
                                        concealedCornerIndex);
  endDrawing();
}
/*--------------------------------------------------------------------------*/
FullBoxDrawerJavaMapper * FullBoxDrawerJoGL::getFullBoxDrawerJavaMapper(void)
{
  return dynamic_cast<FullBoxDrawerJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
}
