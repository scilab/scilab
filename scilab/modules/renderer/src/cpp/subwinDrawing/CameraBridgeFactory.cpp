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
  CameraJoGL * newBridge = new CameraJoGL(m_pSubwin) ;

  if (sciGetIsIsoView(m_pSubwin->getDrawedObject()))
  {
    newBridge->setJavaMapper(new IsoViewCameraJavaMapper()) ;
  }
  else
  {
    newBridge->setJavaMapper(new IsometricCameraJavaMapper()) ;
  }
  
  return newBridge;
}
/*------------------------------------------------------------------------*/

}
