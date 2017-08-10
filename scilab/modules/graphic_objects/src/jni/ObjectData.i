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

// swig -java -package org.scilab.modules.graphic_objects -outdir ../java/org/scilab/modules/graphic_objects/ ObjectData.i

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

%apply double * CHAMPX { double * getChampX(int uid)}


%{

#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "sci_malloc.h"
#include "math.h"

double * getChampX(int uid)
{
    double * vx;
    getGraphicObjectProperty(uid, __GO_BASE_X__, jni_double_vector, (void**)&vx);
    return vx;
}

int _getChampXSize(int uid) {
    
    int * dimension;
    getGraphicObjectProperty(uid, __GO_CHAMP_DIMENSIONS__, jni_int_vector, (void**)&dimension);
    return dimension[0];
}
%}



%typemap(out) double * CHAMPY {
	$result = (*jenv)->NewDoubleArray(jenv, _getChampYSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getChampYSize(arg1), $1);
}

%apply double * CHAMPY { double * getChampY(int uid) }
%{

double * getChampY(int uid) {

    double * vy;
    getGraphicObjectProperty(uid, __GO_BASE_Y__, jni_double_vector, (void**)&vy);
    return vy;
}

int _getChampYSize(int uid) {
    
    int * dimension;
    getGraphicObjectProperty(uid, __GO_CHAMP_DIMENSIONS__, jni_int_vector, (void**)&dimension);
    return dimension[1];
}
%}



%typemap(out) double * ARROWS {
	$result = (*jenv)->NewDoubleArray(jenv, _getArrowsSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getArrowsSize(arg1), $1);
}

%apply double * ARROWS { double * getArrows(int uid) }
%{

double * getArrows(int uid) {

    double * arrows;
    getGraphicObjectProperty(uid, __GO_DIRECTION__, jni_double_vector, (void**)&arrows);
    return arrows;
}

int _getArrowsSize(int uid) {
    
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

%apply double * SEGS { double * getSegsData(int uid) }
%{

double * getSegsData(int uid) {

    double * data;
    getGraphicObjectProperty(uid, __GO_BASE__, jni_double_vector, (void**)&data);
    return data;
}

int _getSegsSize(int uid) {
    
    int base;
    int * pBase = &base;
    getGraphicObjectProperty(uid, __GO_NUMBER_ARROWS__, jni_int, (void**)&pBase);
    return base * 3;
}
%}

%typemap(out) double * FEC {
	$result = (*jenv)->NewDoubleArray(jenv, _getFecElementsSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getFecElementsSize(arg1), $1);
}

%apply double * FEC { double * getFecElements(int uid) }
%{

double * getFecElements(int uid) {

    double * elements;
    getGraphicObjectProperty(uid, __GO_DATA_MODEL_FEC_ELEMENTS__, jni_double_vector, (void**)&elements);
    return elements;
}

int _getFecElementsSize(int uid) {
    
    int indices;
    int * pIndices = &indices;
    int nVertex = 0;
    int* piNVertex = &nVertex;

    getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_INDICES__, jni_int, (void**)&pIndices);
    getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, jni_int, (void**) &piNVertex);
    
    return indices * (nVertex + 2);
}

int getFecNumVerticesByElement(int uid) {
    
    int nVertex = 0;
    int* piNVertex = &nVertex;

    getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, jni_int, (void**) &piNVertex);
    
    return nVertex;
}

%}

%typemap(out) double * FEC_XY {
	$result = (*jenv)->NewDoubleArray(jenv, _getFecDataSize(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, _getFecDataSize(arg1), $1);
}

%apply double * FEC_XY { double * getFecData(int uid) }
%{

double * getFecData(int uid) {

    double * data;
    getGraphicObjectProperty(uid, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**)&data);
    return data;
}

int _getFecDataSize(int uid) {
    
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

%apply double * ARC_ULP { double * getArcUpperLeftPoint(int uid) }
%{

double * getArcUpperLeftPoint(int uid) {

    double * upperLeftPoint;
    getGraphicObjectProperty(uid, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void**)&upperLeftPoint);
    return upperLeftPoint;
}

%}

%typemap(out) double * ARC_DATA {
	$result = (*jenv)->NewDoubleArray(jenv, 4);
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, 4, $1);
}

%apply double * ARC_DATA { double * getArcData(int uid) }
%{

double * getArcData(int uid) {

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


double * getChampX(int uid);
double * getChampY(int uid);
double * getArrows(int uid);
double * getSegsData(int uid);
double * getFecElements(int uid);
double * getFecData(int uid);
double * getArcUpperLeftPoint(int uid);
double * getArcData(int uid);
int getFecNumVerticesByElement(int uid);
