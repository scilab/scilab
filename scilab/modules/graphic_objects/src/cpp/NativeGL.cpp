#include <iostream>
#include <map>
#include "NativeGL.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

void loadGLData(float* buffer, char* id)
{
    int size;
    double* t = (double*) getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector);
    int* size_p = (int*) getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int);
    size = (*size_p);
    
    for (int i = 0 ; i < size ; i++)
    {
      buffer[4*i+0] = t[i];
      buffer[4*i+1] = t[i + size];
      buffer[4*i+2] = t[i + 2*size]; 
      buffer[4*i+3] = 1.0;
    }
}

int getGLDataLength(char* id)
{
  int* size_p = (int*) getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int);
  return 4 * (*size_p);
}


} // extern "C"


