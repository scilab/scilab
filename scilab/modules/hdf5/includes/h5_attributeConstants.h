/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __H5_ATTRIBUTECONSTATS_H__
#define __H5_ATTRIBUTECONSTATS_H__

#define SOD_FILE_VERSION     2

static const char g_SCILAB_CLASS[]              =	"SCILAB_Class";
static const char g_SCILAB_CLASS_EMPTY[]        =	"SCILAB_empty";
static const char g_SCILAB_CLASS_VARNAME[]      =	"SCILAB_varname";
static const char g_SCILAB_CLASS_COMPLEX[]      =	"SCILAB_complex";
static const char g_SCILAB_CLASS_PREC[]         =	"SCILAB_precision";
static const char g_SCILAB_CLASS_ROWS[]         =	"SCILAB_rows";
static const char g_SCILAB_CLASS_COLS[]         =	"SCILAB_cols";
static const char g_SCILAB_CLASS_ITEMS[]        =	"SCILAB_items";
static const char g_SCILAB_CLASS_SOD_VERSION[] =	"SCILAB_sod_version";
static const char g_SCILAB_CLASS_SCI_VERSION[]  =	"SCILAB_scilab_version";


static const char g_SCILAB_CLASS_DOUBLE[]       =	"double";
static const char g_SCILAB_CLASS_STRING[]       =	"string";
static const char g_SCILAB_CLASS_BOOLEAN[]      =	"boolean";
static const char g_SCILAB_CLASS_INT[]          =	"integer";
static const char g_SCILAB_CLASS_POLY[]         =	"polynomial";
static const char g_SCILAB_CLASS_SPARSE[]       =	"sparse";
static const char g_SCILAB_CLASS_BSPARSE[]      =	"boolean sparse";
static const char g_SCILAB_CLASS_LIST[]         =	"list";
static const char g_SCILAB_CLASS_TLIST[]        =	"tlist";
static const char g_SCILAB_CLASS_MLIST[]        =	"mlist";
static const char g_SCILAB_CLASS_VOID[]         =	"void";
static const char g_SCILAB_CLASS_UNDEFINED[]    =	"undefined";

//integer precision
#define SCI_INT8	1
#define SCI_INT16	2
#define SCI_INT32	4
#define SCI_INT64	8
#define SCI_UINT8	11
#define SCI_UINT16	12
#define SCI_UINT32	14
#define SCI_UINT64	18

#endif /* !__H5_ATTRIBUTECONSTATS_H__ */
