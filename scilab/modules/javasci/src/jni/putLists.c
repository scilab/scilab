/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <jni.h>
#include "api_scilab.h"
#include "sci_types.h"
#include "sci_malloc.h"
#include "BOOL.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*--------------------------------------------------------------------------*/
    SciErr sendList(JNIEnv * jenv, jobject list, int * parentList, int pos, char * varName, char listType);
    JNIEXPORT jint JNICALL Java_org_scilab_modules_javasci_Call_1ScilabJNI_putList(JNIEnv *jenv, jclass cl, jstring jvarName, jobject list, jchar type);
    /*--------------------------------------------------------------------------*/

    JNIEXPORT jint JNICALL Java_org_scilab_modules_javasci_Call_1ScilabJNI_putList(JNIEnv *jenv, jclass cl, jstring jvarName, jobject list, jchar type)
    {
        char * varName = (char*)(*jenv)->GetStringUTFChars(jenv, jvarName, 0);
        SciErr sciErr = sendList(jenv, list, 0, 0, varName, (char)type);
        (*jenv)->ReleaseStringUTFChars(jenv, jvarName, (const char *)varName);

        if (sciErr.iErr)
        {
            return -1;
        }

        return 0;
    }

    SciErr sendList(JNIEnv * jenv, jobject list, int * parentList, int pos, char * varName, char listType)
    {
        int listLen = (*jenv)->GetArrayLength(jenv, list);
        jboolean isCopy = JNI_FALSE;
        jintArray jtypes = (jintArray)(*jenv)->GetObjectArrayElement(jenv, list, 0);
        jint * types = (*jenv)->GetIntArrayElements(jenv, jtypes, &isCopy);
        int i = 0;
        SciErr sciErr;
        int * currentList = 0;
        if (parentList)
        {
            switch (listType)
            {
                case 'l' :
                    sciErr = createListInNamedList(NULL, varName, parentList, pos, listLen - 1, &currentList);
                    break;
                case 't' :
                    sciErr = createTListInNamedList(NULL, varName, parentList, pos, listLen - 1, &currentList);
                    break;
                case 'm' :
                    sciErr = createMListInNamedList(NULL, varName, parentList, pos, listLen - 1, &currentList);
                    break;
            }
        }
        else
        {
            //cleanStackListAddress();
            switch (listType)
            {
                case 'l' :
                    sciErr = createNamedList(NULL, varName, listLen - 1, &currentList);
                    break;
                case 't' :
                    sciErr = createNamedTList(NULL, varName, listLen - 1, &currentList);
                    break;
                case 'm' :
                    sciErr = createNamedMList(NULL, varName, listLen - 1, &currentList);
                    break;
            }
        }

        if (sciErr.iErr)
        {
            (*jenv)->ReleaseIntArrayElements(jenv, jtypes, types, JNI_ABORT);
            (*jenv)->DeleteLocalRef(jenv, jtypes);
            return sciErr;
        }

        //pushListAddress(currentList);

        for (; i < listLen - 1; i++)
        {
            switch (types[i])
            {
                case sci_matrix :
                {
                    int nbCol = 0;
                    int j = 0, k = 0;
                    double * real = 0;
                    double * imag = 0;
                    int nbRow = 0;

                    jobjectArray infos = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    int isReal = (*jenv)->GetArrayLength(jenv, infos) == 1;
                    jobjectArray data = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, infos, 0);

                    if (!data)
                    {
                        // empty matrix
                        sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                        (*jenv)->DeleteLocalRef(jenv, infos);
                        break;
                    }

                    nbRow = (*jenv)->GetArrayLength(jenv, data);

                    if (isReal)
                    {
                        // Get the matrix rowsw
                        for (; j < nbRow; j++)
                        {
                            jboolean isCopy1 = JNI_FALSE;
                            jdouble* element = NULL;
                            jdoubleArray oneDim = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, data, j);
                            if (nbCol == 0)
                            {
                                nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                real = (double*)MALLOC(sizeof(double) * nbRow * nbCol);
                            }

                            element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            for (k = 0; k < nbCol; k++)
                            {
                                real[k * nbRow + j] = element[k];
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, data);

                        sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, real);
                        FREE(real);
                    }
                    else
                    {
                        jobject imagData = (jobject)(*jenv)->GetObjectArrayElement(jenv, infos, 1);
                        // Get the matrix rows
                        for (; j < nbRow; j++)
                        {
                            jboolean isCopy1 = JNI_FALSE;
                            jdouble* elementR = NULL;
                            jdouble* elementI = NULL;
                            jdoubleArray oneDimR = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, data, j);
                            jdoubleArray oneDimI = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, imagData, j);
                            if (nbCol == 0)
                            {
                                nbCol = (*jenv)->GetArrayLength(jenv, oneDimR);
                                real = (double*)MALLOC(sizeof(double) * nbRow * nbCol);
                                imag = (double*)MALLOC(sizeof(double) * nbRow * nbCol);
                            }

                            elementR = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimR, &isCopy1);
                            isCopy1 = JNI_FALSE;
                            elementI = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimI, &isCopy1);

                            // Get the matrix element
                            for (k = 0; k < nbCol; k++)
                            {
                                real[k * nbRow + j] = elementR[k];
                                imag[k * nbRow + j] = elementI[k];
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimR, elementR, JNI_ABORT);
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimI, elementI, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDimR);
                            (*jenv)->DeleteLocalRef(jenv, oneDimI);
                        }
                        (*jenv)->DeleteLocalRef(jenv, data);
                        (*jenv)->DeleteLocalRef(jenv, imagData);

                        sciErr = createComplexMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, real, imag);
                        FREE(real);
                        FREE(imag);
                    }
                    (*jenv)->DeleteLocalRef(jenv, infos);
                    break;
                }
                case sci_poly :
                {
                    jobjectArray infos = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    jobject data = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                    int nbRow = (*jenv)->GetArrayLength(jenv, data);
                    int isReal = 0;
                    jstring js;
                    char * polyVarName = NULL;
                    int nbCol = 0;
                    int j = 0, k = 0;
                    double ** real = NULL;
                    double ** imag = NULL;
                    int * nbCoef = NULL;

                    if (!data || nbRow == 0)
                    {
                        sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                        if (!data)
                        {
                            (*jenv)->DeleteLocalRef(jenv, data);
                        }
                        (*jenv)->DeleteLocalRef(jenv, infos);
                        break;
                    }

                    isReal = (*jenv)->GetArrayLength(jenv, infos) == 2;
                    js = (jstring)(*jenv)->GetObjectArrayElement(jenv, infos, 0);
                    polyVarName = (char*)(*jenv)->GetStringUTFChars(jenv, js, 0);

                    if (isReal)
                    {
                        // Get the matrix rows
                        for (; j < nbRow; j++)
                        {
                            jobjectArray dblDim = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, data, j);
                            if (nbCol == 0)
                            {
                                nbCol = (*jenv)->GetArrayLength(jenv, dblDim);
                                real = (double**)MALLOC(sizeof(double*) * nbRow * nbCol);
                                nbCoef = (int*)MALLOC(sizeof(int) * nbRow * nbCol);
                            }

                            // Get the matrix element
                            for (k = 0; k < nbCol; k++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jdouble* element = NULL;
                                jdoubleArray oneDim = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, dblDim, k);
                                int len = (*jenv)->GetArrayLength(jenv, oneDim);
                                nbCoef[k * nbRow + j] = len;
                                real[k * nbRow + j] = (double*)MALLOC(sizeof(double) * len);
                                element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);
                                memcpy(real[k * nbRow + j], element, sizeof(double) * len);
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, 0);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, dblDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, data);

                        sciErr = createMatrixOfPolyInNamedList(NULL, varName, currentList, i + 1, polyVarName, nbRow, nbCol, nbCoef, (const double * const*)real);
                        for (j = 0; j < nbRow * nbCol; j++)
                        {
                            FREE(real[j]);
                        }
                        FREE(real);
                        FREE(nbCoef);
                    }
                    else
                    {
                        jobject imagData = (*jenv)->GetObjectArrayElement(jenv, infos, 2);
                        // Get the matrix rows
                        for (; j < nbRow; j++)
                        {
                            jobjectArray dblDimR = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, data, j);
                            jobjectArray dblDimI = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, imagData, j);
                            if (nbCol == 0)
                            {
                                nbCol = (*jenv)->GetArrayLength(jenv, dblDimR);
                                real = (double**)MALLOC(sizeof(double*) * nbRow * nbCol);
                                imag = (double**)MALLOC(sizeof(double*) * nbRow * nbCol);
                                nbCoef = (int*)MALLOC(sizeof(int) * nbRow * nbCol);
                            }

                            // Get the matrix element
                            for (k = 0; k < nbCol; k++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jdouble* element = NULL;
                                jdoubleArray oneDimR = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, dblDimR, k);
                                jdoubleArray oneDimI = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, dblDimI, k);
                                int len = (*jenv)->GetArrayLength(jenv, oneDimR);
                                nbCoef[k * nbRow + j] = len;
                                real[k * nbRow + j] = (double*)MALLOC(sizeof(double) * len);
                                imag[k * nbRow + j] = (double*)MALLOC(sizeof(double) * len);
                                element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimR, &isCopy1);
                                memcpy(real[k * nbRow + j], element, sizeof(double) * len);
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimR, element, 0);
                                (*jenv)->DeleteLocalRef(jenv, oneDimR);
                                isCopy1 = JNI_FALSE;
                                element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimI, &isCopy1);
                                memcpy(imag[k * nbRow + j], element, sizeof(double) * len);
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimI, element, 0);
                                (*jenv)->DeleteLocalRef(jenv, oneDimI);
                            }
                            (*jenv)->DeleteLocalRef(jenv, dblDimR);
                            (*jenv)->DeleteLocalRef(jenv, dblDimI);
                        }
                        (*jenv)->DeleteLocalRef(jenv, data);
                        (*jenv)->DeleteLocalRef(jenv, imagData);

                        sciErr = createComplexMatrixOfPolyInNamedList(NULL, varName, currentList, i + 1, polyVarName, nbRow, nbCol, nbCoef, (const double * const*)real, (const double * const*)imag);
                        for (j = 0; j < nbRow * nbCol; j++)
                        {
                            FREE(real[j]);
                            FREE(imag[j]);
                        }
                        FREE(real);
                        FREE(imag);
                        FREE(nbCoef);
                    }

                    (*jenv)->ReleaseStringUTFChars(jenv, js, polyVarName);
                    (*jenv)->DeleteLocalRef(jenv, js);
                    (*jenv)->DeleteLocalRef(jenv, infos);
                    break;
                }
                case sci_boolean :
                {
                    jobject data = (*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    int nbRow = (*jenv)->GetArrayLength(jenv, data);
                    int nbCol = 0;
                    int j = 0, k;
                    int * b = 0;

                    if (!data || nbRow == 0)
                    {
                        sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                        if (!data)
                        {
                            (*jenv)->DeleteLocalRef(jenv, data);
                        }
                        break;
                    }

                    // Get the matrix rows
                    for (; j < nbRow; j++)
                    {
                        jboolean isCopy1 = JNI_FALSE;
                        jboolean* element = NULL;
                        jdoubleArray oneDim = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, data, j);
                        if (nbCol == 0)
                        {
                            nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                            b = (int*)MALLOC(sizeof(int) * nbRow * nbCol);
                        }

                        element = (jboolean*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                        // Get the matrix element
                        for (k = 0; k < nbCol; k++)
                        {
                            b[k * nbRow + j] = element[k];
                        }
                        (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                        (*jenv)->DeleteLocalRef(jenv, oneDim);
                    }
                    (*jenv)->DeleteLocalRef(jenv, data);

                    sciErr = createMatrixOfBooleanInNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, (const int *)b);
                    FREE(b);
                    break;
                }
                case sci_sparse :
                {
                    jboolean isCopy1 = JNI_FALSE;
                    int* dims = NULL;
                    int* nbItemRow = NULL;
                    int* colPos = NULL;
                    double* data = NULL;
                    double * imagData = NULL;
                    jobjectArray infos = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    int isReal = (*jenv)->GetArrayLength(jenv, infos) == 4;
                    jintArray jdims = (jintArray)(*jenv)->GetObjectArrayElement(jenv, infos, 0);
                    jintArray jnbItemRow = (jintArray)(*jenv)->GetObjectArrayElement(jenv, infos, 1);
                    jintArray jcolPos = (jintArray)(*jenv)->GetObjectArrayElement(jenv, infos, 2);
                    jint nbNonNull = (*jenv)->GetArrayLength(jenv, jcolPos);
                    jdoubleArray jdata = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, infos, 3);
                    jdoubleArray jimagData;

                    if (!isReal)
                    {
                        jimagData = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, infos, 4);
                    }

                    dims = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, jdims, &isCopy1);
                    isCopy1 = JNI_FALSE;
                    nbItemRow = (int*)(*jenv)->GetPrimitiveArrayCritical(jenv, jnbItemRow, &isCopy1);
                    isCopy1 = JNI_FALSE;
                    colPos = (int*)(*jenv)->GetPrimitiveArrayCritical(jenv, jcolPos, &isCopy1);
                    isCopy1 = JNI_FALSE;
                    data = (double*)(*jenv)->GetPrimitiveArrayCritical(jenv, jdata, &isCopy1);
                    isCopy1 = JNI_FALSE;

                    if (!isReal)
                    {
                        imagData = (double*)(*jenv)->GetPrimitiveArrayCritical(jenv, jimagData, &isCopy1);
                    }

                    if (isReal)
                    {
                        sciErr = createSparseMatrixInNamedList(NULL, varName, currentList, i + 1, dims[0], dims[1], nbNonNull, nbItemRow, colPos, data);
                        (*jenv)->ReleasePrimitiveArrayCritical(jenv, jdata, data, JNI_ABORT);
                    }
                    else
                    {
                        sciErr = createComplexSparseMatrixInNamedList(NULL, varName, currentList, i + 1, dims[0], dims[1], nbNonNull, nbItemRow, colPos, data, imagData);
                        (*jenv)->ReleasePrimitiveArrayCritical(jenv, jimagData, imagData, JNI_ABORT);
                        (*jenv)->ReleasePrimitiveArrayCritical(jenv, jdata, data, JNI_ABORT);
                    }

                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, jcolPos, colPos, JNI_ABORT);
                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, jnbItemRow, nbItemRow, JNI_ABORT);
                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, jdims, dims, JNI_ABORT);

                    if (!isReal)
                    {
                        (*jenv)->DeleteLocalRef(jenv, jimagData);
                    }
                    (*jenv)->DeleteLocalRef(jenv, jdata);
                    (*jenv)->DeleteLocalRef(jenv, jcolPos);
                    (*jenv)->DeleteLocalRef(jenv, jnbItemRow);
                    (*jenv)->DeleteLocalRef(jenv, jdims);
                    (*jenv)->DeleteLocalRef(jenv, infos);

                    break;
                }
                case sci_boolean_sparse :
                {
                    jobjectArray infos = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    jintArray jdims = (jintArray)(*jenv)->GetObjectArrayElement(jenv, infos, 0);
                    jintArray jnbItemRow = (jintArray)(*jenv)->GetObjectArrayElement(jenv, infos, 1);
                    jintArray jcolPos = (jintArray)(*jenv)->GetObjectArrayElement(jenv, infos, 2);
                    jint nbNonNull = (*jenv)->GetArrayLength(jenv, jcolPos);
                    int* nbItemRow = NULL;
                    int* dims = NULL;
                    int* colPos = NULL;

                    jboolean isCopy1 = JNI_FALSE;

                    dims = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, jdims, &isCopy1);
                    isCopy1 = JNI_FALSE;
                    nbItemRow = (int*)(*jenv)->GetPrimitiveArrayCritical(jenv, jnbItemRow, &isCopy1);
                    isCopy1 = JNI_FALSE;
                    colPos = (int*)(*jenv)->GetPrimitiveArrayCritical(jenv, jcolPos, &isCopy1);

                    sciErr = createBooleanSparseMatrixInNamedList(NULL, varName, currentList, i + 1, dims[0], dims[1], nbNonNull, nbItemRow, colPos);

                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, jcolPos, colPos, JNI_ABORT);
                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, jnbItemRow, nbItemRow, JNI_ABORT);
                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, jdims, dims, JNI_ABORT);

                    (*jenv)->DeleteLocalRef(jenv, jcolPos);
                    (*jenv)->DeleteLocalRef(jenv, jnbItemRow);
                    (*jenv)->DeleteLocalRef(jenv, jdims);
                    (*jenv)->DeleteLocalRef(jenv, infos);

                    break;
                }
                case sci_ints :
                {
                    jobjectArray infos = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    jintArray jtype = (jintArray)(*jenv)->GetObjectArrayElement(jenv, infos, 0);
                    jint * typep = (*jenv)->GetIntArrayElements(jenv, jtype, 0);
                    int type = *typep;

                    (*jenv)->ReleaseIntArrayElements(jenv, jtype, typep, JNI_ABORT);
                    (*jenv)->DeleteLocalRef(jenv, jtype);

                    switch (type)
                    {
                        case sci_int8 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            char * data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jbyte* element = NULL;
                                jbyteArray oneDim = (jbyteArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (char*)MALLOC(sizeof(char) * nbRow * nbCol);
                                }
                                isCopy1 = JNI_FALSE;
                                element = (jbyte*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfInteger8InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);
                            break;
                        }
                        case sci_uint8 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            unsigned char * data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jbyte* element = NULL;
                                jboolean isCopy1 = JNI_FALSE;
                                jbyteArray oneDim = (jbyteArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (unsigned char*)MALLOC(sizeof(unsigned char) * nbRow * nbCol);
                                }

                                element = (jbyte*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfUnsignedInteger8InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);

                            break;
                        }
                        case sci_int16 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            short * data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jshort* element = NULL;
                                jshortArray oneDim = (jshortArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (short*)MALLOC(sizeof(short) * nbRow * nbCol);
                                }

                                element = (jshort*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfInteger16InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);

                            break;
                        }
                        case sci_uint16 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            unsigned short * data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jshort* element = NULL;
                                jshortArray oneDim = (jshortArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (unsigned short*)MALLOC(sizeof(unsigned short) * nbRow * nbCol);
                                }
                                isCopy1 = JNI_FALSE;
                                element = (jshort*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfUnsignedInteger16InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);

                            break;
                        }
                        case sci_int32 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            int * data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jint* element = NULL;
                                jintArray oneDim = (jintArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (int*)MALLOC(sizeof(int) * nbRow * nbCol);
                                }
                                isCopy1 = JNI_FALSE;
                                element = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfInteger32InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);

                            break;
                        }
                        case sci_uint32 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            unsigned int * data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jint* element = NULL;
                                jintArray oneDim = (jintArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (unsigned int*)MALLOC(sizeof(unsigned int) * nbRow * nbCol);
                                }
                                isCopy1 = JNI_FALSE;
                                element = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfUnsignedInteger32InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);

                            break;
                        }
#ifdef __SCILAB_INT64__
                        case sci_int64 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            long long* data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jlong* element = NULL;
                                jlongArray oneDim = (jlongArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (long long*)MALLOC(sizeof(long long) * nbRow * nbCol);
                                }
                                isCopy1 = JNI_FALSE;
                                element = (jlong*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfInteger64InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);

                            break;
                        }
                        case sci_uint64 :
                        {
                            jobject jdata = (*jenv)->GetObjectArrayElement(jenv, infos, 1);
                            int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                            int nbCol = 0;
                            int j = 0, k;
                            unsigned long long* data = 0;

                            if (!jdata || nbRow == 0)
                            {
                                sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                                if (!jdata)
                                {
                                    (*jenv)->DeleteLocalRef(jenv, jdata);
                                }
                                break;
                            }

                            // Get the matrix rows
                            for (; j < nbRow; j++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jlong* element = NULL;
                                jlongArray oneDim = (jlongArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                                if (nbCol == 0)
                                {
                                    nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                    data = (unsigned long long*)MALLOC(sizeof(unsigned long long) * nbRow * nbCol);
                                }
                                isCopy1 = JNI_FALSE;
                                element = (jlong*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                                // Get the matrix element
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                            (*jenv)->DeleteLocalRef(jenv, jdata);

                            sciErr = createMatrixOfUnsignedInteger64InNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, data);
                            FREE(data);
                            break;
                        }
#endif
                    }
                    (*jenv)->DeleteLocalRef(jenv, infos);
                    break;
                }
                case sci_strings :
                {
                    jobject data = (*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    int nbRow = (*jenv)->GetArrayLength(jenv, data);
                    int nbCol = 0;
                    int j = 0, k;
                    char ** strings = 0;

                    if (!data || nbRow == 0)
                    {
                        sciErr = createMatrixOfDoubleInNamedList(NULL, varName, currentList, i + 1, 0, 0, 0);
                        if (!data)
                        {
                            (*jenv)->DeleteLocalRef(jenv, data);
                        }
                        break;
                    }

                    // Get the matrix rows
                    for (; j < nbRow; j++)
                    {
                        jobjectArray oneDim = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, data, j);
                        if (nbCol == 0)
                        {
                            nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                            strings = (char**)MALLOC(sizeof(char*) * nbRow * nbCol);
                        }

                        // Get the matrix element
                        for (k = 0; k < nbCol; k++)
                        {
                            jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, oneDim, k);
                            jboolean isCopy1 = JNI_FALSE;
                            char *str = (char *)(*jenv)->GetStringUTFChars(jenv, j_string, &isCopy1);
                            strings[k * nbRow + j] = (char*)MALLOC(sizeof(char) * (strlen(str) + 1));
                            strcpy(strings[k * nbRow + j], str);
                            if (isCopy1)
                            {
                                (*jenv)->ReleaseStringUTFChars(jenv, j_string, (const char *)str);
                            }
                            (*jenv)->DeleteLocalRef(jenv, j_string);
                        }
                        (*jenv)->DeleteLocalRef(jenv, oneDim);
                    }
                    (*jenv)->DeleteLocalRef(jenv, data);

                    sciErr = createMatrixOfStringInNamedList(NULL, varName, currentList, i + 1, nbRow, nbCol, (const char * const*)strings);
                    for (j = 0; j < nbRow * nbCol; j++)
                    {
                        FREE(strings[j]);
                    }
                    FREE(strings);
                    break;
                }
                case sci_list :
                {
                    jobject data = (*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    sciErr = sendList(jenv, data, currentList, i + 1, varName, 'l');
                    (*jenv)->DeleteLocalRef(jenv, data);
                    break;
                }
                case sci_tlist :
                {
                    jobject data = (*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    sciErr = sendList(jenv, data, currentList, i + 1, varName, 't');
                    (*jenv)->DeleteLocalRef(jenv, data);
                    break;
                }
                case sci_mlist :
                {
                    jobject data = (*jenv)->GetObjectArrayElement(jenv, list, i + 1);
                    sciErr = sendList(jenv, data, currentList, i + 1, varName, 'm');
                    (*jenv)->DeleteLocalRef(jenv, data);
                    break;
                }
            }

            if (sciErr.iErr)
            {
                break;
            }
        }

        (*jenv)->ReleaseIntArrayElements(jenv, jtypes, types, JNI_ABORT);
        (*jenv)->DeleteLocalRef(jenv, jtypes);

        //popListAddress();

        if (!parentList)
        {
            //pushVariable(varName);
            //cleanStackListAddress();
        }

        return sciErr;
    }

#ifdef __cplusplus
}
#endif
