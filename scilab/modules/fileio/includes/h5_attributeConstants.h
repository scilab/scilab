/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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

//#define SCILAB_CLASS			"SCILAB_Class"
//#define SCILAB_CLASS_DOUBLE		"double"
//#define SCILAB_CLASS_STRING		"string"
//#define SCILAB_CLASS_LIST		"list"
//#define SCILAB_CLASS_TLIST		"tlist"
//#define SCILAB_CLASS_MLIST		"mlist"

static const char g_SCILAB_CLASS[]					=	"SCILAB_Class";
static const char g_SCILAB_CLASS_EMPTY[]		=	"SCILAB_empty";
static const char g_SCILAB_CLASS_VARNAME[]	=	"SCILAB_varname";
static const char g_SCILAB_CLASS_COMPLEX[]	=	"SCILAB_complex";
static const char g_SCILAB_CLASS_PREC[]			=	"SCILAB_precision";
static const char g_SCILAB_CLASS_ROWS[]			=	"SCILAB_sparse_rows";
static const char g_SCILAB_CLASS_COLS[]			=	"SCILAB_sparse_cols";


static const char g_SCILAB_CLASS_DOUBLE[]		=	"double";
static const char g_SCILAB_CLASS_STRING[]		=	"string";
static const char g_SCILAB_CLASS_BOOLEAN[]	=	"boolean";
static const char g_SCILAB_CLASS_INT[]			=	"integer";
static const char g_SCILAB_CLASS_POLY[]			=	"polynomial";
static const char g_SCILAB_CLASS_SPARSE[]		=	"sparse";
static const char g_SCILAB_CLASS_BSPARSE[]	=	"boolean sparse";
static const char g_SCILAB_CLASS_LIST[]			=	"list";
static const char g_SCILAB_CLASS_TLIST[]		=	"tlist";
static const char g_SCILAB_CLASS_MLIST[]		=	"mlist";

//interger precision
#define SCI_INT8		1
#define SCI_INT16		2
#define SCI_INT32		4
#define SCI_INT64		8

#endif /* !__H5_ATTRIBUTECONSTATS_H__ */
