/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSubwin.h"
#include "DrawableSubwinBridge.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
DrawableSubwin::DrawableSubwin(sciPointObj * pObj) : DrawableObject(pObj)
{
  m_pCamera = NULL ;
  // coordinate transformation not already up to date
  m_bNeedCoordUpdate = true;
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
void DrawableSubwin::hasChanged( void )
{
  DrawableObject::hasChanged();
  parentSubwinChanged();

  // subwin has changed and so need coordinates update.
  m_bNeedCoordUpdate = true;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::displaySingleObjs(std::list<sciPointObj *>& singleObjs)
{

  if (containsSubwin(singleObjs))
  {
    // there is the subwindow inside, just draw it
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
    drawSingleObjs(singleObjs);
  }
  else
  {
    showSingleObjs(singleObjs);
  }
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSubwin::draw( void )
{
  initializeDrawing() ;

  // set up camera
  placeCamera();


  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return UNCHANGED;
  }


  drawBox();

  displayChildren() ;

  // draw ticks after otherwise there are some transparency issues
  drawTicks();

  // labels need ticks display before
  displayLabels();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSubwin::show( void )
{
  
  initializeDrawing() ;

  // set up camera
  m_pCamera->show();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return UNCHANGED;
  }

  showBox();

  displayChildren() ;

  // draw ticks after otherwise there are some transparency issues
  showTicks();

  // labels need ticks display before
  displayLabels();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSubwin::redraw(void)
{
  initializeDrawing() ;

  // set up camera
  placeCamera();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return UNCHANGED;
  }

  drawBox();

  displayChildren() ;

  // draw ticks after otherwise there are some transparency issues
  drawTicks();

  // labels need ticks display before
  displayLabels();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
  return SUCCESS;
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
void DrawableSubwin::drawSingleObjs(std::list<sciPointObj *>& singleObjs)
{

  initializeDrawing() ;

  // set up camera
  placeCamera();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return;
  }

  // camera has been set
  // display only the children
  printSingleObjs(singleObjs);

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::showSingleObjs(std::list<sciPointObj *>& singleObjs)
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
  printSingleObjs(singleObjs);

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
bool DrawableSubwin::containsSubwin(std::list<sciPointObj *>& pObjs)
{
  list<sciPointObj *>::iterator it = pObjs.begin();
  for (; it != pObjs.end(); it++)
  {
    if (sciGetEntityType(*it) == SCI_SUBWIN)
    {
      return true;
    }
  }
  return false;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::printSingleObjs(std::list<sciPointObj *>& pObjs)
{
  list<sciPointObj *>::iterator it = pObjs.begin();
  for (; it != pObjs.end(); it++)
  {
    getHandleDrawer(*it)->display();
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::placeCamera(void)
{
  m_pCamera->draw();
  // coordinate transform have been updated
  m_bNeedCoordUpdate = false;
}
/*---------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwin::getSubwinImp( void )
{
  return dynamic_cast<DrawableSubwinBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/

}
