 /*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Class containing the drawing routine for a figure object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFigure.h"
#include "DrawableFigureBridge.h"
#include "../subwinDrawing/DrawableSubwin.h"
#include "../getHandleDrawer.h"

// here for static methods
#include "DrawableFigureJoGL.h"

//#include <time.h>
//#include <sched.h>
//#include <iostream>

extern "C"
{
#include "GetProperty.h"
}


// struct timespec t_t1 ;
// struct timespec t_t2 ;
// struct timespec t_t3 ;
// struct timespec t_t4 ;

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFigure::DrawableFigure( sciPointObj * pObj )
  : DrawableObject( pObj )
{
  m_bNeedRedraw = false ; // no display lists for figure
  m_pSynchronizer = NULL;
}
/*---------------------------------------------------------------------------------*/
DrawableFigure::~DrawableFigure( void )
{
  closeRenderingCanvas() ;
  if ( m_pSynchronizer != NULL )
  {
    delete m_pSynchronizer;
    m_pSynchronizer = NULL;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setColorMap( const double rgbMat[], int nbColor )
{
  getFigureImp()->setColorMap( rgbMat, nbColor ) ;

  // we need to redraw every object to take new colors into account.
  familyHasChanged();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getColorMap( double rgbMat[] )
{
  getFigureImp()->getColorMap( rgbMat ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getSize( int size[2] )
{
  getFigureImp()->getSize(size) ;
}
/*---------------------------------------------------------------------------------*/
int DrawableFigure::setSize( const int size[2] )
{
  return getFigureImp()->setSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getWindowSize( int size[2] )
{
  getFigureImp()->getWindowSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setWindowSize( const int size[2] )
{
  getFigureImp()->setWindowSize(size);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::openRenderingCanvas( void )
{
  getFigureImp()->openRenderingCanvas( sciGetNum( m_pDrawed ) ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::closeRenderingCanvas( void )
{
  getFigureImp()->closeRenderingCanvas() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::drawBackground(void)
{
  getFigureImp()->drawBackground();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::drawInContext( void )
{
  
  // retest on auto_redraw
  // because display may come directly from JoGL
  //clock_gettime(0, &t_t2);
  if (isDisplayingSingleObject())
  {
    initializeDrawing() ;
    setFigureParameters() ;
    displaySingleObject();
    endDrawing() ;
  }
  else if ( checkVisibility() )
  {
    if (checkAutoRedraw())
    {
      initializeDrawing() ;
      drawBackground();
      setFigureParameters() ;
      displayChildren() ;
      endDrawing() ;
    }
  }
  else
  {
    // clear figure, just draw background
    initializeDrawing() ;
    drawBackground();
    endDrawing() ;

  }
  //clock_gettime(0, &t_t3);

  //double timeSpent = (double) (t_toc.tv_sec - t_tic.tv_sec) + (double) (t_toc.tv_nsec - t_tic.tv_nsec) * 1.0E-9;
  //std::cout << "BB " << timeSpent * 1000.0 << std::endl;

}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::redrawInContext( void )
{
  // force update of all the graphic hierarchy
  familyHasChanged();
  // draw as usual
  //drawInContext();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::redrawSubwins(void)
{
  // just call the function on children
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  while ( curSon != NULL )
  {
    if (sciGetEntityType(curSon->pointobj) == SCI_SUBWIN)
    {
      getSubwinDrawer( curSon->pointobj )->parentSubwinChanged();
    }
    curSon = curSon->pprev ;
  }
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableFigure::draw( void )
{ 
  if ( !checkAutoRedraw() && !isDisplayingSingleObject() )
  {
    // if a single object is available this override drawlater()/ drawnow()
    return UNCHANGED;
  }

  // make sure the context is created
 
  //clock_gettime(0, &t_t1);
  drawCanvas() ;
  return SUCCESS;
  //clock_gettime(0, &t_t4);

//   double aa = (double) (t_t4.tv_sec - t_t1.tv_sec)
//     + (double) (t_t4.tv_nsec - t_t1.tv_nsec) * 1.0E-9;

//   double bb = (double) (t_t3.tv_sec - t_t2.tv_sec)
//     + (double) (t_t3.tv_nsec - t_t2.tv_nsec) * 1.0E-9;

//   double cc = (double) (t_t2.tv_sec - t_t1.tv_sec + t_t4.tv_sec - t_t3.tv_sec)
//     + (double) (t_t2.tv_nsec - t_t1.tv_nsec + t_t4.tv_nsec - t_t3.tv_nsec) * 1.0E-9;

//   std::cout << "AA " << aa * 1000.0 << std::endl;
//   std::cout << "BB " << bb * 1000.0 << std::endl;
//   std::cout << "CC " << cc * 1000.0 << std::endl;

}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableFigure::show( void )
{
  return draw() ;
}
/*---------------------------------------------------------------------------------*/
DrawableFigureBridge * DrawableFigure::getFigureImp( void )
{
  return dynamic_cast<DrawableFigureBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::drawCanvas( void )
{
  getFigureImp()->drawCanvas() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setInfoMessage( const char * infoMessage )
{
  getFigureImp()->setInfoMessage(infoMessage) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setFigureParameters( void )
{
  getFigureImp()->setFigureParameters() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getWindowPosition( int pos[2] )
{
  getFigureImp()->getWindowPosition(pos) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setWindowPosition( const int pos[2] )
{
  getFigureImp()->setWindowPosition(pos) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setAutoResizeMode(bool onOrOff)
{
  getFigureImp()->setAutoResizeMode(onOrOff);
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigure::getAutoResizeMode(void)
{
  return getFigureImp()->getAutoResizeMode();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getViewport(int viewport[4])
{
  getFigureImp()->getViewport(viewport);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setViewport(const int viewport[4])
{
  getFigureImp()->setViewport(viewport);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setBackgroundColor(int backColor)
{
  getFigureImp()->setBackgroundColor(backColor);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::rubberBox(bool isClick, bool isZoom, const int initialRect[4], int endRect[4], int * usedButton)
{
  getFigureImp()->rubberBox(isClick, isZoom, initialRect, endRect, usedButton);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setTitle( const char * title )
{
  getFigureImp()->setTitle(title) ;
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigure::getRotationDisplacement(int displacement[2])
{
  return getFigureImp()->getRotationDisplacement(displacement);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::stopRotationRecording(void)
{
  return getFigureImp()->stopRotationRecording();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::showWindow(void)
{
  return getFigureImp()->showWindow();
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigure::isAbleToCreateFigure(void)
{
  return DrawableFigureJoGL::isAbleToCreateFigure();
}
/*---------------------------------------------------------------------------------*/
}
