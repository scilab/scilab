/*------------------------------------------------------------------------*/
/* file: DrawableSubwin.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a subwin object        */
/*------------------------------------------------------------------------*/

#include "DrawableSubwin.h"
#include "DrawableSubwinBridge.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSubwin::DrawableSubwin(sciPointObj * pObj) : DrawableObject(pObj)
{
  m_pCamera = NULL ;
}
/*---------------------------------------------------------------------------------*/
DrawableSubwin::~DrawableSubwin( void )
{
  if ( m_pCamera != NULL )
  {
    delete m_pCamera ;
    m_pCamera = NULL ;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::setCamera( Camera * cam )
{
  if (m_pCamera != NULL)
  {
    delete m_pCamera;
    m_pCamera = NULL;
  }
  m_pCamera = cam;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::displaySingleObj(sciPointObj * pObj)
{

  if (sciGetEntityType(pObj) == SCI_SUBWIN)
  {
    if (m_bNeedRedraw)
    {
      draw();
    }
    else
    {
      show();
    }
  }
  // it is a child
  else if (m_bNeedRedraw)
  {
    drawSingleObj(pObj);
  }
  else
  {
    showSingleObj(pObj);
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::draw( void )
{
  initializeDrawing() ;

  // fill Frect with real data bounds
  computeRealDataBounds();

  // set up camera
  // so update coordinates transformations
  m_pCamera->draw();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return;
  }

  drawAxesBox();

  displayChildren() ;

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::show( void )
{
  
  initializeDrawing() ;

  // set up camera
  // so update coordinates transformations
  m_pCamera->show();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return;
  }

  showAxesBox();

  displayChildren() ;

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::drawAxesBox(void)
{
  drawBox();
  drawTicks();
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::showAxesBox(void)
{
  showBox();
  showTicks();
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::drawSingleObj(sciPointObj * pObj)
{

  initializeDrawing() ;

  // fill Frect with real data bounds
  computeRealDataBounds();

  // set up camera
  m_pCamera->draw();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return;
  }

  // camera has been set
  // display only the children
  getHandleDrawer(pObj)->display();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::showSingleObj(sciPointObj * pObj)
{
  initializeDrawing() ;

  // set up camera
  m_pCamera->show();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return;
  }

  // camera has been set
  // display only the children
  getHandleDrawer(pObj)->display();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwin::getSubwinImp( void )
{
  return dynamic_cast<DrawableSubwinBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/

}
