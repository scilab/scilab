/*------------------------------------------------------------------------*/
/* file: gluNurbsWrapping.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Wrapping of GLU nurbs related functions.                        */
/*        Perform type conversion                                         */
/*------------------------------------------------------------------------*/


#ifndef _GLU_NURBS_WRAPPING_HXX_
#define _GLU_NURBS_WRAPPING_HXX_

#include <GL/glu.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Wrapping for gluNewNurbsRenderer(void)
 */
GLUnurbsObj * gluNewNurbsRendererWrapping(void);

/**
 * Wrapping for gluDeleteNurbsRendererWrapping(GLUnurbsObj *)
 */
void gluDeleteNurbsRendererWrapping(GLUnurbsObj * nurbsObj);

/**
 * Wrapping for gluBeginCurve(GLUnurbsObj *)
 */
void gluBeginCurveWrapping(GLUnurbsObj * nurbsObj);

/**
 * Wrapping for gluEndCurve(GLUnurbsObj *)
 */
void gluEndCurveWrapping(GLUnurbsObj * nurbsObj);


/**
 * Wrapping for gluNurbsCurve(GLUnurbsObj *, GLint, GLfloat *, GLint, GLfloat *, GLint, GLenum)
 */
void gluNurbsCurveWrapping(GLUnurbsObj * nurbsObj, int nknots,
                           float knot[], int stride, float ctlArray[],
                           int order, int type);

/**
 * Wrapping for gluBeginSurface(GLUnurbsObj *)
 */
void gluBeginSurfaceWrapping(GLUnurbsObj * nurbsObj);

/**
 * Wrapping for gluEndSurface(GLUnurbsObj *)
 */
void gluEndSurfaceWrapping(GLUnurbsObj * nurbsObj);

/**
 * Wrapping for gluNurbsSurface(GLUnurbsObj *, GLint, GLfloat *, GLint, GLfloat *,
 *                              GLint, GLint, GLfoat *n GLint, GLint, GLenum);
 */
void gluNurbsSurfaceWrapping(GLUnurbsObj * nurbsObj, int sknot_count,
                             float sknot[], int tknot_count,
                             float tknot[], int s_stride, int t_stride,
                             float ctlArray[], int sorder,
                             int torder, int type );

/**
 * Wrapping for gluBeginTrim(GLUnurbsObj *)
 */
void gluBeginTrimWrapping(GLUnurbsObj * nurbsObj);

/**
 * Wrapping for gluEndTrim(GLUnurbsObj *)
 */
void gluEndTrimWrapping(GLUnurbsObj * nurbsObj);

/**
 * Wrapping for gluPwlCurve(GLUnurbsObj *, GLint, GLfloat *, GLint, GLenum)
 */
void gluPwlCurveWrapping(GLUnurbsObj * nurbsObj, int count, float array[],
                         int stride, int type);

/**
 * Wrapping for gluNurbsProperty(GLUnurbsObj *, GLenum, GLfloat)
 */
void gluNurbsPropertyWrapping(GLUnurbsObj * nurbsObj, int property, float value);

/**
 * Wrapping for gluGetNurbsProperty(GLUnurbsObj *, GLenum, GLfloat *)
 * For compatibility wuth Java, the computed third parameter is returned
 * instead of being pushed by adress.
 */
float gluGetNurbsPropertyWrapping(GLUnurbsObj * nurbsObj, int property);

/**
 * Wrapping for gluLoadSamplingMatrices(GLUnurbsObj *, const GLfloat[16],
 *                                      const GLfloat[16], const GLint[4]);
 */
void gluLoadSamplingMatricesWrapping(GLUnurbsObj * nurbsObj,
                                     const float modelMatrix[16],
                                     const float projMatrix[16],
                                     const int viewPort[16] );


#ifdef __cplusplus
}
#endif

#endif /* _GLU_NURBS_WRAPPING_HXX_ */
