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

#include "YTicksDrawerJoGL.hxx"
#include "LeftYTicksDrawerJavaMapper.hxx"
#include "MiddleYTicksDrawerJavaMapper.hxx"
#include "RightYTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YTicksDrawerJoGL::YTicksDrawerJoGL(DrawableSubwin * subwin)
  : TicksDrawerJoGL(subwin)
{
  sciPointObj * pSubwin = subwin->getDrawedObject();

  switch(pSUBWIN_FEATURE(pSubwin)->axes.ydir)
  {
  case 'l':
    // special case for 2D, y labels is drawn on the opposite side
    if(sciGetIs3d(pSubwin))
    {
      setJavaMapper(new LeftYTicksDrawerJavaMapper());
    }
    else
    {
      setJavaMapper(new RightYTicksDrawerJavaMapper());
    }
    break;
  case 'c':
    setJavaMapper(new MiddleYTicksDrawerJavaMapper());
    break;
  case 'r':
    // special case for 2D, y labels is drawn on the opposite side
    if(sciGetIs3d(pSubwin))
    {
      setJavaMapper(new RightYTicksDrawerJavaMapper());
    }
    else
    {
      setJavaMapper(new LeftYTicksDrawerJavaMapper());
    }
    break;
  default:
    setJavaMapper(new LeftYTicksDrawerJavaMapper());
    break;
  }

}
/*------------------------------------------------------------------------------------------*/
YTicksDrawerJoGL::~YTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * YTicksDrawerJoGL::getYTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
