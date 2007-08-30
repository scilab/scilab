/*------------------------------------------------------------------------*/
/* file: gluNurbsWrapping.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Wrapping of GLU nurbs related functions.                        */
/*        Perform type conversion                                         */
/*------------------------------------------------------------------------*/
#include <stdio.h>

#include "gluNurbsWrapping.hxx"

/*------------------------------------------------------------------------------------------*/
/**
 * Callback for when errors happens with nurbs.
 */
static void /*CALLBACK*/ nurbsErrorDefault(GLenum errno)
{
  const GLubyte * errString;
  errString = gluErrorString(errno);
  printf("%s\n", errString);
}
/*------------------------------------------------------------------------------------------*/
GLUnurbsObj * gluNewNurbsRendererWrapping(void)
{
  GLUnurbsObj * newRenderer = gluNewNurbsRenderer();
  // set default error call back
  // gluNurbsCallback(newRenderer, GLU_ERROR, (void (CALLBACK*)()) nurbsErrorDefault);
  return newRenderer;
}
/*------------------------------------------------------------------------------------------*/
void gluDeleteNurbsRendererWrapping(GLUnurbsObj * nurbsObj)
{
  gluDeleteNurbsRenderer(nurbsObj);
}
/*------------------------------------------------------------------------------------------*/
void gluBeginCurveWrapping(GLUnurbsObj * nurbsObj)
{
  gluEndCurve(nurbsObj);
}
/*------------------------------------------------------------------------------------------*/
void gluEndCurveWrapping(GLUnurbsObj * nurbsObj)
{
  gluEndCurve(nurbsObj);
}
/*------------------------------------------------------------------------------------------*/
void gluNurbsCurveWrapping(GLUnurbsObj * nurbsObj, int nknots,
                           float knot[], int stride, float ctlarray[],
                           int order, int type)
{
  gluNurbsCurve(nurbsObj, nknots, knot, stride, ctlarray, order, type);
}
/*------------------------------------------------------------------------------------------*/
void gluBeginSurfaceWrapping(GLUnurbsObj * nurbsObj)
{
  gluBeginSurface(nurbsObj);
}
/*------------------------------------------------------------------------------------------*/
void gluEndSurfaceWrapping(GLUnurbsObj * nurbsObj)
{
  gluEndSurface(nurbsObj);
}
/*------------------------------------------------------------------------------------------*/
void gluNurbsSurfaceWrapping(GLUnurbsObj * nurbsObj, int sknot_count,
                             float sknot[], int tknot_count,
                             float tknot[], int s_stride, int t_stride,
                             float ctlarray[], int sorder,
                             int torder, int type )
{
  gluNurbsSurface(nurbsObj, sknot_count, sknot, tknot_count, tknot, s_stride, t_stride,
                  ctlarray, sorder, torder, type);
}
/*------------------------------------------------------------------------------------------*/
void gluBeginTrimWrapping(GLUnurbsObj * nurbsObj)
{
  gluBeginTrim(nurbsObj);
}
/*------------------------------------------------------------------------------------------*/
void gluEndTrimWrapping(GLUnurbsObj * nurbsObj)
{
  gluEndTrim(nurbsObj);
}
/*------------------------------------------------------------------------------------------*/
void gluPwlCurveWrapping(GLUnurbsObj * nurbsObj, int count, float array[],
                         int stride, int type)
{
  gluPwlCurve(nurbsObj, count, array, stride, type);
}
/*------------------------------------------------------------------------------------------*/
void gluNurbsPropertyWrapping(GLUnurbsObj * nurbsObj, int property, float value)
{
  gluNurbsProperty(nurbsObj, property, value);
}
/*------------------------------------------------------------------------------------------*/
float gluGetNurbsPropertyWrapping(GLUnurbsObj * nurbsObj, int property)
{
  float value = 0.0f;
  gluGetNurbsProperty(nurbsObj, property, &value);

  return value;
}
/*------------------------------------------------------------------------------------------*/
void gluLoadSamplingMatricesWrapping(GLUnurbsObj * nurbsObj,
                                     const float modelMatrix[16],
                                     const float projMatrix[16],
                                     const int viewPort[16] )
{
  gluLoadSamplingMatrices(nurbsObj, modelMatrix, projMatrix, viewPort);
}
/*------------------------------------------------------------------------------------------*/
