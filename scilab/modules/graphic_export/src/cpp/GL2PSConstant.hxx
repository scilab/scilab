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
 
#ifndef _GL2PS_CONSTANT_H_
#define _GL2PS_CONSTANT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "GL2PSConstant.hxx"


/* Version number 
#define GL2PS_MAJOR_VERSION 1
#define GL2PS_MINOR_VERSION 3
#define GL2PS_PATCH_VERSION 2
#define GL2PS_EXTRA_VERSION ""*/

int get_GL2PS_MAJOR_VERSION(void);

int get_GL2PS_MINOR_VERSION(void);

int get_GL2PS_PATCH_VERSION(void);

char *get_GL2PS_EXTRA_VERSION(void);


/* Output file formats (the values and the ordering are important!) 
#define GL2PS_PS  0
#define GL2PS_EPS 1
#define GL2PS_TEX 2
#define GL2PS_PDF 3
#define GL2PS_SVG 4
#define GL2PS_PGF 5*/

int get_GL2PS_PS(void);

int get_GL2PS_EPS(void);

int get_GL2PS_TEX(void);

int get_GL2PS_PDF(void);

int get_GL2PS_SVG(void);

int get_GL2PS_PGF(void);


/* Sorting algorithms 
#define GL2PS_NO_SORT     1
#define GL2PS_SIMPLE_SORT 2
#define GL2PS_BSP_SORT    3*/

int get_GL2PS_NO_SORT(void);

int get_GL2PS_SIMPLE_SORT(void);

int get_GL2PS_BSP_SORT(void);


/* Message levels and error codes 
#define GL2PS_SUCCESS       0
#define GL2PS_INFO          1
#define GL2PS_WARNING       2
#define GL2PS_ERROR         3
#define GL2PS_NO_FEEDBACK   4
#define GL2PS_OVERFLOW      5
#define GL2PS_UNINITIALIZED 6*/

int get_GL2PS_SUCCESS(void);

int get_GL2PS_INFO(void);

int get_GL2PS_WARNING(void);

int get_GL2PS_ERROR(void);

int get_GL2PS_NO_FEEDBACK(void);

int get_GL2PS_OVERFLOW(void);

int get_GL2PS_UNINITIALIZED(void);


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

int get_GL2PS_NONE(void);

int get_GL2PS_DRAW_BACKGROUND(void);

int get_GL2PS_SIMPLE_LINE_OFFSET(void);

int get_GL2PS_SILENT(void);

int get_GL2PS_BEST_ROOT(void);

int get_GL2PS_OCCLUSION_CULL(void);

int get_GL2PS_NO_TEXT(void);

int get_GL2PS_LANDSCAPE(void);

int get_GL2PS_NO_PS3_SHADING(void);

int get_GL2PS_NO_PIXMAP(void);

int get_GL2PS_USE_CURRENT_VIEWPORT(void);

int get_GL2PS_COMPRESS(void);

int get_GL2PS_NO_BLENDING(void);

int get_GL2PS_TIGHT_BOUNDING_BOX(void);


/* Arguments for gl2psEnable/gl2psDisable 
#define GL2PS_POLYGON_OFFSET_FILL 1
#define GL2PS_POLYGON_BOUNDARY    2
#define GL2PS_LINE_STIPPLE        3
#define GL2PS_BLEND               4*/

int get_GL2PS_POLYGON_OFFSET_FILL(void);

int get_GL2PS_POLYGON_BOUNDARY(void);

int get_GL2PS_LINE_STIPPLE(void);

int get_GL2PS_BLEND(void);


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

int get_GL2PS_TEXT_C(void);

int get_GL2PS_TEXT_CL(void);

int get_GL2PS_TEXT_CR(void);

int get_GL2PS_TEXT_B(void);

int get_GL2PS_TEXT_BL(void);

int get_GL2PS_TEXT_BR(void);

int get_GL2PS_TEXT_T(void);

int get_GL2PS_TEXT_TL(void);

int get_GL2PS_TEXT_TR(void);



#ifdef __cplusplus
}
#endif

#endif /* _GL2PS_CONSTANT_H_ */
