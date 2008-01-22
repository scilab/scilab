/*------------------------------------------------------------------------*/
/* file: exportToFile.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions to export files                                       */
/*------------------------------------------------------------------------*/

#include "exportToFile.hxx"
#include "FileExporter.hxx"


extern "C"
{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
}



/*---------------------------------------------------------------------------------*/
void exportToFile(sciPointObj * pFigure, const char * fileName, ExportFileType fileType)
{

  org_scilab_modules_graphic_export::FileExporter::fileExport(getScilabJavaVM(),
                                                              sciGetNum(pFigure),
                                                              (char *)fileName,
                                                              fileType);
}
/*---------------------------------------------------------------------------------*/
