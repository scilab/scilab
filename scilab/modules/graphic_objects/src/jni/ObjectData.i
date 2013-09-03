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


double * getChampX(char * uid);
double * getChampY(char * uid);
double * getArrows(char * uid);
double * getSegsData(char * uid);