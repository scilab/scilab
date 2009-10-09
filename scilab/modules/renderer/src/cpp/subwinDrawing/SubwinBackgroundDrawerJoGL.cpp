/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SubwinBackgroundDrawerJoGL.hxx"
#include "SubwinBackgroundDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
SubwinBackgroundDrawerJoGL::SubwinBackgroundDrawerJoGL(DrawableSubwin * subwin)
  : DrawAxesBoxStrategy(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new SubwinBackgroundDrawerJavaMapper());
}
/*--------------------------------------------------------------------------*/
SubwinBackgroundDrawerJoGL::~SubwinBackgroundDrawerJoGL(void)
{
  
}
/*--------------------------------------------------------------------------*/
void SubwinBackgroundDrawerJoGL::drawAxesBox(int concealedCornerIndex)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();
  initializeDrawing();

  getSubwinBackgroundDrawerJavaMapper()->setBoxParameters(sciGetGraphicContext(pSubwin)->backgroundcolor);

  // get displayed bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getSubwinBackgroundDrawerJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2],
                                                 bounds[3], bounds[4], bounds[5],
                                                 concealedCornerIndex);
  endDrawing();
}
/*--------------------------------------------------------------------------*/
SubwinBackgroundDrawerJavaMapper * SubwinBackgroundDrawerJoGL::getSubwinBackgroundDrawerJavaMapper(void)
{
  return dynamic_cast<SubwinBackgroundDrawerJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
}
