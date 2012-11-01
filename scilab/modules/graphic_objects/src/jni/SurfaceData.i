/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

%module SurfaceData
 

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scigraphic_objects");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}

%typemap(jni) double * "jobject"
%typemap(jtype) double * "Object"
%typemap(jstype) double * "Object"
%typemap(javain) double * "$javainput"
%typemap(javaout) double * { return $jnicall; }

%typemap(in) double * getSurfDataX {
	$1 = (*jenv)->GetDoubleArrayElements(jenv, $input, NULL);
}
%typemap(argout) double * getSurfDataX {
	(*jenv)->ReleaseDoubleArrayElements(jenv, $input, $1, 0);
}
%typemap(out) double * getSurfDataX {
	$result = (*jenv)->NewDoubleArray(jenv, getDataSizeX(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, getDataSizeX(arg1), $1);
}

%typemap(in) double * getSurfDataY {
	$1 = (*jenv)->GetDoubleArrayElements(jenv, $input, NULL);
}
%typemap(argout) double * getSurfDataY {
	(*jenv)->ReleaseDoubleArrayElements(jenv, $input, $1, 0);
}
%typemap(out) double * getSurfDataY {
	$result = (*jenv)->NewDoubleArray(jenv, getDataSizeY(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, getDataSizeY(arg1), $1);
}

%typemap(in) double * getSurfDataZ {
	$1 = (*jenv)->GetDoubleArrayElements(jenv, $input, NULL);
}
%typemap(argout) double * getSurfDataZ {
	(*jenv)->ReleaseDoubleArrayElements(jenv, $input, $1, 0);
}
%typemap(out) double * getSurfDataZ {
	$result = (*jenv)->NewDoubleArray(jenv, getDataSizeZ(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, getDataSizeZ(arg1), $1);
}

%{
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
%}

%{
double * getSurfDataX(char * uid)
{
	double * X;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &X);
	return X;
}

int getDataSizeX(char * uid)
{
	int type, size = 0, nGon = 0, nVert = 0;
	int * pType = &type;
	int * pSize = &size;
	int * pNGon = &nGon;
	int * pNVert = &nVert;
	getGraphicObjectProperty(uid, __GO_TYPE__, jni_int, (void**) &pType);

	switch(type)
	{
		case __GO_PLOT3D__:
		case __GO_GRAYPLOT__:
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &pSize);
			break;
		case __GO_FAC3D__:
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &pNGon);
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &pNVert);
			size = nGon * nVert;
			break;
	}
	return size;
}
%}

%{
double * getSurfDataY(char * uid)
{
	double * Y;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &Y);
	return Y;
}

int getDataSizeY(char * uid)
{
	int type, size = 0, nGon = 0, nVert = 0;
	int * pType = &type;
	int * pSize = &size;
	int * pNGon = &nGon;
	int * pNVert = &nVert;
	getGraphicObjectProperty(uid, __GO_TYPE__, jni_int, (void**) &pType);

	switch(type)
	{
		case __GO_PLOT3D__:
		case __GO_GRAYPLOT__:
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &pSize);
			break;
		case __GO_FAC3D__:
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &pNGon);
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &pNVert);
			size = nGon * nVert;
			break;
	}
	return size;
}
%}

%{
double * getSurfDataZ(char * uid)
{
	double * Z;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &Z);
	return Z;
}

int getDataSizeZ(char * uid)
{
	int type, size = 0, nGon = 0, nVert = 0;
	int * pType = &type;
	int * pSize = &size;
	int * pNGon = &nGon;
	int * pNVert = &nVert;
	getGraphicObjectProperty(uid, __GO_TYPE__, jni_int, (void**) &pType);

	switch(type)
	{
		case __GO_PLOT3D__:
		case __GO_GRAYPLOT__:
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_Z__, jni_int, (void**) &pSize);
			break;
		case __GO_FAC3D__:
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &pNGon);
			getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &pNVert);
			size = nGon * nVert;
			break;
	}
	return size;
}
%}

%{
extern char * createObject3dData(char *obj, char *newObj, int type);
extern double pickSurface(char * uid, double x, double y,  double z, double dx, double dy, double dz, double mx, double my, double mz, double mw);
%}


double * getSurfDataX(char * uid);
double * getSurfDataY(char * uid);
double * getSurfDataZ(char * uid);
char * createObject3dData(char *obj, char *newObj, int type);
double pickSurface(char * uid, double x, double y,  double z, double dx, double dy, double dz, double mx, double my, double mz, double mw);



