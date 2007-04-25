/*------------------------------------------------------------------------*/
/* file: DrawableClippedObject.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class wich represent an object wich can be clipped              */
/*------------------------------------------------------------------------*/

#include "DrawableClippedObject.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableClippedObject::DrawableClippedObject( sciPointObj * drawed )
  : DrawableObject( drawed )
{
  m_bIsClipped = false ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableClippedObject::setClipBox( double xMin, double xMax,
                                        double yMin, double yMax,
                                        double zMin, double zMax )
{
  m_dXmin = xMin ;
  m_dXmax = xMax ;

  m_dYmin = yMin ;
  m_dYmax = yMax ;

  m_dZmin = zMin ;
  m_dZmax = zMax ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableClippedObject::clip( void )
{
  m_bIsClipped = true ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableClippedObject::unClip( void )
{
  m_bIsClipped = false ;
}
/*------------------------------------------------------------------------------------------*/

}
