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

%module ObjectData
 

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

%typemap(in) double * {
	$1 = (*jenv)->GetDoubleArrayElements(jenv, $input, NULL);
}

%typemap(argout) double * {
	(*jenv)->ReleaseDoubleArrayElements(jenv, $input, $1, 0);
}

%typemap(jni) int * "jobject"
%typemap(jtype) int * "Object"
%typemap(jstype) int * "Object"
%typemap(javain) int * "$javainput"
%typemap(javaout) int * { return $jnicall; }

%typemap(in) int * {
	$1 = (*jenv)->GetIntArrayElements(jenv, $input, NULL);
}

%typemap(argout) int * {
	(*jenv)->ReleaseIntArrayElements(jenv, $input, $1, 0);
}

%typemap(out) double * CHAMPX {
	$result = (*jenv)->NewDoubleArray(jenv, _getChampXSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getChampXSize(arg1), $1);
}

%apply double * CHAMPX { double * getChampX(char * uid)}


%{

#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "MALLOC.h"
#include "math.h"

double * getChampX(char * uid)
{
    double * vx;
    getGraphicObjectProperty(uid, __GO_BASE_X__, jni_double_vector, (void**)&vx);
    return vx;
}

int _getChampXSize(char * uid) {
    
    int * dimension;
    getGraphicObjectProperty(uid, __GO_CHAMP_DIMENSIONS__, jni_int_vector, (void**)&dimension);
    return dimension[0];
}
%}



%typemap(out) double * CHAMPY {
	$result = (*jenv)->NewDoubleArray(jenv, _getChampYSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getChampYSize(arg1), $1);
}

%apply double * CHAMPY { double * getChampY(char * uid) }
%{

double * getChampY(char * uid) {

    double * vy;
    getGraphicObjectProperty(uid, __GO_BASE_Y__, jni_double_vector, (void**)&vy);
    return vy;
}

int _getChampYSize(char * uid) {
    
    int * dimension;
    getGraphicObjectProperty(uid, __GO_CHAMP_DIMENSIONS__, jni_int_vector, (void**)&dimension);
    return dimension[1];
}
%}



%typemap(out) double * ARROWS {
	$result = (*jenv)->NewDoubleArray(jenv, _getArrowsSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getArrowsSize(arg1), $1);
}

%apply double * ARROWS { double * getArrows(char * uid) }
%{

double * getArrows(char * uid) {

    double * arrows;
    getGraphicObjectProperty(uid, __GO_DIRECTION__, jni_double_vector, (void**)&arrows);
    return arrows;
}

int _getArrowsSize(char * uid) {
    
    int numArrows;
    int * pNumArrows = &numArrows;
    getGraphicObjectProperty(uid, __GO_NUMBER_ARROWS__, jni_int, (void**)&pNumArrows);
    return numArrows * 3;
}
%}



%typemap(out) double * SEGS {
	$result = (*jenv)->NewDoubleArray(jenv, _getSegsSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getSegsSize(arg1), $1);
}

%apply double * SEGS { double * getSegsData(char * uid) }
%{

double * getSegsData(char * uid) {

    double * data;
    getGraphicObjectProperty(uid, __GO_BASE__, jni_double_vector, (void**)&data);
    return data;
}

int _getSegsSize(char * uid) {
    
    int base;
    int * pBase = &base;
    getGraphicObjectProperty(uid, __GO_NUMBER_ARROWS__, jni_int, (void**)&pBase);
    return base * 3;
}
%}

%typemap(out) double * FEC {
	$result = (*jenv)->NewDoubleArray(jenv, _getFecTrianglesSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getFecTrianglesSize(arg1), $1);
}

%apply double * FEC { double * getFecTriangles(char * uid) }
%{

double * getFecTriangles(char * uid) {

    double * triangles;
    getGraphicObjectProperty(uid, __GO_DATA_MODEL_FEC_TRIANGLES__, jni_double_vector, (void**)&triangles);
    return triangles;
}

int _getFecTrianglesSize(char * uid) {
    
    int indices;
    int * pIndices = &indices;
    getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_INDICES__, jni_int, (void**)&pIndices);
    return indices * 5;
}

%}

%typemap(out) double * FEC_XY {
	$result = (*jenv)->NewDoubleArray(jenv, _getFecDataSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getFecDataSize(arg1), $1);
}

%apply double * FEC_XY { double * getFecData(char * uid) }
%{

double * getFecData(char * uid) {

    double * data;
    getGraphicObjectProperty(uid, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**)&data);
    return data;
}

int _getFecDataSize(char * uid) {
    
    int size;
    int * pSize = &size;
    getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**)&pSize);
    return size * 3;
}

%}


%typemap(out) double * ARC_ULP {
	$result = (*jenv)->NewDoubleArray(jenv, 3);
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, 3, $1);
}

%apply double * ARC_ULP { double * getArcUpperLeftPoint(char * uid) }
%{

double * getArcUpperLeftPoint(char * uid) {

    double * upperLeftPoint;
    getGraphicObjectProperty(uid, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void**)&upperLeftPoint);
    return upperLeftPoint;
}

%}

%typemap(out) double * ARC_DATA {
	$result = (*jenv)->NewDoubleArray(jenv, 4);
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, 4, $1);
}

%apply double * ARC_DATA { double * getArcData(char * uid) }
%{

double * getArcData(char * uid) {

    double * data = (double *)MALLOC(sizeof(double)*4);
	double value;
	double * temp = &value;
    getGraphicObjectProperty(uid, __GO_HEIGHT__, jni_double, (void**)&temp);
	data[0] = value;
    getGraphicObjectProperty(uid, __GO_WIDTH__, jni_double, (void**)&temp);
	data[1] = value;
    getGraphicObjectProperty(uid, __GO_START_ANGLE__, jni_double, (void**)&temp);
	data[2] = value;
    getGraphicObjectProperty(uid, __GO_END_ANGLE__, jni_double, (void**)&temp);
	data[3] = value;
    return data;
}

%}


double * getChampX(char * uid);
double * getChampY(char * uid);
double * getArrows(char * uid);
double * getSegsData(char * uid);
double * getFecTriangles(char * uid);
double * getFecData(char * uid);
double * getArcUpperLeftPoint(char * uid);
double * getArcData(char * uid);