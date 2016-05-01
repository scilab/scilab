/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __H5_ATTRIBUTECONSTATS_H__
#define __H5_ATTRIBUTECONSTATS_H__

#define SOD_FILE_VERSION     3

static const char g_SCILAB_CLASS[]              = "SCILAB_Class";
static const char g_SCILAB_CLASS_EMPTY[]        = "SCILAB_empty";
static const char g_SCILAB_CLASS_VARNAME[]      = "SCILAB_varname";
static const char g_SCILAB_CLASS_COMPLEX[]      = "SCILAB_complex";
static const char g_SCILAB_CLASS_PREC[]         = "SCILAB_precision";
static const char g_SCILAB_CLASS_ROWS[]         = "SCILAB_rows";
static const char g_SCILAB_CLASS_COLS[]         = "SCILAB_cols";
static const char g_SCILAB_CLASS_ITEMS[]        = "SCILAB_items";
static const char g_SCILAB_CLASS_SOD_VERSION[]  = "SCILAB_sod_version";
static const char g_SCILAB_CLASS_SCI_VERSION[]  = "SCILAB_scilab_version";


static const char g_SCILAB_CLASS_DOUBLE[]       = "double";
static const char g_SCILAB_CLASS_STRING[]       = "string";
static const char g_SCILAB_CLASS_BOOLEAN[]      = "boolean";
static const char g_SCILAB_CLASS_INT[]          = "integer";
static const char g_SCILAB_CLASS_POLY[]         = "polynomial";
static const char g_SCILAB_CLASS_SPARSE[]       = "sparse";
static const char g_SCILAB_CLASS_BSPARSE[]      = "boolean sparse";
static const char g_SCILAB_CLASS_LIST[]         = "list";
static const char g_SCILAB_CLASS_TLIST[]        = "tlist";
static const char g_SCILAB_CLASS_MLIST[]        = "mlist";
static const char g_SCILAB_CLASS_VOID[]         = "void";
static const char g_SCILAB_CLASS_UNDEFINED[]    = "undefined";
static const char g_SCILAB_CLASS_STRUCT[]       = "struct";
static const char g_SCILAB_CLASS_CELL[]         = "cell";
static const char g_SCILAB_CLASS_HANDLE[]       = "handle";
static const char g_SCILAB_CLASS_MACRO[]        = "macro";
static const char g_SCILAB_CLASS_USERTYPE[]     = "usertype";

//integer precision
#define SCI_INT8    1
#define SCI_INT16   2
#define SCI_INT32   4
#define SCI_INT64   8
#define SCI_UINT8   11
#define SCI_UINT16  12
#define SCI_UINT32  14
#define SCI_UINT64  18

#endif /* !__H5_ATTRIBUTECONSTATS_H__ */
