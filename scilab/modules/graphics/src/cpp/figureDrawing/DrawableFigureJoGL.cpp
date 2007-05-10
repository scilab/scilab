/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

#include "DrawableFigureJoGL.h"
extern "C"
{
#include "Xcall1.h"
#include "GetProperty.h"
#include "periScreen.h" /* should be removed */
#include "DrawObjects.h"
#include "getScilabJavaVM.h"
#include "JniUtils.h"
}

#include <stdio.h>

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigureJoGL::DrawableFigureJoGL( DrawableFigure * drawer ) : DrawableFigureImp( drawer )
{
  jniCreateDefaultInstance( "org/scilab/modules/graphics/figureDrawing/DrawableFigureJoGL", &m_oDrawableClass, &m_oDrawableObject) ;
}
/*------------------------------------------------------------------------------------------*/
DrawableFigureJoGL::~DrawableFigureJoGL( void )
{
  closeRenderingCanvas() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::initializeDrawing( void )
{
  sciPointObj * pObj     = m_pDrawer->getDrawedObject() ;
  sciFigure   * ppFigure = pFIGURE_FEATURE( pObj ) ;
  int         background = sciGetBackground (pObj) ;

#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc() ;
#endif

  C2F(dr)("xset","pixmap",&(ppFigure->pixmap),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  if ( ppFigure->pixmap == 0 || GetDriverId() != 0 )
  {
    /* Change background BEFORE xclear F.Leray */
    C2F(dr)("xset","background",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L); 
    C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
  }
  else
  {
    C2F (dr) ("xset","wwpc", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
  }

  C2F(dr)("xset","background",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L); /* Change background F.Leray*/
  C2F(dr)("xset","alufunction",&(sciGetScilabXgc(pObj)->CurDrawFunction),PI0,PI0,PI0,
    PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

#ifdef _MSC_VER
  if ( flag_DO == 1) { ReleaseWinHdc(); }
#endif

  jniCallVoidFunctionSafe( m_oDrawableObject, "initializeDrawing", "" ) ;

}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::endDrawing( void )
{   
  jniCallVoidFunctionSafe( m_oDrawableObject, "endDrawing", "" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::updateInfoMessage( void  )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "updateInfoMessage", "" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::openRenderingCanvas( int figureIndex )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "openRenderingCanvas", "I", figureIndex ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::closeRenderingCanvas( void )
{
  if ( m_oDrawableObject != NULL )
  {
    jniCallVoidFunctionSafe( m_oDrawableObject, "closeRenderingCanvas", "" ) ;
    jniDeleteLocalEntity( m_oDrawableObject ) ;
  }
  m_oDrawableObject = NULL ;
  m_oDrawableClass  = NULL ;
}
/*------------------------------------------------------------------------------------------*/

}
