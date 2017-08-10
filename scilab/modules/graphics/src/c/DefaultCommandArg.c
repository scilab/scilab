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
/* file: DefaultCommandArg.c                                              */
/* desc : access to default values for graphics arguments                 */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "DefaultCommandArg.h"

/*------------------------------------------------------------------------*/
/* The default values */
static double def_rect[4]      = {0.0, 0.0, 0.0, 0.0};
static char   def_strf[4]      = DEFSTRF          ;
static char   def_legend[]     = "X@Y@Z"          ;
static double def_zminmax[2]   = {0., 0.}          ;
static int    def_nax[4]       = {2, 10, 2, 10}      ;
static int    def_colminmax[2] = {0, 0}            ;
static int    def_colout[2]    = { -1, -1}          ;
static BOOL   def_with_mesh    = FALSE            ;
static char   def_logflags[4]  = DEFLOGFLAGS      ;
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
double* getDefRect(void)
{
    return def_rect ;
}
/*------------------------------------------------------------------------*/
char * getDefStrf(void)
{
    return def_strf ;
}
/*------------------------------------------------------------------------*/
char * getDefLegend(void)
{
    return def_legend ;
}
/*------------------------------------------------------------------------*/
double* getDefZminMax(void)
{
    return def_zminmax ;
}
/*------------------------------------------------------------------------*/
int * getDefNax(void)
{
    return def_nax ;
}
/*------------------------------------------------------------------------*/
int * getDefColMinMax(void)
{
    return def_colminmax ;
}
/*------------------------------------------------------------------------*/
int * getDefColOut(void)
{
    return def_colout ;
}
/*------------------------------------------------------------------------*/
BOOL getDefWithMesh(void)
{
    return def_with_mesh ;
}
/*------------------------------------------------------------------------*/
char * getDefLogFlags(void)
{
    return def_logflags ;
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
void setDefRect(const double newDef[4])
{
    def_rect[0] = newDef[0];
    def_rect[1] = newDef[1];
    def_rect[2] = newDef[2];
    def_rect[3] = newDef[3];
}
/*------------------------------------------------------------------------*/
void setDefStrf(const char newDef[4])
{
    def_strf[0] = newDef[0];
    def_strf[1] = newDef[1];
    def_strf[2] = newDef[2];
    /* last element should be /0 */
}
/*------------------------------------------------------------------------*/
void setDefZminMax(const double newDef[2])
{
    def_zminmax[0] = newDef[0];
    def_zminmax[1] = newDef[1];
}
/*------------------------------------------------------------------------*/
void setDefNax(const int newDef[4])
{
    def_nax[0] = newDef[0];
    def_nax[1] = newDef[1];
    def_nax[2] = newDef[2];
    def_nax[3] = newDef[3];
}
/*------------------------------------------------------------------------*/
void setDefColMinMax(const int  newDef[2])
{
    def_colminmax[0] = newDef[0];
    def_colminmax[1] = newDef[1];
}
/*------------------------------------------------------------------------*/
void setDefColOut(const int newDef[2])
{
    def_colout[0] = newDef[0];
    def_colout[1] = newDef[1];
}
/*------------------------------------------------------------------------*/
void setDefWithMesh(BOOL newDef)
{
    def_with_mesh = newDef;
}
/*------------------------------------------------------------------------*/
void setDefLogFlags(const char newDef[4])
{
    def_logflags[0] = newDef[0];
    def_logflags[1] = newDef[1];
    def_logflags[2] = newDef[2];
    /* last element should be /0 */
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
void reinitDefStrf(void)
{
    strcpy(def_strf, DEFSTRF);
}
/*------------------------------------------------------------------------*/
void reinitDefStrfN(void)
{
    strcpy(def_strf, DEFSTRFN);
}
/*------------------------------------------------------------------------*/
void reinitDefLogFlags(void)
{
    strcpy(def_logflags, DEFLOGFLAGS);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
BOOL isDefRect(const double* rect)
{
    return rect == def_rect;
}
/*------------------------------------------------------------------------*/
BOOL isDefStrf(const char * strf)
{
    return strf == def_strf;
}
/*------------------------------------------------------------------------*/
BOOL isDefLegend(const char * legend)
{
    return legend == def_legend;
}
/*------------------------------------------------------------------------*/
BOOL isDefZminMax(const double* zminmax)
{
    return zminmax == def_zminmax;
}
/*------------------------------------------------------------------------*/
BOOL isDefNax(const int * nax)
{
    return nax == def_nax;
}
/*------------------------------------------------------------------------*/
BOOL isDefColMinMax(const int * colminmax)
{
    return colminmax == def_colminmax;
}
/*------------------------------------------------------------------------*/
BOOL isDefColOut(const int * colout)
{
    return colout == def_colout;
}
/*------------------------------------------------------------------------*/
BOOL isDefWithMesh(const BOOL withmesh)
{
    return withmesh == def_with_mesh;
}
/*------------------------------------------------------------------------*/
BOOL isDefLogFlags(const char * logflags)
{
    return logflags == def_logflags;
}
/*------------------------------------------------------------------------*/
