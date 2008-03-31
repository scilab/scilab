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

#include "LeftYTicksPositioner.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
LeftYTicksPositioner::LeftYTicksPositioner(DrawableSubwin * subwin)
  : YTicksPositioner(subwin)
{

}
/*---------------------------------------------------------------------------------*/
LeftYTicksPositioner::~LeftYTicksPositioner(void)
{

}
/*---------------------------------------------------------------------------------*/
double LeftYTicksPositioner::findZCoordinate(void)
{
  return findLowerZCoordinate();
}
/*---------------------------------------------------------------------------------*/
double LeftYTicksPositioner::findXCoordinate(double zCoordinate)
{
  return findFrontXCoordinate(zCoordinate);
}
/*---------------------------------------------------------------------------------*/

}
