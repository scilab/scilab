/*------------------------------------------------------------------------*/
/* file: SetJavaProperty.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface functions between between SetProperty functions and   */
/*        the C++/Java part of module                                     */
/*------------------------------------------------------------------------*/

#include "SetJavaProperty.h"
#include "getHandleDrawer.h"
#include "figureDrawing/DrawableFigure.h"
#include "subwinDrawing/DrawableSubwin.h"

using namespace sciGraphics ;

/*---------------------------------------------------------------------------------*/
void sciSetJavaColormap( sciPointObj * pFigure, const double rgbMat[], int nbColor )
{
  getFigureDrawer(pFigure)->setColorMap( rgbMat, nbColor ) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaFigureSize( sciPointObj * pFigure, const int size[2] )
{
  getFigureDrawer(pFigure)->setSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaWindowSize( sciPointObj * pFigure, const int size[2] )
{
  getFigureDrawer(pFigure)->setWindowSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaWindowPosition( sciPointObj * pFigure, const int pos[2] )
{
  getFigureDrawer(pFigure)->setWindowPosition(pos) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaInfoMessage( sciPointObj * pFigure, const char * infoMessage )
{
  getFigureDrawer(pFigure)->setInfoMessage(infoMessage);
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaPixmapMode(sciPointObj * pFigure, BOOL onOrOff)
{
  getFigureDrawer(pFigure)->setPixmapMode(onOrOff == TRUE);
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaRenderingEnable(sciPointObj * pFigure, BOOL isEnable)
{
  getFigureDrawer(pFigure)->setRenderingEnable(isEnable == TRUE);
}
/*---------------------------------------------------------------------------------*/
void sciJavaZoomRect(sciPointObj * pSubwin, int posX, int posY, int width, int height)
{
  getSubwinDrawer(pSubwin)->getCamera()->zoomRect(posX, posY, width, height);
}
/*---------------------------------------------------------------------------------*/
void sciJavaExportToFile(sciPointObj * pFigure, const char * fileName, ExportFileType fileType)
{
  getFigureDrawer(pFigure)->exportToFile(fileName, fileType);
}
/*---------------------------------------------------------------------------------*/

