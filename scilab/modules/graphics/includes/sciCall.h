/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Djalel Abdemouche
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------*/
/* file: sciCall.h                                                        */
/* desc : Graphic subroutines interface                                   */
/*------------------------------------------------------------------------*/

#ifndef _SCI_CALL_H_
#define _SCI_CALL_H_

#include "dynlib_graphics.h"
#include "BOOL.h"
#include "BuildObjects.h"

GRAPHICS_IMPEXP void Objrect(double* x, double* y, double* width, double* height, int* foreground, int* background, BOOL isfilled, BOOL isline, long* hdl);

GRAPHICS_IMPEXP void Objarc(double* angle1, double* angle2, double* x, double* y, double* width, double* height, int* foreground, int* background, BOOL isfilled, BOOL isline, long* hdl);

GRAPHICS_IMPEXP void Objpoly(double* x, double* y, int n, int closed, int mark, long* hdl);

GRAPHICS_IMPEXP void Objfpoly(double* x, double* y, int n, int* style, long* hdl, int shading);

GRAPHICS_IMPEXP void Objsegs(int* style, int flag , int n1, double* x, double* y, double* z, double arsize);

GRAPHICS_IMPEXP void Objstring(char** fname, int nbRow, int nbCol, double  x, double  y, double* angle, double  box[4], BOOL autoSize, double userSize[2], long* hdl, int centerPos, int* foreground, int* background, BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment alignment);

GRAPHICS_IMPEXP void Objplot2d(int ptype, char logflags[], double x[], double y[], int* n1, int* n2, int style[], char strflag[], char legend[], double brect[], int aaint[], BOOL flagNax);

GRAPHICS_IMPEXP void Objgrayplot(double x[], double y[], double z[], int* n1, int* n2, char strflag[], double brect[], int aaint[], BOOL flagNax, char logflag[]);

GRAPHICS_IMPEXP void Objmatplot(double z[], int* n1, int* n2, char strflag[], double brect[], int aaint[], BOOL flagNax);

GRAPHICS_IMPEXP void ObjmatplotImage(void * z, int* n1, int* n2, char strflag[], double brect[], int aaint[], BOOL flagNax, int plottype);

GRAPHICS_IMPEXP void Objmatplot1(double z[], int* n1, int* n2, double xrect[], int plottype);

GRAPHICS_IMPEXP void Objplot3d(char* fname, int* isfac, int* izcol, double x[], double y[], double z[], double* zcol, int* m, int* n, double* theta, double* alpha, char* legend, int* iflag, double* ebox, int* m1, int* n1, int* m2, int* n2, int* m3, int* n3, int* m3n, int* n3n);

GRAPHICS_IMPEXP void Objdrawaxis(char dir, char tics, double* x, int* nx, double* y, int* ny, char* val[], int subint, char* format, int font, int textcol, int ticscol, char flag, int seg, int nb_tics_labels);

GRAPHICS_IMPEXP void Objnumb(char* fname, unsigned long fname_len, int n, int flag, double x, double y, double* angle, double* box);

GRAPHICS_IMPEXP void Objfec(double x[], double y[], double noeud[], double* fun, int* n, int* m, int* p, char strflag[], char legend[], double brect[], int aaint[], double Zminmax[], int Colminmax[], int ColOut[], BOOL WithMesh, BOOL flagNax);

#endif /* _SCI_CALL_H_ */
