/*------------------------------------------------------------------------*/
/* file: xs2file.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2file routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _XS2FILE_H_
#define _XS2FILE_H_

#include "machine.h"
#include "ObjectStructure.h"

/** Enum listing the supported type of export files */
typedef enum
{
  NO_EXPORT = 0,
  BMP_EXPORT = 1,
  GIF_EXPORT = 2,
  JPG_EXPORT = 3,
  PNG_EXPORT = 4,
  PPM_EXPORT = 5
} ExportFileType;


/**
 * Generic function to create either ps, fig, gif or ppm files.
 *
 * @param[in] fname     name of the routine (ie xs2ps, xs2fig,...)
 * @param fileType type of the export (ie JPG_EXPORT, BMP_EXPORT, PS_EXPORT, ...).
 * @return 0 if success, -1 otherwise
 */
int xs2file(char * fname, ExportFileType fileType) ;

/**
 * send recorded graphics to file bufname in ``driver'' syntax ( Pos or Fig )
 * @param win_num the number of the window,
 * @param colored 1 if color is wanted
 * @param[in] bufname string the name of the file 
 * @param[in] driver driver for code generation 
 */
int scig_toPs( integer win_num, integer colored, char * bufname, char * driver ) ;

int C2F(xg2psofig)(char *fname, integer *len, integer *iwin, integer *color, char *driver, long int l1, long int l2) ;

void exportToFile(sciPointObj * pFigure, const char * fileName, ExportFileType fileType);

#endif /* _INT_XS2FILE_H_ */
