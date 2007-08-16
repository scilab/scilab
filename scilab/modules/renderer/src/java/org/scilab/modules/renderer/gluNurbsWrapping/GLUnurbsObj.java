/*------------------------------------------------------------------------*/
/* file: SciRenderer.java                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Wrapping for the GLUnurbsObj pointer and the nurbs realted GLU  */
/*        functions                                                       */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.gluNurbsWrapping;

import org.scilab.modules.renderer.jni.GluNurbsWrapping;
import org.scilab.modules.renderer.jni.SWIGTYPE_p_GLUnurbsObj;

/**
 * Wrapping for the GLUnurbsObj pointer and the nurbs realted GLU functions
 * @copyright INRIA 2007
 * @author Jean-Baptiste Silvy
 */
public class GLUnurbsObj {
  
  /** Pointer on swig object */
  private SWIGTYPE_p_GLUnurbsObj swigPointer;

  /**
   * Default constructor.
   */
  protected GLUnurbsObj() {
    swigPointer = null;
  }

  /**
   * Wrapping of gluNewNurbsRenderer()
   * @return new nurbs renderer
   */
  public static GLUnurbsObj gluNewNurbsRenderer() {
    GLUnurbsObj res = new GLUnurbsObj();
    res.setSwigPointer(GluNurbsWrapping.gluNewNurbsRendererWrapping());
    return res;
  }

  /**
   * Wrapping of gluDeleteNurbsRenderer(GLUnurbsObj *)
   * @param nurbsObj nurbs renderer to destroy
   */
  public static void gluDeleteNurbsRenderer(GLUnurbsObj nurbsObj) {
    GluNurbsWrapping.gluDeleteNurbsRendererWrapping(nurbsObj.getSwigPointer());
    nurbsObj.setSwigPointer(null);
  }

  /**
   * Wrapping of gluBeginCurve(GLUnurbsObj *)
   */
  public void gluBeginCurve() {
    GluNurbsWrapping.gluBeginCurveWrapping(getSwigPointer());
  }

  /**
   * Wrapping of gluEndCurve(GLUnurbsObj *)
   */
  public void gluEndCurve() {
    GluNurbsWrapping.gluEndCurveWrapping(getSwigPointer());
  }

  /**
   * Wrapping of gluNurbsCurve( GLUnurbsObj *, GLint, GLfloat *, GLint, GLfloat *, GLint, GLenum)
   * @param nknots number of nots
   * @param knot array of knot
   * @param stride offset
   * @param ctlarray array of control points
   * @param order order of the Nurbs curve
   * @param type attribute of the curve
   */
  public void gluNurbsCurve(int nknots, float[] knot,
                            int stride, float[] ctlarray, int order, int type) {
    GluNurbsWrapping.gluNurbsCurveWrapping(getSwigPointer(), nknots, knot,
                                           stride, ctlarray, order, type);
  }
  
  /**
   * Wrapping of gluBeginSurface(GLUnurbsObj *)
   */
  public void gluBeginSurface() {
    GluNurbsWrapping.gluBeginSurfaceWrapping(getSwigPointer());
  }

  /**
   * Wrapping of gluEndSurface(GLUnurbsObj *)
   */
  public void gluEndSurface() {
    GluNurbsWrapping.gluEndSurfaceWrapping(getSwigPointer());
  }

  /**
   * Wrapping of gluNubrsCurve( GLUnurbsObj *, GLint, GLfloat *, GLint, GLfloat *, GLint, GLint, GLfloat, GLint, GLint GLenum)
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
  public void gluNurbsSurface(int sknot_count, float[] sknot, int tknot_count,
                              float[] tknot, int s_stride, int t_stride,
                              float[] ctlarray, int sorder, int torder, int type) {
    GluNurbsWrapping.gluNurbsSurfaceWrapping(getSwigPointer(), sknot_count, sknot,
                                             tknot_count, tknot, s_stride, t_stride,
                                             ctlarray, sorder, torder, type);
  }

   /**
    * Wrapping of gluBeginTrim(GLUnurbsObj *)
    */
  public void gluBeginTrimWrapping() {
    GluNurbsWrapping.gluBeginTrimWrapping(getSwigPointer());
  }

  
  /**
   * Wrapping of gluEndTrim(GLUnurbsObj *)
   */
  public void gluEndTrimWrapping() {
    GluNurbsWrapping.gluEndTrimWrapping(getSwigPointer());
  }


  /**
   * Wrapping of gluPwlCurve(GLUnurbsObj *, GLint, GLfloat *, GLint, GLenum)
   * @param count number of points on the curve
   * @param array array containing curve points
   * @param stride offset between values and curve points
   * @param type type of triming region
   */
  public void gluPwlCurve(int count, float[] array, int stride, int type) {
    GluNurbsWrapping.gluPwlCurveWrapping(getSwigPointer(), count, array, stride, type);
  }

  
  /**
   * Wrapping of gluNurbsProperty(GLUnurbsObj *, GLenum, GLfloat)
   * @param property type of property to modify
   * @param value new value to set
   */
  public void gluNurbsProperty(int property, float value) {
    GluNurbsWrapping.gluNurbsPropertyWrapping(getSwigPointer(), property, value);
  }

  
  /**
   * Wrapping of gluGetNurbsProperty(GLUnurbsObj *, GLenum, GLfloat *)
   * @param property type of property to get
   * @return queried value
   */
  public float gluGetNurbsProperty(int property) {
    return GluNurbsWrapping.gluGetNurbsPropertyWrapping(getSwigPointer(), property);
  }

  
  /**
   * Wrapping of gluLoadSamplingMatrices(GLUnurbsObj *, const GLfloat[16], const GLfloat[16], const GLint[4])
   * @param modelMatrix model view Matrix (4x4)
   * @param projMatrix projection Matrix (4x4).
   * @param viewPort current viewport (x,y,w,h)
   */
  public void gluLoadSamplingMatrices(float[] modelMatrix, float[] projMatrix, int[] viewPort) {
    GluNurbsWrapping.gluLoadSamplingMatricesWrapping(getSwigPointer(), modelMatrix, projMatrix, viewPort);
  }

  
  

  /**
   * Get the swig pointer on the glu object. Use with caution.
   * @return swig pointer on the glu renderer.
   */
  protected SWIGTYPE_p_GLUnurbsObj getSwigPointer() {
    return swigPointer;
  }

  /**
   * Set the pointer to the swig object.
   * @param swigPointer the pointer to use.
   */
  protected void setSwigPointer(SWIGTYPE_p_GLUnurbsObj swigPointer) {
    this.swigPointer = swigPointer;
  }
  
}
