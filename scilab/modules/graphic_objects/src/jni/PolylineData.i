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

/* sci_types.i */
/**
 * Windows: swig -java -package org.scilab.modules.graphic_objects -outdir ../java/org/scilab/modules/graphic_objects/ PolylineData.i
 * Other: Use the option --enable-build-swig to the configure
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
#include "sci_malloc.h"
#include "math.h"

/*help funtion*/
int getDataSize_(int uid)
{
	int size = 0;
	int *j = &size;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**)&j);
	return size;
}

BOOL isZCoordSet(int uid)
{
	int result = 0;
	int *pResult = &result;

	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Z_COORDINATES_SET__, jni_int, (void**)&pResult);
	return (BOOL)result;
}

double * getDataX(int uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X__, jni_double_vector, (void**)&vec);
	return vec;
}
double * getDataY(int uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y__, jni_double_vector, (void**)&vec);
	return vec;
}

double * getDataZ(int uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Z__, jni_double_vector, (void**)&vec);
	return vec;
}

double * getShiftX(int uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**)&vec);
	return vec;
}

double * getShiftY(int uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**)&vec);
	return vec;
}

double * getShiftZ(int uid)
{
	double *vec;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double_vector, (void**)&vec);
	return vec;
}

BOOL isXShiftSet(int uid)
{
	int b = 0;
	int * pb = &b;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, jni_int, (void**)&pb);
	return b;
}

BOOL isYShiftSet(int uid)
{
	int b = 0;
	int * pb = &b;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, jni_int, (void**)&pb);
	return b;
}

BOOL isZShiftSet(int uid)
{
	int b = 0;
	int * pb = &b;
	getGraphicObjectProperty(uid, __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, jni_int, (void**)&pb);
	return b;
}


int createPolylineData(int uidFrom, int uidTo)
{
	double *dataFrom, *dataTo;
	int n[2];
	int size = 63;
	int *tmp = &size;
	BOOL result;

    int polylineID = createDataObject(uidTo, __GO_POLYLINE__);
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

BOOL translatePolyline(int uid, double x, double y, double z, int flagX, int flagY, int flagZ)
{
    double *datax = NULL;
    double *datay = NULL;
	double *dataz = NULL;

    int i = 0;
    if (x != 0.0)
    {
        datax = getDataX(uid);
        if (datax == NULL) return FALSE;
        if (flagX) {
            for (i = 0; i < getDataSize_(uid); ++i)
            {
                datax[i] = pow(10.,log10(datax[i]) + x);
            }
        } else {
            for (i = 0; i < getDataSize_(uid); ++i)
            {
                datax[i] += x;
            }
        }
    }
    if (y != 0.0)
    {
        datay = getDataY(uid);
        if (datay == NULL) return FALSE;
        if (flagY) {
            for (i = 0; i < getDataSize_(uid); ++i)
            {
                datay[i] = pow(10.,log10(datay[i]) + y);
            }
        } else {
            for (i = 0; i < getDataSize_(uid); ++i)
            {
                datay[i] += y;
            }
        }
    }
	if (z != 0 && isZCoordSet(uid))
	{
        if (flagZ) {
            for (i = 0; i < getDataSize_(uid); ++i)
            {
                dataz[i] = pow(10.,log10(dataz[i]) + z);
            }
        } else {
            for (i = 0; i < getDataSize_(uid); ++i)
            {
                dataz[i] += z;
            }
        }
	}

	return TRUE;
}

BOOL translatePoint(int uid, int index, double x, double y, double z, int flagX, int flagY, int flagZ)
{
    double *datax = NULL;
    double *datay = NULL;
    double *dataz = NULL;

    int size = getDataSize_(uid);

    if (index >= 0 && index < size) 
    {
	datax = getDataX(uid);
	if (datax == NULL) return FALSE;
	datay = getDataY(uid);
	if (datay == NULL) return FALSE;
	
	if (flagX) {
	    datax[index] *= pow(10., x);
	} else {
	    datax[index] += x;
	}
	
	if (flagY) {
	    datay[index] *= pow(10., y);
	} else {
	    datay[index] += y;
	}

	if (z != 0 && isZCoordSet(uid))
	{
	    dataz = getDataZ(uid);
	    if (flagZ) {
		dataz[index] *= pow(10., z);
	    } else {
		dataz[index] += z;
	    }
	}
    }
    /*update*/
    setGraphicObjectProperty(uid, __GO_DATA_MODEL__, &uid, jni_int, 1);
    return TRUE;
}

BOOL setPointValue(int uid, int index, double x, double y, double z)
{
    double *datax = NULL;
    double *datay = NULL;
    double *dataz = NULL;

    int size = getDataSize_(uid);


	if (index >= 0 && index < size) 
	{
		datax = getDataX(uid);
		if (datax == NULL) return FALSE;
		datay = getDataY(uid);
		if (datay == NULL) return FALSE;

		datax[index] = x;
		datay[index] = y;

		if (z != 0 && isZCoordSet(uid))
		{
			dataz = getDataZ(uid);
			dataz[index] = z;
		}	
	}
	/*update*/
	setGraphicObjectProperty(uid, __GO_DATA_MODEL__, &uid, jni_int, 1);
	return TRUE;
}

BOOL insertPoint(int uid, int index, double x, double y, double z)
{
    double *curData, *newData;
	int size = getDataSize_(uid);
	BOOL result;
	int i ,j, n[2];

	// -1 = inset before the first element
	if (index >= size || index < -1) return FALSE;

	curData = getDataX(uid);
	if (curData == NULL) return FALSE;

	newData = MALLOC(sizeof(double)*3*(size+1));
	if (newData == NULL) return FALSE;

	j = index == -1 ? 1 : 0;
	for (i = 0; i < size; ++i, ++j)
	{
		newData[j] = curData[i];
		newData[(size+1)+j] = curData[size+i];
		newData[(size+1)*2+j] = curData[size*2+i];
		if (i == index)
			++j;
	}

	newData[index+1] = x;
	newData[size+index+2] = y;
	if (isZCoordSet(uid))
	{
		newData[(size+1)*2+index+1] = z;
	}
	else
	{
		newData[(size+1)*2+index+1] = 0.0;
	}

	n[0] = 1; n[1] = size+1;
	
	result = setGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, &n, jni_int_vector, 2);
	setGraphicObjectProperty(uid, __GO_DATA_MODEL_COORDINATES__, newData, jni_double_vector, size+1);

	FREE(newData);
	return TRUE;
}

BOOL removePoint(int uid, int index)
{
	double *curData, *newData;
	int size = getDataSize_(uid);
	BOOL result;
	int i ,j, n[2];

	if (index >= size || index < 0) return FALSE;

	curData = getDataX(uid);
	if (curData == NULL) return FALSE;

	newData = MALLOC(sizeof(double)*3*(size-1));
	if (newData == NULL) return FALSE;

	j = 0;
	for (i = 0; i < size; ++i, ++j)
	{
		if (i == index)
		{
			--j;
			continue;
		}

		newData[j] = curData[i];
		newData[(size-1)+j] = curData[size+i];
		newData[(size-1)*2+j] = curData[size*2+i];
	}

	n[0] = 1; n[1] = size-1;
	result = setGraphicObjectProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, &n, jni_int_vector, 2);
	setGraphicObjectProperty(uid, __GO_DATA_MODEL_COORDINATES__, newData, jni_double_vector, size-1);

	FREE(newData);
	return TRUE;
}
%}


double * getDataX(int uid);
double * getDataY(int uid);
double * getDataZ(int uid);
double * getShiftX(int uid);
double * getShiftY(int uid);
double * getShiftZ(int uid);
int isZCoordSet(int uid);
int isXShiftSet(int uid);
int isYShiftSet(int uid);
int isZShiftSet(int uid);
int createPolylineData(int uidFrom, int uidTo);
int translatePolyline(int uid, double x, double y, double z,  int flagX, int flagY, int flagZ);
int translatePoint(int uid, int index, double x, double y, double z,  int flagX, int flagY, int flagZ);
int setPointValue(int uid, int index, double x, double y, double z);
int insertPoint(int uid, int index, double x, double y, double z);
int removePoint(int uid, int index);




