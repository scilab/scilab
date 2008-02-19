/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing ticks for X axis 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "XTicksDrawerJoGL.hxx"
#include "TopXTicksDrawerJavaMapper.hxx"
#include "MiddleXTicksDrawerJavaMapper.hxx"
#include "BottomXTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::XTicksDrawerJoGL(DrawableSubwin * subwin)
 : TicksDrawerJoGL(subwin)
{
  sciPointObj * pSubwin = subwin->getDrawedObject();

  switch(pSUBWIN_FEATURE(pSubwin)->axes.xdir)
  {
  case 'u':
    setJavaMapper(new TopXTicksDrawerJavaMapper());
    break;
  case 'c':
    setJavaMapper(new MiddleXTicksDrawerJavaMapper());
    break;
  case 'd':
    setJavaMapper(new BottomXTicksDrawerJavaMapper());
    break;
  default:
    setJavaMapper(new TopXTicksDrawerJavaMapper());
    break;
  }
  
}
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::~XTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * XTicksDrawerJoGL::getXTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
