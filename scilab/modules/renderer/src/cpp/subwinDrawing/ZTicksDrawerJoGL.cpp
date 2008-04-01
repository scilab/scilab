/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing ticks for Z axis
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ZTicksDrawerJoGL.hxx"
#include "ZTicksDrawerJavaMapper.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
ZTicksDrawerJoGL::ZTicksDrawerJoGL(DrawableSubwin * subwin)
  : TicksDrawerJoGL(subwin)
{

  // no choice here, only one position for z ticks
  setJavaMapper(new ZTicksDrawerJavaMapper());

}
/*------------------------------------------------------------------------------------------*/
ZTicksDrawerJoGL::~ZTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * ZTicksDrawerJoGL::getZTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
