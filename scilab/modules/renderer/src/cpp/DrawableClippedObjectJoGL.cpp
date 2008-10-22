/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class with driver dependent algorithms of DrawableClippedObject class using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableClippedObjectJoGL.h"
#include "DrawableObjectBridge.h"


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableClippedObjectJoGL::DrawableClippedObjectJoGL( DrawableObject * drawer )
  : DrawableObjectJoGL(drawer)
{
  m_dXmin = 0.0;
  m_dXmax = 0.0;

  m_dYmin = 0.0;
  m_dYmax = 0.0;

  m_dZmin = 0.0;
  m_dZmax = 0.0;
}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::setClipBox( double xMin, double xMax,
                                            double yMin, double yMax,
                                            double zMin, double zMax )
{
  m_dXmin = xMin;
  m_dXmax = xMax;

  m_dYmin = yMin;
  m_dYmax = yMax;

  m_dZmin = zMin;
  m_dZmax = zMax;

}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::clipX( void )
{
  getClippedJavaMapper()->clipX(m_dXmin, m_dXmax);
}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::clipY( void )
{
  getClippedJavaMapper()->clipY(m_dYmin, m_dYmax);
}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::clipZ( void )
{
  getClippedJavaMapper()->clipZ(m_dZmin, m_dZmax);
}
/*---------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::unClip( void )
{
  getClippedJavaMapper()->unClip();
}
/*---------------------------------------------------------------------------------*/
DrawableClippedObject * DrawableClippedObjectJoGL::getClippedObjectDrawer( void )
{
  return dynamic_cast<DrawableClippedObject *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableClippedObjectJavaMapper * DrawableClippedObjectJoGL::getClippedJavaMapper(void)
{
  return dynamic_cast<DrawableClippedObjectJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
