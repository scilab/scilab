/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: DefaultCommandArg.h                                              */
/* desc : access to default values for graphics arguments                 */
/*------------------------------------------------------------------------*/

#ifndef _DEFAULT_COMMAND_ARG_H_
#define _DEFAULT_COMMAND_ARG_H_

#include "dynlib_graphics.h"
#include "BOOL.h"

/*------------------------------------------------------------------------*/
/* real default */
#define DEFSTRF "081"
#define DEFSTRFN "099"
#define DEFLOGFLAGS "gnn"

/*------------------------------------------------------------------------*/
/* Accessors to to default values */
GRAPHICS_IMPEXP double* getDefRect(void);
GRAPHICS_IMPEXP char* getDefStrf(void);
GRAPHICS_IMPEXP char* getDefLegend(void);
GRAPHICS_IMPEXP double* getDefZminMax(void);
GRAPHICS_IMPEXP int* getDefNax(void);
GRAPHICS_IMPEXP int* getDefColMinMax(void);
GRAPHICS_IMPEXP int* getDefColOut(void);
GRAPHICS_IMPEXP BOOL getDefWithMesh(void);
GRAPHICS_IMPEXP char* getDefLogFlags(void);
/*------------------------------------------------------------------------*/
/* set default values by copying the newDef */
GRAPHICS_IMPEXP void setDefRect(const double newDef[4]);
GRAPHICS_IMPEXP void setDefStrf(const char newDef[4]);
GRAPHICS_IMPEXP void setDefZminMax(const double newDef[2]);
GRAPHICS_IMPEXP void setDefNax(const int newDef[4]);
GRAPHICS_IMPEXP void setDefColMinMax(const int newDef[2]);
GRAPHICS_IMPEXP void setDefColOut(const int newDef[2]);
GRAPHICS_IMPEXP void setDefWithMesh(BOOL newDef);
GRAPHICS_IMPEXP void setDefLogFlags(const char newDef[4]);
/*------------------------------------------------------------------------*/
/* change values */
GRAPHICS_IMPEXP void reinitDefStrf(void);
GRAPHICS_IMPEXP void reinitDefStrfN(void);
GRAPHICS_IMPEXP void reinitDefLogFlags(void);
/*------------------------------------------------------------------------*/
/* check if we are dealing with the default value */
GRAPHICS_IMPEXP BOOL isDefRect(const double* rect);
GRAPHICS_IMPEXP BOOL isDefStrf(const char* strf);
GRAPHICS_IMPEXP BOOL isDefLegend(const char* legend);
GRAPHICS_IMPEXP BOOL isDefZminMax(const double* zminmax);
GRAPHICS_IMPEXP BOOL isDefNax(const int* nax);
GRAPHICS_IMPEXP BOOL isDefColMinMax(const int* colminmax);
GRAPHICS_IMPEXP BOOL isDefColOut(const int* colout);
GRAPHICS_IMPEXP BOOL isDefWithMesh(const BOOL withmesh);
GRAPHICS_IMPEXP BOOL isDefLogFlags(const char* logflags);
/*------------------------------------------------------------------------*/

#endif /* _DEFAULT_COMMAND_ARG_H_ */
