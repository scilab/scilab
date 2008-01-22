/*------------------------------------------------------------------------*/
/* file: xs2file.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2file routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _XS2FILE_H_
#define _XS2FILE_H_

#include "ObjectStructure.h"
#include "exportTofile.hxx"

/**
 * Generic function to create either ps, fig, gif or ppm files.
 *
 * @param[in] fname     name of the routine (ie xs2ps, xs2fig,...)
 * @param fileType type of the export (ie JPG_EXPORT, BMP_EXPORT, PS_EXPORT, ...).
 * @return 0 if success, -1 otherwise
 */
int xs2file(char * fname, ExportFileType fileType) ;


#endif /* _INT_XS2FILE_H_ */
