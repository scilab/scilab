/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : C++ interface of JoGL function used by GL2PS 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _JOGL_INTERFACE_H_
#define _JOGL_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BOOL.h"

void joglFeedbackBuffer(int size, int type);

void joglReadFeedbackBuffer(int size, float * buffer);

BOOL joglIsEnabled(int cap);

void joglBegin(int mode);

void joglVertex3f(float x, float y, float z);

void joglEnd(void);

void joglGetCurrentRasterPositionValid(BOOL * value);

int joglRenderMode(int mode);

void joglGetLineStipplePattern(int * value);

void joglGetLineStippleRepeat(int * value);

void joglGetIndexClearValue(int * value);

void joglGetViewport(int * viewPort);

void joglGetBlendSrc(int * value);

void joglGetBlendDst(int * value);

void joglGetColorClearValue(float * color);

void joglGetPolygonOffsetFactor(float * value);

void joglGetPolygonOffsetUnits(float * value);

void joglGetCurrentRasterPosition(float * position);

void joglGetCurrentRasterColor(float * color);

void joglPassThrough(float token);


int joglGL_RGBA(void);
int joglGL_RGB(void);
int joglGL_CURRENT_RASTER_POSITION_VALID(void);
int joglGL_CURRENT_RASTER_POSITION(void);
int joglGL_CURRENT_RASTER_COLOR(void);
int joglGL_SRC_ALPHA(void);
int joglGL_ONE_MINUS_SRC_ALPHA(void);
int joglGL_ONE(void);
int joglGL_ZERO(void);
int joglGL_COLOR_INDEX(void);
int joglGL_POINT_TOKEN(void);
int joglGL_LINE_TOKEN(void);
int joglGL_LINE_RESET_TOKEN(void);
int joglGL_POLYGON_TOKEN(void);
int joglGL_BITMAP_TOKEN(void);
int joglGL_DRAW_PIXEL_TOKEN(void);
int joglGL_COPY_PIXEL_TOKEN(void);
int joglGL_PASS_THROUGH_TOKEN(void);
int joglGL_FEEDBACK(void);
int joglGL_COLOR_CLEAR_VALUE(void);
int joglGL_INDEX_CLEAR_VALUE(void);
int joglGL_RENDER(void);
int joglGL_VIEWPORT(void);
int joglGL_BLEND(void);
int joglGL_BLEND_SRC(void);
int joglGL_BLEND_DST(void);
int joglGL_3D_COLOR(void);
int joglGL_FLOAT(void);
int joglGL_UNSIGNED_BYTE(void);
int joglGL_POINTS(void);
int joglGL_POLYGON_OFFSET_FACTOR(void);
int joglGL_POLYGON_OFFSET_UNITS(void);
int joglGL_LINE_STIPPLE_PATTERN(void);
int joglGL_LINE_STIPPLE_REPEAT(void);

#ifdef __cplusplus
}
#endif


#endif /* _JOGL_INTERFACE_H_ */
