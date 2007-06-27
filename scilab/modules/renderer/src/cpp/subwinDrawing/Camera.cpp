/*------------------------------------------------------------------------*/
/* file: Camera.cpp                                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : File used to position viewpoint and rendering zone in the       */
/*        canvas.                                                         */
/*------------------------------------------------------------------------*/

#include "Camera.h"
#include "MALLOC.h"

namespace sciGraphics
{

/*-----------------------------------------------------------------------------------*/
Camera::Camera( void )
{
  m_pImp = NULL;
}
/*-----------------------------------------------------------------------------------*/
Camera::~Camera( void )
{
  if ( m_pImp != NULL )
  {
    delete m_pImp;
  }
  m_pImp = NULL;
}
/*-----------------------------------------------------------------------------------*/
void Camera::setViewingArea( double axesBounds[4], double margins[4] )
{
  double translation[2] ;
  // T = Taxes + SizeAxes * [marginLeft, marginTop}.
  translation[0] = axesBounds[0] + axesBounds[2] * margins[0] ;
  translation[1] = axesBounds[1] + axesBounds[3] * margins[2] ;

  // S = Saxes - margin.Saxes
  // where Saxes = [w,h] and margin = [margin_left+margin_right, margin_top+margin_bottom].
  double scale[2] ;
  scale[0] = (1.0 - margins[0] - margins[1]) * axesBounds[2] ;
  scale[1] = (1.0 - margins[2] - margins[3]) * axesBounds[3] ;
  m_pImp->setViewingArea(translation, scale);
}
/*-----------------------------------------------------------------------------------*/
void Camera::setRotationAngles( double alpha, double theta )
{
  m_pImp->setRotationAngles(alpha, theta);
}
/*-----------------------------------------------------------------------------------*/
void Camera::setSubWinBox( double box[6] )
{
  m_pImp->setSubWinBox(box);
}
/*-----------------------------------------------------------------------------------*/
void Camera::setFarthestDistance( double maxDist )
{
  m_pImp->setFarthestDistance(maxDist);
}
/*-----------------------------------------------------------------------------------*/
void Camera::renderPosition( void )
{
  m_pImp->renderPosition();
}
/*-----------------------------------------------------------------------------------*/

}
