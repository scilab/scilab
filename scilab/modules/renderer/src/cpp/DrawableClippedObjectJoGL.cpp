/*------------------------------------------------------------------------*/
/* file: DrawableClippedObjectJoGL.h                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class with driver dependent algorithms of DrawableClippedObject */
/*        class using JoGL                                                */
/*------------------------------------------------------------------------*/

#include "DrawableClippedObjectJoGL.h"
#include "DrawableObjectImp.h"

extern "C"
{
#include "JniUtils.h"
}


namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableClippedObjectJoGL::DrawableClippedObjectJoGL( DrawableObject * drawer, const char * className )
  : DrawableObjectJoGL(drawer, className)
{
  m_dXmin = 0.0;
  m_dXmax = 0.0;

  m_dYmin = 0.0;
  m_dYmax = 0.0;

  m_dZmin = 0.0;
  m_dZmax = 0.0;
}
/*------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::clipX( void )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "clipX", "(DD)V", m_dXmin, m_dXmax) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::clipY( void )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "clipY", "(DD)V", m_dYmin, m_dYmax) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::clipZ( void )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "clipZ", "(DD)V", m_dZmin, m_dZmax) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableClippedObjectJoGL::unClip( void )
{
  jniCallMemberFunctionSafe(m_oDrawableObject,NULL,"unClip","()V");
}
/*------------------------------------------------------------------------------------------*/
DrawableClippedObject * DrawableClippedObjectJoGL::getClippedObjectDrawer( void )
{
  return dynamic_cast<DrawableClippedObject *>(getDrawer()) ;
}
/*------------------------------------------------------------------------------------------*/
}