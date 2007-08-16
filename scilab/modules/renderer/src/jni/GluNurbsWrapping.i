/* Swig file: GluNurbsWrapping.i */
/*  swig -java -package org.scilab.modules.renderer.gluNurbsWrapping -outdir ../java/org/scilab/modules/renderer/gluNurbsWrapping/  GluNurbsWrapping.i */
%module GluNurbsWrapping
%{
#include "gluNurbsWrapping/gluNurbsWrapping.hxx"
#include <GL/glu.h>
%}

/* JavaDoc for GluNurbsWrappingJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
  * Test array calling 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007
  */
public class%}

/* Constructor for GluNurbsWrappingJNI class */
%pragma(java) jniclasscode="
  /**
   * Constructor. Should not be called
   */
  protected GluNurbsWrappingJNI() {
	throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scirenderer");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scirenderer does not exist or cannot be found.");
		e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for GluNurbsWrapping class */
%pragma(java) moduleclassmodifiers="
 /** 
  * Test array calling
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007 
  */
public class";

/* Constructor for GluNurbsWrapping class */
%pragma(java) modulecode="
 /**
  * Constructor. Should not be called.
  */
 protected GluNurbsWrapping() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers gluNewNurbsRendererWrapping(void)  "
 /**
  * Wrapping of gluNewNurbsRenderer()
  * @return new nurbs renderer
  */
public";
GLUnurbsObj * gluNewNurbsRendererWrapping(void);

/* JavaDoc */
%javamethodmodifiers gluDeleteNurbsRendererWrapping(GLUnurbsObj * nurbsObj)  "
 /**
  * Wrapping of gluDeleteNurbsRenderer(GLUnurbsObj *)
  * @param nurbsObj nurbs renderer to destroy
  */
public";
void gluDeleteNurbsRendererWrapping(GLUnurbsObj * nurbsObj);

/* JavaDoc */
%javamethodmodifiers gluBeginCurveWrapping(GLUnurbsObj * nurbsObj)  "
 /**
  * Wrapping of gluBeginCurve(GLUnurbsObj *)
  * @param nurbsObj nurbs renderer to use
  */
public";
void gluBeginCurveWrapping(GLUnurbsObj * nurbsObj);

/* JavaDoc */
%javamethodmodifiers gluEndCurveWrapping(GLUnurbsObj * nurbsObj)  "
 /**
  * Wrapping of gluEndCurve(GLUnurbsObj *)
  * @param nurbsObj nurbs renderer to use
  */
public";
void gluEndCurveWrapping(GLUnurbsObj * nurbsObj);


/* JavaDoc */
%include "arrays_java.i";
%javamethodmodifiers gluNurbsCurveWrapping(GLUnurbsObj * nurbsObj, int nknots,
                                           float knot[], int stride, float ctlarray[],
                                           int order, int type) "
 /**
  * Wrapping of gluNurbsCurve( GLUnurbsObj *, GLint, GLfloat *, GLint, GLfloat *, GLint, GLenum)
  * @param nurbsObj nurbs renderer to use
  * @param nknots number of nots
  * @param knot array of knot
  * @param stride offset
  * @param cltarray array of control points
  * @param type attribute of the curve
  */
public";
void gluNurbsCurveWrapping(GLUnurbsObj * nurbsObj, int nknots,
                           float knot[], int stride, float ctlarray[],
                           int order, int type);

/* JavaDoc */
%javamethodmodifiers gluBeginSurfaceWrapping(GLUnurbsObj * nurbsObj)  "
 /**
  * Wrapping of gluBeginSurface(GLUnurbsObj *)
  * @param nurbsObj nurbs renderer to use
  */
public";
void gluBeginSurfaceWrapping(GLUnurbsObj * nurbsObj);

/* JavaDoc */
%javamethodmodifiers gluEndSurfaceWrapping(GLUnurbsObj * nurbsObj)  "
 /**
  * Wrapping of gluEndSurface(GLUnurbsObj *)
  * @param nurbsObj nurbs renderer to use
  */
public";
void gluEndSurfaceWrapping(GLUnurbsObj * nurbsObj);

/* JavaDoc */
%javamethodmodifiers gluNurbsSurfaceWrapping(GLUnurbsObj * nurbsObj, int sknot_count,
                                             float sknot[], int tknot_count,
                                             float tknot[], int s_stride, int t_stride,
                                             float ctlarray[], int sorder,
                                             int torder, int type) "
 /**
  * Wrapping of gluNubrsCurve( GLUnurbsObj *, GLint, GLfloat *, GLint, GLfloat *, GLint, GLint, GLfloat, GLint, GLint GLenum)
  * @param nurbsObj    nurbs renderer to use
  * @param sknot_count number of knot in first dimension
  * @param sknot       knot values for first dimension
  * @param tknot_count number of knot in second dimension
  * @param tknot       knot values for second dimension
  * @param s_stride    offest for first dimension
  * @param t_stride    offset for second dimension
  * @param ctlarray    array of control points
  * @param sorder      order for first dimension
  * @param torder      order for second dimension
  * @param type        attribute of the curve
  */
public";
void gluNurbsSurfaceWrapping(GLUnurbsObj * nurbsObj, int sknot_count,
                             float sknot[], int tknot_count,
                             float tknot[], int s_stride, int t_stride,
                             float ctlarray[], int sorder,
                             int torder, int type ) ;



/* JavaDoc */
%javamethodmodifiers gluBeginTrimWrapping(GLUnurbsObj * nurbsObj)  "
 /**
  * Wrapping of gluBeginTrim(GLUnurbsObj *)
  * @param nurbsObj nurbs renderer to use
  */
public";
void gluBeginTrimWrapping(GLUnurbsObj * nurbsObj);

/* JavaDoc */
%javamethodmodifiers gluEndTrimWrapping(GLUnurbsObj * nurbsObj)  "
 /**
  * Wrapping of gluEndTrim(GLUnurbsObj *)
  * @param nurbsObj nurbs renderer to use
  */
public";
void gluEndTrimWrapping(GLUnurbsObj * nurbsObj);

/* JavaDoc */
%javamethodmodifiers gluPwlCurveWrapping(GLUnurbsObj * nurbsObj, int count, float array[],
                                         int stride, int type )  "
 /**
  * Wrapping of gluPwlCurve(GLUnurbsObj *, GLint, GLfloat *, GLint, GLenum)
  * @param nurbsObj nurbs renderer to use
  * @param count number of points on the curve
  * @param array array containing curve points
  * @param stride offset between values and curve points
  * @param type type of triming region
  */
public";
void gluPwlCurveWrapping(GLUnurbsObj * nurbsObj, int count, float array[],
                         int stride, int type);

/* JavaDoc */
%javamethodmodifiers gluNurbsPropertyWrapping(GLUnurbsObj * nurbsObj,
                                              int property, float value)  "
 /**
  * Wrapping of gluNurbsProperty(GLUnurbsObj *, GLenum, GLfloat)
  * @param nurbsObj nurbs renderer to use
  * @param property type of property to modify
  * @param value new value to set
  */
public";
void gluNurbsPropertyWrapping(GLUnurbsObj * nurbsObj, int property, float value);

/* JavaDoc */
%javamethodmodifiers gluGetNurbsPropertyWrapping(GLUnurbsObj * nurbsObj, int property)  "
 /**
  * Wrapping of gluGetNurbsProperty(GLUnurbsObj *, GLenum, GLfloat *)
  * @param nurbsObj nurbs renderer to use
  * @param property type of property to get
  * @return queried value
  */
public";
float gluGetNurbsPropertyWrapping(GLUnurbsObj * nurbsObj, int property);

/* JavaDoc */
%javamethodmodifiers gluLoadSamplingMatricesWrapping(GLUnurbsObj * nurbsObj,
                                                     const float modelMatrix[16],
                                                     const float projMatrix[16],
                                                     const int viewPort[16] )  "
 /**
  * Wrapping of gluLoadSamplingMatrices(GLUnurbsObj *, const GLfloat[16], const GLfloat[16], const GLint[4])
  * @param nurbsObj nurbs renderer to use
  * @param property type of property to get
  * @return queried value
  */
public";
void gluLoadSamplingMatricesWrapping(GLUnurbsObj * nurbsObj,
                                     const float modelMatrix[16],
                                     const float projMatrix[16],
                                     const int viewPort[16] );



