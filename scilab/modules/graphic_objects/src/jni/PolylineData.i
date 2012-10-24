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

%module PolylineData
 

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


%typemap(jni) double *BUF "jobject"
%typemap(jtype) double *BUF "Object"
%typemap(jstype) double *BUF "Object"
%typemap(javain) double *BUF "$javainput"
%typemap(javaout) double *BUF { return $jnicall; }

%typemap(in) double *BUF {
	$1 = (*jenv)->GetDoubleArrayElements(jenv, $input, NULL);
}

%typemap(argout) double *BUF {
	(*jenv)->ReleaseDoubleArrayElements(jenv, $input, $1, 0);
}

%typemap(out) double *BUF {
	$result = (*jenv)->NewDoubleArray(jenv, getDataSize_(arg1));
	(*jenv)->SetDoubleArrayRegion(jenv, $result, 0, getDataSize_(arg1), $1);
}

%apply double *BUF { double * }

%{

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include"MALLOC.h"

/*help funtion*/
int getDataSize_(char * uid)
{
	int size = 0;
	int *j = &size;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**)&j);
	return size;
}

double * getDataX(char * uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X__, jni_double_vector, (void**)&vec);
	return vec;
}
double * getDataY(char * uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y__, jni_double_vector, (void**)&vec);
	return vec;
}

double * getShiftX(char * uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**)&vec);
	return vec;
}
double * getShiftY(char * uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**)&vec);
	return vec;
}

BOOL isXShiftSet(char * uid)
{
	int b = 0;
	int * pb = &b;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, jni_int, (void**)&pb);
	return b;
}

BOOL isYShiftSet(char * uid)
{
	int b = 0;
	int * pb = &b;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, jni_int, (void**)&pb);
	return b;
}


char * createPolylineData(char * uidFrom, char *uidTo)
{
	double *dataFrom, *dataTo;
	int n[2];
	int size = 63;
	int *tmp = &size;
	BOOL result;

    char * polylineID = (char *)createDataObject(uidTo, __GO_POLYLINE__);
	if (polylineID == NULL)
    {
        return NULL;
    }

	getGraphicObjectProperty(uidFrom, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**)&tmp);
	n[0] = 1; n[1] = size;
	
	result = setGraphicObjectProperty(polylineID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, &n, jni_int_vector, 2);
	if (result == FALSE)
    {
            deleteDataObject(polylineID);
            return NULL;
    }

	getGraphicObjectProperty(uidFrom, __GO_DATA_MODEL_X__, jni_double_vector, (void**)&dataFrom);
	getGraphicObjectProperty(uidTo, __GO_DATA_MODEL_X__, jni_double_vector, (void**)&dataTo);

	memcpy(dataTo, dataFrom, sizeof(double)*3*size);
	return uidTo;
	
}

void translatePolyline(char *uid, double x, double y)
{
    double *datax = NULL;
    double *datay = NULL;
    int i = 0;
    if (x != 0.0)
    {
        datax = getDataX(uid);
        if (datax == NULL) return;
        for (i = 0; i < getDataSize_(uid); ++i)
        {
            datax[i]+= x;
        }
    }
    if (y != 0.0)
    {
        datay = getDataY(uid);
        if (datay == NULL) return;
        for (i = 0; i < getDataSize_(uid); ++i)
        {
            datay[i]+= y;
        }
    }
}

void translatePoint(char * uid, int index, double x, double y)
{
    double *datax = NULL;
    double *datay = NULL;
    int size = getDataSize_(uid);

	if (index >= 0 && index < size) 
	{
		datax = getDataX(uid);
		if (datax == NULL) return;
		datay = getDataY(uid);
		if (datay == NULL) return;

		datax[index] += x;
		datay[index] += y;
	}
	/*update*/
	setGraphicObjectProperty(uid, __GO_DATA_MODEL__, uid, jni_string, 1);
}

void insertPoint(char * uid, int index, double x, double y)
{
    double *curData, *newData;
	int size = getDataSize_(uid);
	BOOL result;
	int i ,j, n[2];

	if (index >= size || index < 0) return;

	curData = getDataX(uid);
	if (curData == NULL) return;

	newData = MALLOC(sizeof(double)*3*(size+1));
	if (newData == NULL) return;

	j = 0;
	for (i = 0; i < size; ++i, ++j)
	{
		newData[j] = curData[i];
		newData[(size+1)+j] = curData[size+i];
		if (i == index)
			++j;
	}

	newData[index+1] = x;
	newData[size+index+2] = y;

	n[0] = 1; n[1] = size+1;
	
	result = setGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, &n, jni_int_vector, 2);
	setGraphicObjectProperty(uid, __GO_DATA_MODEL_COORDINATES__, newData, jni_double_vector, size+1);

	FREE(newData);
}

void removePoint(char * uid, int index)
{
	double *curData, *newData;
	int size = getDataSize_(uid);
	BOOL result;
	int i ,j, n[2];

	if (index >= size || index < 0) return;

	curData = getDataX(uid);
	if (curData == NULL) return;

	newData = MALLOC(sizeof(double)*3*(size-1));
	if (newData == NULL) return;

	j = 0;
	for (i = 0; i < size; ++i, ++j)
	{
		newData[j] = curData[i];
		newData[(size-1)+j] = curData[size+i];
		if (i == index)
			--j;
	}

	n[0] = 1; n[1] = size-1;
	result = setGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, &n, jni_int_vector, 2);
	setGraphicObjectProperty(uid, __GO_DATA_MODEL_COORDINATES__, newData, jni_double_vector, size-1);

	FREE(newData);
}
%}


double * getDataX(char * uid);
double * getDataY(char * uid);
double * getShiftX(char * uid);
double * getShiftY(char * uid);
int isXShiftSet(char * uid);
int isYShiftSet(char * uid);
char * createPolylineData(char * uidFrom, char *uidTo);
void translatePolyline(char *uid, double x, double y);
void translatePoint(char * uid, int index, double x, double y);
void insertPoint(char * uid, int index, double x, double y);
void removePoint(char * uid, int index);




