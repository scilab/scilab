/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class positioning Y label using JoGLs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "YLabelPositionerJoGL.hxx"
#include "YLabelPositionerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YLabelPositionerJoGL::YLabelPositionerJoGL(DrawableLabel * label)
  : LabelPositionerJoGL(label)
{
  setJavaMapper(new YLabelPositionerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
YLabelPositionerJoGL::~YLabelPositionerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
double YLabelPositionerJoGL::getAutoOrientation(void)
{
  // special case for 2D, Y labels is drawn vertically insttead of horizontally
  sciPointObj * pLabel = m_pDrawer->getDrawedObject();
  if (sciGetIs3d(sciGetParentSubwin(pLabel)))
  {
    // horizontal
    return 0;
  }
  else
  {
    // vertical
    return 90;
  }
}
/*------------------------------------------------------------------------------------------*/
}
