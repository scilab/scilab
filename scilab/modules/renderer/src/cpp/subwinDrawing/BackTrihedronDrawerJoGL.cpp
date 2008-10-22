/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing only the backTrihedron of a subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "BackTrihedronDrawerJoGL.hxx"
#include "BackTrihedronDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJoGL::BackTrihedronDrawerJoGL(DrawableSubwin * subwin)
  : DrawAxesBoxStrategy(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new BackTrihedronDrawerJavaMapper());
}
/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJoGL::~BackTrihedronDrawerJoGL(void)
{
  
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJoGL::drawAxesBox(int concealedCornerIndex)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();
  initializeDrawing();

  getBackTrihedronDrawerJavaMapper()->setBoxParameters(pSUBWIN_FEATURE(pSubwin)->axes.hiddenAxisColor,
                                                       (float) sciGetLineWidth(pSubwin));

  // get displayed bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getBackTrihedronDrawerJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2],
                                              bounds[3], bounds[4], bounds[5],
                                              concealedCornerIndex);
  endDrawing();
}
/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJavaMapper * BackTrihedronDrawerJoGL::getBackTrihedronDrawerJavaMapper(void)
{
  return dynamic_cast<BackTrihedronDrawerJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
}
