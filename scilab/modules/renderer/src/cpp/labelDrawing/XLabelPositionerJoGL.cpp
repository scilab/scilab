/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class positioning X label using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "XLabelPositionerJoGL.hxx"
#include "XLabelPositionerJavaMapper.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XLabelPositionerJoGL::XLabelPositionerJoGL(DrawableLabel * label)
  : LabelPositionerJoGL(label)
{
  setJavaMapper(new XLabelPositionerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
XLabelPositionerJoGL::~XLabelPositionerJoGL(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
double XLabelPositionerJoGL::getAutoOrientation(void)
{
  // horizontal
  return 0;
}
/*------------------------------------------------------------------------------------------*/
}
