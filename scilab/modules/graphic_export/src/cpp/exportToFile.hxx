/*------------------------------------------------------------------------*/
/* file: exportToFile.hxx                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions to export files                                       */
/*------------------------------------------------------------------------*/

#ifndef _EXPORT_TO_FILE_HXX_
#define _EXPORT_TO_FILE_HXX_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"

/** Enum listing the supported type of export files */
typedef enum
{
  NO_EXPORT = 0,
  BMP_EXPORT = 1,
  GIF_EXPORT = 2,
  JPG_EXPORT = 3,
  PNG_EXPORT = 4,
  PPM_EXPORT = 5,
  EPS_EXPORT = 6,
  PDF_EXPORT = 7,
  SVG_EXPORT = 8,
  PS_EXPORT = 9
} ExportFileType;

void exportToFile(sciPointObj * pFigure, const char * fileName, ExportFileType fileType);

#ifdef __cplusplus
}
#endif

#endif /* _EXPORT_TO_FILE_HXX_ */

