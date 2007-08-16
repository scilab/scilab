/*------------------------------------------------------------------------*/
/* file: gluNurbsWrapping.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Wrapping of GLU constant related to NURBS. These functions are  */
/*        to be used from Java code in order to retrieve the constants.   */
/*------------------------------------------------------------------------*/

#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <GL/glu.h>
#include "gluNurbsConstWrapping.hxx"

/** All constants might not be defined on every OS. If not defined return this value */
#define UNDEF_GLU_CONSTANT 0

/*------------------------------------------------------------------------------------------*/
int getGluOutlinePolygon(void)
{
#ifdef GLU_OUTLINE_POLYGON
  return GLU_OUTLINE_POLYGON;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluOutlinePatch(void)
{
#ifdef GLU_OUTLINE_PATCH
  return GLU_OUTLINE_PATCH;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError(void)
{
#ifdef GLU_NURBS_ERROR
  return GLU_NURBS_ERROR;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsBegin(void)
{
#ifdef GLU_NURBS_BEGIN
  return GLU_NURBS_BEGIN;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsBeginExt(void)
{
#ifdef GLU_NURBS_BEGIN_EXT
  return GLU_NURBS_BEGIN_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsVertex(void)
{
#ifdef GLU_NURBS_VERTEX
  return GLU_NURBS_VERTEX;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsVertexExt(void)
{
#ifdef GLU_NURBS_VERTEX_EXT
  return GLU_NURBS_VERTEX_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsNormal(void)
{
#ifdef GLU_NURBS_NORMAL
  return GLU_NURBS_NORMAL;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsNormalExt(void)
{
#ifdef GLU_NURBS_NORMAL_EXT
  return GLU_NURBS_NORMAL_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsColor(void)
{
#ifdef GLU_NURBS_COLOR
  return GLU_NURBS_COLOR;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsColorExt(void)
{
#ifdef GLU_NURBS_COLOR_EXT
  return GLU_NURBS_COLOR_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsTextureCoord(void)
{
#ifdef GLU_NURBS_TEXTURE_COORD
  return GLU_NURBS_TEXTURE_COORD;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsTexCoordExt(void)
{
#ifdef GLU_NURBS_TEX_COORD_EXT
  return GLU_NURBS_TEX_COORD_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsEnd(void)
{
#ifdef GLU_NURBS_END
  return GLU_NURBS_END;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsEndExt(void)
{
#ifdef GLU_NURBS_END_EXT
  return GLU_NURBS_END_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsBeginData(void)
{
#ifdef GLU_NURBS_BEGIN_DATA
  return GLU_NURBS_BEGIN_DATA;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsBeginDataExt(void)
{
#ifdef GLU_NURBS_BEGIN_DATA_EXT
  return GLU_NURBS_BEGIN_DATA_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsVertexData(void)
{
#ifdef GLU_NURBS_VERTEX_DATA
  return GLU_NURBS_VERTEX_DATA;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsVertexDataExt(void)
{
#ifdef GLU_NURBS_VERTEX_DATA_EXT
  return GLU_NURBS_VERTEX_DATA_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsNormalData(void)
{
#ifdef GLU_NURBS_NORMAL_DATA
  return GLU_NURBS_NORMAL_DATA;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsNormalDataExt(void)
{
#ifdef GLU_NURBS_NORMAL_DATA_EXT
  return GLU_NURBS_NORMAL_DATA_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsColorData(void)
{
#ifdef GLU_NURBS_COLOR_DATA
  return GLU_NURBS_COLOR_DATA;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsColorDataExt(void)
{
#ifdef GLU_NURBS_COLOR_DATA_EXT
  return GLU_NURBS_COLOR_DATA_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsTextureCoordData(void)
{
#ifdef GLU_NURBS_TEXTURE_COORD_DATA
  return GLU_NURBS_TEXTURE_COORD_DATA;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsTexCoordDataExt(void)
{
#ifdef GLU_NURBS_TEX_COORD_DATA_EXT
  return GLU_NURBS_TEX_COORD_DATA_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsEndData(void)
{
#ifdef GLU_NURBS_END_DATA
  return GLU_NURBS_END_DATA;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsEndDataExt(void)
{
#ifdef GLU_NURBS_END_DATA_EXT
  return GLU_NURBS_END_DATA_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError1(void)
{
#ifdef GLU_NURBS_ERROR1
  return GLU_NURBS_ERROR1;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError2(void)
{
#ifdef GLU_NURBS_ERROR2
  return GLU_NURBS_ERROR2;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError3(void)
{
#ifdef GLU_NURBS_ERROR3
  return GLU_NURBS_ERROR3;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError4(void)
{
#ifdef GLU_NURBS_ERROR4
  return GLU_NURBS_ERROR4;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError5(void)
{
#ifdef GLU_NURBS_ERROR5
  return GLU_NURBS_ERROR5;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError6(void)
{
#ifdef GLU_NURBS_ERROR6
  return GLU_NURBS_ERROR6;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError7(void)
{
#ifdef GLU_NURBS_ERROR7
  return GLU_NURBS_ERROR7;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError8(void)
{
#ifdef GLU_NURBS_ERROR8
  return GLU_NURBS_ERROR8;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError9(void)
{
#ifdef GLU_NURBS_ERROR9
  return GLU_NURBS_ERROR9;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError10(void)
{
#ifdef GLU_NURBS_ERROR10
  return GLU_NURBS_ERROR10;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError11(void)
{
#ifdef GLU_NURBS_ERROR11
  return GLU_NURBS_ERROR11;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError12(void)
{
#ifdef GLU_NURBS_ERROR12
  return GLU_NURBS_ERROR12;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError13(void)
{
#ifdef GLU_NURBS_ERROR13
  return GLU_NURBS_ERROR13;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError14(void)
{
#ifdef GLU_NURBS_ERROR14
  return GLU_NURBS_ERROR14;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError15(void)
{
#ifdef GLU_NURBS_ERROR15
  return GLU_NURBS_ERROR15;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError16(void)
{
#ifdef GLU_NURBS_ERROR16
  return GLU_NURBS_ERROR16;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError17(void)
{
#ifdef GLU_NURBS_ERROR17
  return GLU_NURBS_ERROR17;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError18(void)
{
#ifdef GLU_NURBS_ERROR18
  return GLU_NURBS_ERROR18;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError19(void)
{
#ifdef GLU_NURBS_ERROR19
  return GLU_NURBS_ERROR19;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError20(void)
{
#ifdef GLU_NURBS_ERROR20
  return GLU_NURBS_ERROR20;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError21(void)
{
#ifdef GLU_NURBS_ERROR21
  return GLU_NURBS_ERROR21;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError22(void)
{
#ifdef GLU_NURBS_ERROR22
  return GLU_NURBS_ERROR22;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError23(void)
{
#ifdef GLU_NURBS_ERROR23
  return GLU_NURBS_ERROR23;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError24(void)
{
#ifdef GLU_NURBS_ERROR24
  return GLU_NURBS_ERROR24;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError25(void)
{
#ifdef GLU_NURBS_ERROR25
  return GLU_NURBS_ERROR25;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError26(void)
{
#ifdef GLU_NURBS_ERROR26
  return GLU_NURBS_ERROR26;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError27(void)
{
#ifdef GLU_NURBS_ERROR27
  return GLU_NURBS_ERROR27;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError28(void)
{
#ifdef GLU_NURBS_ERROR28
  return GLU_NURBS_ERROR28;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError29(void)
{
#ifdef GLU_NURBS_ERROR29
  return GLU_NURBS_ERROR29;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError30(void)
{
#ifdef GLU_NURBS_ERROR30
  return GLU_NURBS_ERROR30;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError31(void)
{
#ifdef GLU_NURBS_ERROR31
  return GLU_NURBS_ERROR31;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError32(void)
{
#ifdef GLU_NURBS_ERROR32
  return GLU_NURBS_ERROR32;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError33(void)
{
#ifdef GLU_NURBS_ERROR33
  return GLU_NURBS_ERROR33;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError34(void)
{
#ifdef GLU_NURBS_ERROR34
  return GLU_NURBS_ERROR34;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError35(void)
{
#ifdef GLU_NURBS_ERROR35
  return GLU_NURBS_ERROR35;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError36(void)
{
#ifdef GLU_NURBS_ERROR36
  return GLU_NURBS_ERROR36;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsError37(void)
{
#ifdef GLU_NURBS_ERROR37
  return GLU_NURBS_ERROR37;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluAutoLoadMatrix(void)
{
#ifdef GLU_AUTO_LOAD_MATRIX
  return GLU_AUTO_LOAD_MATRIX;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluCulling(void)
{
#ifdef GLU_CULLING
  return GLU_CULLING;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluSamplingTolerance(void)
{
#ifdef GLU_SAMPLING_TOLERANCE
  return GLU_SAMPLING_TOLERANCE;
#else
  return UNDEF_GLU_CONSTANT;
#endif

}
/*------------------------------------------------------------------------------------------*/
int getGluDisplayMode(void)
{
#ifdef GLU_DISPLAY_MODE
  return GLU_DISPLAY_MODE;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluParametricTolerance(void)
{
#ifdef GLU_PARAMETRIC_TOLERANCE
  return GLU_PARAMETRIC_TOLERANCE;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluSamplingMethod(void)
{
#ifdef GLU_SAMPLING_METHOD
  return GLU_SAMPLING_METHOD;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluUStep(void)
{
#ifdef GLU_U_STEP
  return GLU_U_STEP;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluVStep(void)
{
#ifdef GLU_V_STEP
  return GLU_V_STEP;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsMode(void)
{
#ifdef GLU_NURBS_MODE
  return GLU_NURBS_MODE;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsModeExt(void)
{
#ifdef GLU_NURBS_MODE_EXT
  return GLU_NURBS_MODE_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsTessellator(void)
{
#ifdef GLU_NURBS_TESSELLATOR
  return GLU_NURBS_TESSELLATOR;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsTessellatorExt(void)
{
#ifdef GLU_NURBS_TESSELLATOR_EXT
  return GLU_NURBS_TESSELLATOR_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsRenderer(void)
{
#ifdef GLU_NURBS_RENDERER
  return GLU_NURBS_RENDERER;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluNurbsRendererExt(void)
{
#ifdef GLU_NURBS_RENDERER_EXT
  return GLU_NURBS_RENDERER_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluObjectParametricError(void)
{
#ifdef GLU_OBJECT_PARAMETRIC_ERROR
  return GLU_OBJECT_PARAMETRIC_ERROR;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluObjectParametricErrorExt(void)
{
#ifdef GLU_OBJECT_PARAMETRIC_ERROR_EXT
  return GLU_OBJECT_PARAMETRIC_ERROR_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluObjectPathLength(void)
{
#ifdef GLU_OBJECT_PATH_LENGTH
  return GLU_OBJECT_PATH_LENGTH;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluObjectPathLengthExt(void)
{
#ifdef GLU_OBJECT_PATH_LENGTH_EXT
  return GLU_OBJECT_PATH_LENGTH_EXT;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluPathLength(void)
{
#ifdef GLU_PATH_LENGTH
  return GLU_PATH_LENGTH;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluParametricError(void)
{
#ifdef GLU_PARAMETRIC_ERROR
  return GLU_PARAMETRIC_ERROR;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluDomainDistance(void)
{
#ifdef GLU_DOMAIN_DISTANCE
  return GLU_DOMAIN_DISTANCE;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluMap1Trim2(void)
{
#ifdef GLU_MAP1_TRIM_2
  return GLU_MAP1_TRIM_2;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
int getGluMap1Trim3(void)
{
#ifdef GLU_MAP1_TRIM_3
  return GLU_MAP1_TRIM_3;
#else
  return UNDEF_GLU_CONSTANT;
#endif
}
/*------------------------------------------------------------------------------------------*/
