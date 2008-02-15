/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "GL2PSConstant.hxx"

#include "gl2ps/gl2ps.h"


/* Version number 
#define GL2PS_MAJOR_VERSION 1
#define GL2PS_MINOR_VERSION 3
#define GL2PS_PATCH_VERSION 2
#define GL2PS_EXTRA_VERSION ""*/


int get_GL2PS_MAJOR_VERSION(void){
	return GL2PS_MAJOR_VERSION;
}

int get_GL2PS_MINOR_VERSION(void){
	return GL2PS_MINOR_VERSION;
}

int get_GL2PS_PATCH_VERSION(void){
	return GL2PS_PATCH_VERSION;
}

char *get_GL2PS_EXTRA_VERSION(void){
	return GL2PS_EXTRA_VERSION;
}

/* Output file formats (the values and the ordering are important!) 
#define GL2PS_PS  0
#define GL2PS_EPS 1
#define GL2PS_TEX 2
#define GL2PS_PDF 3
#define GL2PS_SVG 4
#define GL2PS_PGF 5*/

int get_GL2PS_PS(void){
	return GL2PS_PS;
}

int get_GL2PS_EPS(void){
	return GL2PS_EPS;
}

int get_GL2PS_TEX(void){
	return GL2PS_TEX;
}

int get_GL2PS_PDF(void){
	return GL2PS_PDF;
}

int get_GL2PS_SVG(void){
	return GL2PS_SVG;
}

int get_GL2PS_PGF(void){
	return GL2PS_PGF;
}

/* Sorting algorithms 
#define GL2PS_NO_SORT     1
#define GL2PS_SIMPLE_SORT 2
#define GL2PS_BSP_SORT    3*/

int get_GL2PS_NO_SORT(void){
	return GL2PS_NO_SORT;
}

int get_GL2PS_SIMPLE_SORT(void){
	return GL2PS_SIMPLE_SORT;
}

int get_GL2PS_BSP_SORT(void){
	return GL2PS_BSP_SORT;
}

/* Message levels and error codes 
#define GL2PS_SUCCESS       0
#define GL2PS_INFO          1
#define GL2PS_WARNING       2
#define GL2PS_ERROR         3
#define GL2PS_NO_FEEDBACK   4
#define GL2PS_OVERFLOW      5
#define GL2PS_UNINITIALIZED 6*/

int get_GL2PS_SUCCESS(void){
	return GL2PS_SUCCESS;
}

int get_GL2PS_INFO(void){
	return GL2PS_INFO;
}

int get_GL2PS_WARNING(void){
	return GL2PS_WARNING;
}

int get_GL2PS_ERROR(void){
	return GL2PS_ERROR;
}

int get_GL2PS_NO_FEEDBACK(void){
	return GL2PS_NO_FEEDBACK;
}

int get_GL2PS_OVERFLOW(void){
	return GL2PS_OVERFLOW;
}

int get_GL2PS_UNINITIALIZED(void){
	return GL2PS_UNINITIALIZED;
}


/* Options for gl2psBeginPage 
#define GL2PS_NONE                 0
#define GL2PS_DRAW_BACKGROUND      (1<<0)
#define GL2PS_SIMPLE_LINE_OFFSET   (1<<1)
#define GL2PS_SILENT               (1<<2)
#define GL2PS_BEST_ROOT            (1<<3)
#define GL2PS_OCCLUSION_CULL       (1<<4)
#define GL2PS_NO_TEXT              (1<<5)
#define GL2PS_LANDSCAPE            (1<<6)
#define GL2PS_NO_PS3_SHADING       (1<<7)
#define GL2PS_NO_PIXMAP            (1<<8)
#define GL2PS_USE_CURRENT_VIEWPORT (1<<9)
#define GL2PS_COMPRESS             (1<<10)
#define GL2PS_NO_BLENDING          (1<<11)
#define GL2PS_TIGHT_BOUNDING_BOX   (1<<12)*/

int get_GL2PS_NONE(void){
	return GL2PS_NONE;
}

int get_GL2PS_DRAW_BACKGROUND(void){
	return GL2PS_DRAW_BACKGROUND;
}

int get_GL2PS_SIMPLE_LINE_OFFSET(void){
	return GL2PS_SIMPLE_LINE_OFFSET;
}

int get_GL2PS_SILENT(void){
	return GL2PS_SILENT;
}

int get_GL2PS_BEST_ROOT(void){
	return GL2PS_BEST_ROOT;
}

int get_GL2PS_OCCLUSION_CULL(void){
	return GL2PS_OCCLUSION_CULL;
}

int get_GL2PS_NO_TEXT(void){
	return GL2PS_NO_TEXT;
}

int get_GL2PS_LANDSCAPE(void){
	return GL2PS_LANDSCAPE;
}

int get_GL2PS_NO_PS3_SHADING(void){
	return GL2PS_NO_PS3_SHADING;
}

int get_GL2PS_NO_PIXMAP(void){
	return GL2PS_NO_PIXMAP;
}

int get_GL2PS_USE_CURRENT_VIEWPORT(void){
	return GL2PS_USE_CURRENT_VIEWPORT;
}

int get_GL2PS_COMPRESS(void){
	return GL2PS_COMPRESS;
}

int get_GL2PS_NO_BLENDING(void){
	return GL2PS_NO_BLENDING;
}

int get_GL2PS_TIGHT_BOUNDING_BOX(void){
	return GL2PS_TIGHT_BOUNDING_BOX;
}

/* Arguments for gl2psEnable/gl2psDisable 
#define GL2PS_POLYGON_OFFSET_FILL 1
#define GL2PS_POLYGON_BOUNDARY    2
#define GL2PS_LINE_STIPPLE        3
#define GL2PS_BLEND               4*/

int get_GL2PS_POLYGON_OFFSET_FILL(void){
	return GL2PS_POLYGON_OFFSET_FILL;
}

int get_GL2PS_POLYGON_BOUNDARY(void){
	return GL2PS_POLYGON_BOUNDARY;
}

int get_GL2PS_LINE_STIPPLE(void){
	return GL2PS_LINE_STIPPLE;
}

int get_GL2PS_BLEND(void){
	return GL2PS_BLEND;
}


/* Text alignment (o=raster position; default mode is BL):
   +---+ +---+ +---+ +---+ +---+ +---+ +-o-+ o---+ +---o 
   | o | o   | |   o |   | |   | |   | |   | |   | |   | 
   +---+ +---+ +---+ +-o-+ o---+ +---o +---+ +---+ +---+ 
    C     CL    CR    B     BL    BR    T     TL    TR 

#define GL2PS_TEXT_C  1
#define GL2PS_TEXT_CL 2
#define GL2PS_TEXT_CR 3
#define GL2PS_TEXT_B  4
#define GL2PS_TEXT_BL 5
#define GL2PS_TEXT_BR 6
#define GL2PS_TEXT_T  7
#define GL2PS_TEXT_TL 8
#define GL2PS_TEXT_TR 9*/

int get_GL2PS_TEXT_C(void){
	return GL2PS_TEXT_C;
}

int get_GL2PS_TEXT_CL(void){
	return GL2PS_TEXT_CL;
}

int get_GL2PS_TEXT_CR(void){
	return GL2PS_TEXT_CR;
}

int get_GL2PS_TEXT_B(void){
	return GL2PS_TEXT_B;
}

int get_GL2PS_TEXT_BL(void){
	return GL2PS_TEXT_BL;
}

int get_GL2PS_TEXT_BR(void){
	return GL2PS_TEXT_BR;
}

int get_GL2PS_TEXT_T(void){
	return GL2PS_TEXT_T;
}

int get_GL2PS_TEXT_TL(void){
	return GL2PS_TEXT_TL;
}
int get_GL2PS_TEXT_TR(void){
	return GL2PS_TEXT_TR;
}

