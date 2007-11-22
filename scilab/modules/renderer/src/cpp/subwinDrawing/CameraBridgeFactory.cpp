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
#include "XAxisReverseJoGL.hxx"
#include "YAxisReverseJoGL.hxx"
#include "ZAxisReverseJoGL.hxx"

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

  if (sciGetXAxisReverse(pSubwin))
  {
    XAxisReverseJoGL * axesReverseStrategy = new XAxisReverseJoGL();
    axesReverseStrategy->setJavaMapper(javaMapper);
    newBridge->addAxesReverseStrategy(axesReverseStrategy);
  }

  if (sciGetYAxisReverse(pSubwin))
  {
    YAxisReverseJoGL * axesReverseStrategy = new YAxisReverseJoGL();
    axesReverseStrategy->setJavaMapper(javaMapper);
    newBridge->addAxesReverseStrategy(axesReverseStrategy);
  }

  if (sciGetZAxisReverse(pSubwin))
  {
    ZAxisReverseJoGL * axesReverseStrategy = new ZAxisReverseJoGL();
    axesReverseStrategy->setJavaMapper(javaMapper);
    newBridge->addAxesReverseStrategy(axesReverseStrategy);
  }

  newBridge->setJavaMapper(javaMapper);
  
  return newBridge;
}
/*------------------------------------------------------------------------*/

}
