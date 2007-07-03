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
  m_dAlpha = alpha ;
  m_dTheta = theta ;
}
/*-----------------------------------------------------------------------------------*/
void Camera::setSubwinBox( double bounds[6] )
{
  for ( int i = 0 ; i < 6 ; i++ )
  {
    m_aAxesBox[i] = bounds[i] ;
  }
  
  double scale[3] ;
  // 1.0 / ( Xmax - Xmin )
  scale[0] = 1.0 / (bounds[1] - bounds[0]) ;
  scale[1] = 1.0 / (bounds[3] - bounds[2]) ;
  scale[2] = 1.0 / (bounds[5] - bounds[4]) ;
  m_pImp->setAxesScale(scale) ;

  double trans[3];
  trans[0] = -bounds[0] ;
  trans[1] = -bounds[2] ;
  trans[2] = bounds[4] ;
  m_pImp->setAxesTranslation(trans) ;
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
void Camera::getSubwinCenter( double center[3] )
{
  // middle of the box
  center[0] = (m_aAxesBox[0] + m_aAxesBox[1]) / 2.0 ;
  center[1] = (m_aAxesBox[2] + m_aAxesBox[3]) / 2.0 ;
  center[2] = (m_aAxesBox[4] + m_aAxesBox[5]) / 2.0 ;
}
/*-----------------------------------------------------------------------------------*/


}
