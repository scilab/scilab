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
%}


double * getDataX(char * uid);
double * getDataY(char * uid);
char * createPolylineData(char * uidFrom, char *uidTo);




