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

#include "JoGLInterface.h"
#include "GL2PSToJOGL.hxx"
#include "JOGLConstant.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

#define VIEWPORT_SIZE 4
#define COLOR_CLEAR_VALUE_SIZE 4
#define CURRENT_RASTER_POSITION_SIZE 4
#define CURRENT_RASTER_COLOR_SIZE 4

using namespace org_scilab_modules_graphic_export;

/*---------------------------------------------------------------------------------*/
void joglFeedbackBuffer(int size, int type)
{
  GL2PSToJOGL::glFeedbackBuffer(getScilabJavaVM(), size, type);
}
/*---------------------------------------------------------------------------------*/
void joglReadFeedbackBuffer(int size, float * buffer)
{
	float * javaBuffer = GL2PSToJOGL::readFeedbackBuffer(getScilabJavaVM(), size);
	
	 // copy the computed buffer into buffer
  for (int i = 0; i < size; i++)
  {
    buffer[i] = javaBuffer[i];
  }
  delete[] javaBuffer;

}
/*---------------------------------------------------------------------------------*/
BOOL joglIsEnabled(int cap)
{
  return (GL2PSToJOGL::glIsEnabled(getScilabJavaVM(), cap) == true);
}
/*---------------------------------------------------------------------------------*/
void joglBegin(int mode)
{
  GL2PSToJOGL::glBegin(getScilabJavaVM(), mode);
}
/*---------------------------------------------------------------------------------*/
void joglVertex3f(float x, float y, float z)
{
  GL2PSToJOGL::glVertex3f(getScilabJavaVM(), x, y, z);
}
/*---------------------------------------------------------------------------------*/
void joglEnd(void)
{
  GL2PSToJOGL::glEnd(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetCurrentRasterPositionValid(BOOL * value)
{
  *value =(GL2PSToJOGL::glGetCurrentRasterPositionValid(getScilabJavaVM()) == true);
}
/*---------------------------------------------------------------------------------*/
int joglRenderMode(int mode)
{
  return GL2PSToJOGL::glRenderMode(getScilabJavaVM(), mode);
}
/*---------------------------------------------------------------------------------*/
void joglGetLineStipplePattern(int * value)
{
  *value = GL2PSToJOGL::glGetLineStipplePattern(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetLineStippleRepeat(int * value)
{
  *value = GL2PSToJOGL::glGetLineStippleRepeat(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetIndexClearValue(int * value)
{
  *value = GL2PSToJOGL::glGetIndexClearValue(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetViewport(int * viewPort)
{
  int * javaViewPort = GL2PSToJOGL::glGetViewport(getScilabJavaVM());

  for (int i = 0; i < VIEWPORT_SIZE; i++)
  {
    viewPort[i] = javaViewPort[i];
  }
  delete[] javaViewPort;
}
/*---------------------------------------------------------------------------------*/
void joglGetBlendSrc(int * value)
{
  *value = GL2PSToJOGL::glGetBlendSrc(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetBlendDst(int * value)
{
  *value = GL2PSToJOGL::glGetBlendDst(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetColorClearValue(float * color)
{
  float * javaColor = GL2PSToJOGL::glGetColorClearValue(getScilabJavaVM());

  for (int i = 0; i < COLOR_CLEAR_VALUE_SIZE; i++)
  {
    color[i] = javaColor[i];
  }
  delete[] javaColor;
}
/*---------------------------------------------------------------------------------*/
void joglGetPolygonOffsetFactor(float * value)
{
  *value = GL2PSToJOGL::glGetPolygonOffsetFactor(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetPolygonOffsetUnits(float * value)
{
  *value = GL2PSToJOGL::glGetPolygonOffsetUnits(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
void joglGetCurrentRasterPosition(float * position)
{
  float * javaPos = GL2PSToJOGL::glGetCurrentRasterPosition(getScilabJavaVM());

  for (int i = 0; i < CURRENT_RASTER_POSITION_SIZE; i++)
  {
    position[i] = javaPos[i];
  }
  delete[] javaPos;
}
/*---------------------------------------------------------------------------------*/
void joglGetCurrentRasterColor(float * color)
{
  float * javaColor = GL2PSToJOGL::glGetCurrentRasterColor(getScilabJavaVM());

  for (int i = 0; i < CURRENT_RASTER_COLOR_SIZE; i++)
  {
    color[i] = javaColor[i];
  }
  delete[] javaColor;
}
/*---------------------------------------------------------------------------------*/
void joglPassThrough(float token)
{
  GL2PSToJOGL::glPassThrough(getScilabJavaVM(), token);
}
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
int joglGL_RGBA(void)
{
  return JOGLConstant::get_GL_RGBA(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_RGB(void)
{
  return JOGLConstant::get_GL_RGB(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_SRC_ALPHA(void)
{
  return JOGLConstant::get_GL_SRC_ALPHA(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_ONE_MINUS_SRC_ALPHA(void)
{
  return JOGLConstant::get_GL_ONE_MINUS_SRC_ALPHA(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_ONE(void)
{
  return JOGLConstant::get_GL_ONE(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_ZERO(void)
{
  return JOGLConstant::get_GL_ZERO(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_COLOR_INDEX(void)
{
  return JOGLConstant::get_GL_COLOR_INDEX(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_POINT_TOKEN(void)
{
  return JOGLConstant::get_GL_POINT_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_LINE_TOKEN(void)
{
  return JOGLConstant::get_GL_LINE_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_LINE_RESET_TOKEN(void)
{
  return JOGLConstant::get_GL_LINE_RESET_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_POLYGON_TOKEN(void)
{
  return JOGLConstant::get_GL_POLYGON_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_BITMAP_TOKEN(void)
{
  return JOGLConstant::get_GL_BITMAP_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_DRAW_PIXEL_TOKEN(void)
{
  return JOGLConstant::get_GL_DRAW_PIXEL_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_COPY_PIXEL_TOKEN(void)
{
  return JOGLConstant::get_GL_COPY_PIXEL_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_PASS_THROUGH_TOKEN(void)
{
  return JOGLConstant::get_GL_PASS_THROUGH_TOKEN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_FEEDBACK(void)
{
  return JOGLConstant::get_GL_FEEDBACK(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_COLOR_CLEAR_VALUE(void)
{
  return JOGLConstant::get_GL_COLOR_CLEAR_VALUE(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_INDEX_CLEAR_VALUE(void)
{
  return JOGLConstant::get_GL_INDEX_CLEAR_VALUE(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_RENDER(void)
{
  return JOGLConstant::get_GL_RENDER(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_VIEWPORT(void)
{
  return JOGLConstant::get_GL_VIEWPORT(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_BLEND(void)
{
  return JOGLConstant::get_GL_BLEND(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_BLEND_SRC(void)
{
  return JOGLConstant::get_GL_BLEND_SRC(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_BLEND_DST(void)
{
  return JOGLConstant::get_GL_BLEND_DST(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_3D_COLOR(void)
{
  return JOGLConstant::get_GL_3D_COLOR(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_FLOAT(void)
{
  return JOGLConstant::get_GL_FLOAT(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
/* Calixte */
int joglGL_UNSIGNED_BYTE(void)
{
  return JOGLConstant::get_GL_UNSIGNED_BYTE(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_POINTS(void)
{
  return JOGLConstant::get_GL_POINTS(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_POLYGON_OFFSET_FACTOR(void)
{
  return JOGLConstant::get_GL_POLYGON_OFFSET_FACTOR(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_POLYGON_OFFSET_UNITS(void)
{
  return JOGLConstant::get_GL_POLYGON_OFFSET_UNITS(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_LINE_STIPPLE_PATTERN(void)
{
  return JOGLConstant::get_GL_LINE_STIPPLE_PATTERN(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
int joglGL_LINE_STIPPLE_REPEAT(void)
{
  return JOGLConstant::get_GL_LINE_STIPPLE_REPEAT(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
