/*------------------------------------------------------------------------*/
/* file: CameraBridgeFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for CameraBridge class                                  */
/*------------------------------------------------------------------------*/

#include "CameraBridgeFactory.h"
#include "CameraJoGL.h"
#include "IsoViewCameraJavaMapper.hxx"
#include "IsometricCameraJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------*/
CameraBridge * CameraBridgeFactory::create( void )
{
  CameraJoGL * newBridge = new CameraJoGL(m_pCamera) ;

  CameraJavaMapper * javaMapper;

  sciPointObj * pSubwin = m_pCamera->getDrawedObject();

  if (sciGetIsIsoView(pSubwin))
  {
    javaMapper = new IsoViewCameraJavaMapper() ;
  }
  else
  {
    javaMapper = new IsometricCameraJavaMapper() ;
  }

  newBridge->setJavaMapper(javaMapper);
  
  return newBridge;
}
/*------------------------------------------------------------------------*/

}
