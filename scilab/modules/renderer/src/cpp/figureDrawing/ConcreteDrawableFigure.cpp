/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableFigure.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independent algorithms to draw a    */
/*        figure handle                                                   */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawableFigure.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFigure::exportInContext(const char * fileName, ExportFileType fileType)
{
  // fill the buffer
  drawInContext();

  switch (fileType)
  {
    case BMP_EXPORT:
    case GIF_EXPORT:
    case PNG_EXPORT:
    case JPG_EXPORT:
    case PPM_EXPORT:
      // bitmap export
      getFigureImp()->exportToBitmapFile(fileName, fileType);
      break;
    default:
      // vectorial export
      break;
  }
  
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::checkAutoRedraw( void )
{
  return ( pFIGURE_FEATURE(m_pDrawed)->auto_redraw == TRUE ) ;
}
/*---------------------------------------------------------------------------------*/

}
