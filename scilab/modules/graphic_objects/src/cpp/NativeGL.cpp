#include <iostream>
#include <map>
#include "NativeGL.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

void loadGLData(float* buffer, char* id)
{
    int iSize = 0;
    double* t;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &t);
    int* piSize = &iSize;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piSize);

    for (int i = 0 ; i < iSize ; i++)
    {
      buffer[4*i+0] = t[i];
      buffer[4*i+1] = t[i + iSize];
      buffer[4*i+2] = t[i + 2*iSize];
      buffer[4*i+3] = 1.0;
    }
}

int getGLDataLength(char* id)
{
    int iSize = 0;
    int *piSize = &iSize;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piSize);
    return 4 * iSize;
}


} // extern "C"


