/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class positioning Z label using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ZLabelPositionerJoGL.hxx"
#include "ZLabelPositionerJavaMapper.hxx"

extern "C"
{
#include "math_graphics.h"
}


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
ZLabelPositionerJoGL::ZLabelPositionerJoGL(DrawableLabel * label)
  : LabelPositionerJoGL(label)
{
  setJavaMapper(new ZLabelPositionerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
ZLabelPositionerJoGL::~ZLabelPositionerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
double ZLabelPositionerJoGL::getAutoOrientation(void)
{
  // vertical
  return 1.5 * M_PI;
}
/*------------------------------------------------------------------------------------------*/
}
