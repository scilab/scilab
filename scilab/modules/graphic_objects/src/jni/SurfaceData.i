/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
double * getSurfDataX(int uid)
{
	double * X;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &X);
	return X;
}

int getDataSizeX(int uid)
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
double * getSurfDataY(int uid)
{
	double * Y;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &Y);
	return Y;
}

int getDataSizeY(int uid)
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
double * getSurfDataZ(int uid)
{
	double * Z;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &Z);
	return Z;
}

int getDataSizeZ(int uid)
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
extern int createObject3dData(int obj, int newObj, int type);
extern double pickSurface(int uid, double x, double y,  double z, double dx, double dy, double dz, double mx, double my, double mz, double mw);
%}


double * getSurfDataX(int uid);
double * getSurfDataY(int uid);
double * getSurfDataZ(int uid);
int createObject3dData(int obj, int newObj, int type);
double pickSurface(int uid, double x, double y,  double z, double dx, double dy, double dz, double mx, double my, double mz, double mw);



