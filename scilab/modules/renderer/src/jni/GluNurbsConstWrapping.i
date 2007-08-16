/* Swig file: GluNurbsConstWrapping.i */
/*  swig -java -package org.scilab.modules.renderer.gluNurbsWrapping -outdir ../java/org/scilab/modules/renderer/gluNurbsWrapping/  GluNurbsConstWrapping.i */
%module GluNurbsConstWrapping
%{
#include "gluNurbsWrapping/gluNurbsConstWrapping.hxx"
%}

/* JavaDoc for GluNurbsConstWrappingJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
  * Test array calling 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007
  */
public class%}

/* Constructor for GluNurbsConstWrappingJNI class */
%pragma(java) jniclasscode="
  /**
   * Constructor. Should not be called
   */
  protected GluNurbsConstWrappingJNI() {
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

/* JavaDoc for GluNurbsConstWrapping class */
%pragma(java) moduleclassmodifiers="
 /** 
  * Test array calling
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007 
  */
public class";

/* Constructor for GluNurbsConstWrapping class */
%pragma(java) modulecode="
 /**
  * Constructor. Should not be called.
  */
 protected GluNurbsConstWrapping() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers getGluOutlinePolygon(void)  "
 /**
  * @return GLU_OUTLINE_POLYGON
  */
public";
int getGluOutlinePolygon(void);

/* JavaDoc */
%javamethodmodifiers getGluOutlinePatch(void)  "
 /**
  * @return GLU_OUTLINE_PATCH
  */
public";
int getGluOutlinePatch(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError(void)  "
 /**
  * @return GLU_NURBS_ERROR
  */
public";
int getGluNurbsError(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsBegin(void)  "
 /**
  * @return GLU_NURBS_BEGIN
  */
public";
int getGluNurbsBegin(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsBeginExt(void)  "
 /**
  * @return GLU_NURBS_BEGIN_EXT
  */
public";
int getGluNurbsBeginExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsVertex(void)  "
 /**
  * @return GLU_NURBS_VERTEX
  */
public";
int getGluNurbsVertex(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsVertexExt(void)  "
 /**
  * @return GLU_NURBS_VERTEX_EXT
  */
public";
int getGluNurbsVertexExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsNormal(void)  "
 /**
  * @return GLU_NURBS_NORMAL
  */
public";
int getGluNurbsNormal(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsNormalExt(void)  "
 /**
  * @return GLU_NURBS_NORMAL_EXT
  */
public";
int getGluNurbsNormalExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsColor(void)  "
 /**
  * @return GLU_NURBS_COLOR
  */
public";
int getGluNurbsColor(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsColorExt(void)  "
 /**
  * @return GLU_NURBS_COLOR_EXT
  */
public";
int getGluNurbsColorExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsTextureCoord(void)  "
 /**
  * @return GLU_NURBS_TEXTURE_COORD
  */
public";
int getGluNurbsTextureCoord(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsTexCoordExt(void)  "
 /**
  * @return GLU_NURBS_TEX_COORD_EXT
  */
public";
int getGluNurbsTexCoordExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsEnd(void)  "
 /**
  * @return GLU_NURBS_END
  */
public";
int getGluNurbsEnd(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsEndExt(void)  "
 /**
  * @return GLU_NURBS_END_EXT
  */
public";
int getGluNurbsEndExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsBeginData(void)  "
 /**
  * @return GLU_NURBS_BEGIN_DATA
  */
public";
int getGluNurbsBeginData(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsBeginDataExt(void)  "
 /**
  * @return GLU_NURBS_BEGIN_DATA_EXT
  */
public";
int getGluNurbsBeginDataExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsVertexData(void)  "
 /**
  * @return GLU_NURBS_VERTEX_DATA
  */
public";
int getGluNurbsVertexData(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsVertexDataExt(void)  "
 /**
  * @return GLU_NURBS_VERTEX_DATA_EXT
  */
public";
int getGluNurbsVertexDataExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsNormalData(void)  "
 /**
  * @return GLU_NURBS_NORMAL_DATA
  */
public";
int getGluNurbsNormalData(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsNormalDataExt(void)  "
 /**
  * @return GLU_NURBS_NORMAL_DATA_EXT
  */
public";
int getGluNurbsNormalDataExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsColorData(void)  "
 /**
  * @return GLU_NURBS_COLOR_DATA
  */
public";
int getGluNurbsColorData(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsColorDataExt(void)  "
 /**
  * @return GLU_NURBS_COLOR_DATA_EXT
  */
public";
int getGluNurbsColorDataExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsTextureCoordData(void)  "
 /**
  * @return GLU_NURBS_TEXTURE_COORD_DATA
  */
public";
int getGluNurbsTextureCoordData(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsTexCoordDataExt(void)  "
 /**
  * @return GLU_NURBS_TEX_COORD_DATA_EXT
  */
public";
int getGluNurbsTexCoordDataExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsEndData(void)  "
 /**
  * @return GLU_NURBS_END_DATA
  */
public";
int getGluNurbsEndData(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsEndDataExt(void)  "
 /**
  * @return GLU_NURBS_END_DATA_EXT
  */
public";
int getGluNurbsEndDataExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError1(void)  "
 /**
  * @return GLU_NURBS_ERROR1
  */
public";
int getGluNurbsError1(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError2(void)  "
 /**
  * @return GLU_NURBS_ERROR2
  */
public";
int getGluNurbsError2(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError3(void)  "
 /**
  * @return GLU_NURBS_ERROR3
  */
public";
int getGluNurbsError3(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError4(void)  "
 /**
  * @return GLU_NURBS_ERROR4
  */
public";
int getGluNurbsError4(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError5(void)  "
 /**
  * @return GLU_NURBS_ERROR5
  */
public";
int getGluNurbsError5(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError6(void)  "
 /**
  * @return GLU_NURBS_ERROR6
  */
public";
int getGluNurbsError6(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError7(void)  "
 /**
  * @return GLU_NURBS_ERROR7
  */
public";
int getGluNurbsError7(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError8(void)  "
 /**
  * @return GLU_NURBS_ERROR8
  */
public";
int getGluNurbsError8(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError9(void)  "
 /**
  * @return GLU_NURBS_ERROR9
  */
public";
int getGluNurbsError9(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError10(void)  "
 /**
  * @return GLU_NURBS_ERROR10
  */
public";
int getGluNurbsError10(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError11(void)  "
 /**
  * @return GLU_NURBS_ERROR11
  */
public";
int getGluNurbsError11(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError12(void)  "
 /**
  * @return GLU_NURBS_ERROR12
  */
public";
int getGluNurbsError12(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError13(void)  "
 /**
  * @return GLU_NURBS_ERROR13
  */
public";
int getGluNurbsError13(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError14(void)  "
 /**
  * @return GLU_NURBS_ERROR14
  */
public";
int getGluNurbsError14(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError15(void)  "
 /**
  * @return GLU_NURBS_ERROR15
  */
public";
int getGluNurbsError15(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError16(void)  "
 /**
  * @return GLU_NURBS_ERROR16
  */
public";
int getGluNurbsError16(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError17(void)  "
 /**
  * @return GLU_NURBS_ERROR17
  */
public";
int getGluNurbsError17(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError18(void)  "
 /**
  * @return GLU_NURBS_ERROR18
  */
public";
int getGluNurbsError18(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError19(void)  "
 /**
  * @return GLU_NURBS_ERROR19
  */
public";
int getGluNurbsError19(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError20(void)  "
 /**
  * @return GLU_NURBS_ERROR20
  */
public";
int getGluNurbsError20(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError21(void)  "
 /**
  * @return GLU_NURBS_ERROR21
  */
public";
int getGluNurbsError21(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError22(void)  "
 /**
  * @return GLU_NURBS_ERROR22
  */
public";
int getGluNurbsError22(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError23(void)  "
 /**
  * @return GLU_NURBS_ERROR23
  */
public";
int getGluNurbsError23(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError24(void)  "
 /**
  * @return GLU_NURBS_ERROR24
  */
public";
int getGluNurbsError24(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError25(void)  "
 /**
  * @return GLU_NURBS_ERROR25
  */
public";
int getGluNurbsError25(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError26(void)  "
 /**
  * @return GLU_NURBS_ERROR26
  */
public";
int getGluNurbsError26(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError27(void)  "
 /**
  * @return GLU_NURBS_ERROR27
  */
public";
int getGluNurbsError27(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError28(void)  "
 /**
  * @return GLU_NURBS_ERROR28
  */
public";
int getGluNurbsError28(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError29(void)  "
 /**
  * @return GLU_NURBS_ERROR29
  */
public";
int getGluNurbsError29(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError30(void)  "
 /**
  * @return GLU_NURBS_ERROR30
  */
public";
int getGluNurbsError30(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError31(void)  "
 /**
  * @return GLU_NURBS_ERROR31
  */
public";
int getGluNurbsError31(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError32(void)  "
 /**
  * @return GLU_NURBS_ERROR32
  */
public";
int getGluNurbsError32(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError33(void)  "
 /**
  * @return GLU_NURBS_ERROR33
  */
public";
int getGluNurbsError33(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError34(void)  "
 /**
  * @return GLU_NURBS_ERROR34
  */
public";
int getGluNurbsError34(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError35(void)  "
 /**
  * @return GLU_NURBS_ERROR35
  */
public";
int getGluNurbsError35(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError36(void)  "
 /**
  * @return GLU_NURBS_ERROR36
  */
public";
int getGluNurbsError36(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsError37(void)  "
 /**
  * @return GLU_NURBS_ERROR37
  */
public";
int getGluNurbsError37(void);

/* JavaDoc */
%javamethodmodifiers getGluAutoLoadMatrix(void)  "
 /**
  * @return GLU_AUTO_LOAD_MATRIX
  */
public";
int getGluAutoLoadMatrix(void);

/* JavaDoc */
%javamethodmodifiers getGluCulling(void)  "
 /**
  * @return GLU_CULLING
  */
public";
int getGluCulling(void);

/* JavaDoc */
%javamethodmodifiers getGluSamplingTolerance(void)  "
 /**
  * @return GLU_SAMPLING_TOLERANCE
  */
public";
int getGluSamplingTolerance(void);

/* JavaDoc */
%javamethodmodifiers getGluDisplayMode(void)  "
 /**
  * @return GLU_DISPLAY_MODE
  */
public";
int getGluDisplayMode(void);

/* JavaDoc */
%javamethodmodifiers getGluParametricTolerance(void)  "
 /**
  * @return GLU_PARAMETRIC_TOLERANCE
  */
public";
int getGluParametricTolerance(void);

/* JavaDoc */
%javamethodmodifiers getGluSamplingMethod(void)  "
 /**
  * @return GLU_SAMPLING_METHOD
  */
public";
int getGluSamplingMethod(void);

/* JavaDoc */
%javamethodmodifiers getGluUStep(void)  "
 /**
  * @return GLU_U_STEP
  */
public";
int getGluUStep(void);

/* JavaDoc */
%javamethodmodifiers getGluVStep(void)  "
 /**
  * @return GLU_V_STEP
  */
public";
int getGluVStep(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsMode(void)  "
 /**
  * @return GLU_NURBS_MODE
  */
public";
int getGluNurbsMode(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsModeExt(void)  "
 /**
  * @return GLU_NURBS_MODE_EXT
  */
public";
int getGluNurbsModeExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsTessellator(void)  "
 /**
  * @return GLU_NURBS_TESSELLATOR
  */
public";
int getGluNurbsTessellator(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsTessellatorExt(void)  "
 /**
  * @return GLU_NURBS_TESSELLATOR_EXT
  */
public";
int getGluNurbsTessellatorExt(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsRenderer(void)  "
 /**
  * @return GLU_NURBS_RENDERER
  */
public";
int getGluNurbsRenderer(void);

/* JavaDoc */
%javamethodmodifiers getGluNurbsRendererExt(void)  "
 /**
  * @return GLU_NURBS_RENDERER_EXT
  */
public";
int getGluNurbsRendererExt(void);

/* JavaDoc */
%javamethodmodifiers getGluObjectParametricError(void)  "
 /**
  * @return GLU_OBJECT_PARAMETRIC_ERROR
  */
public";
int getGluObjectParametricError(void);

/* JavaDoc */
%javamethodmodifiers getGluObjectParametricErrorExt(void)  "
 /**
  * @return GLU_OBJECT_PARAMETRIC_ERROR_EXT
  */
public";
int getGluObjectParametricErrorExt(void);

/* JavaDoc */
%javamethodmodifiers getGluObjectPathLength(void)  "
 /**
  * @return GLU_OBJECT_PATH_LENGTH
  */
public";
int getGluObjectPathLength(void);

/* JavaDoc */
%javamethodmodifiers getGluObjectPathLengthExt(void)  "
 /**
  * @return GLU_OBJECT_PATH_LENGTH_EXT
  */
public";
int getGluObjectPathLengthExt(void);

/* JavaDoc */
%javamethodmodifiers getGluPathLength(void)  "
 /**
  * @return GLU_PATH_LENGTH
  */
public";
int getGluPathLength(void);

/* JavaDoc */
%javamethodmodifiers getGluParametricError(void)  "
 /**
  * @return GLU_PARAMETRIC_ERROR
  */
public";
int getGluParametricError(void);

/* JavaDoc */
%javamethodmodifiers getGluDomainDistance(void)  "
 /**
  * @return GLU_DOMAIN_DISTANCE
  */
public";
int getGluDomainDistance(void);

/* JavaDoc */
%javamethodmodifiers getGluMap1Trim2(void)  "
 /**
  * @return GLU_MAP1_TRIM_2
  */
public";
int getGluMap1Trim2(void);

/* JavaDoc */
%javamethodmodifiers getGluMap1Trim3(void)  "
 /**
  * @return GLU_MAP1_TRIM_3
  */
public";
int getGluMap1Trim3(void);
