/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <jni.h>
#include "api_scilab.h"
#include "stack-c.h"
#include "localization.h"
#include "sci_types.h"
#include "MALLOC.h"
#include "BOOL.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*--------------------------------------------------------------------------*/
    SciErr sendVar(JNIEnv * jenv, int stackPos, int * parentList, int listPos, jobject var, const int type, const int swap);
    JNIEXPORT jint JNICALL Java_org_scilab_modules_types_GetScilabVariableJNI_putVarOnStack(JNIEnv *jenv, jclass cl, jint stackPos, jobject var, jint type, jboolean swap);
    /*--------------------------------------------------------------------------*/

    JNIEXPORT jint JNICALL Java_org_scilab_modules_types_GetScilabVariableJNI_putVarOnStack(JNIEnv *jenv, jclass cl, jint stackPos, jobject var, jint type, jboolean swap)
    {
        SciErr sciErr = sendVar(jenv, stackPos, NULL, 0, var, type, (int)swap);

        if (sciErr.iErr)
        {
            return -1;
        }

        return 0;
    }

    SciErr sendVar(JNIEnv * jenv, int stackPos, int * parentList, int listPos, jobject var, const int type, const int swap)
    {
        SciErr sciErr;

        switch (type)
        {
            case sci_matrix :
            {
                int nbCol = 0;
                int j = 0, k = 0;
                double * real = 0;
                double * imag = 0;
                int nbRow = 0;

                int isReal = (*jenv)->GetArrayLength(jenv, var) == 1;
                jobjectArray data = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, var, 0);

                if (!data)
                {
                    // empty matrix
                    if (parentList)
                    {
                        sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                    }
                    else
                    {
                        sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                    }
                    (*jenv)->DeleteLocalRef(jenv, var);
                    break;
                }

                nbRow = (*jenv)->GetArrayLength(jenv, data);

                if (isReal)
                {
                    // Get the matrix rows
                    for (; j < nbRow; j++)
                    {
                        jboolean isCopy1 = JNI_FALSE;
                        jdouble* element = NULL;
                        jdoubleArray oneDim = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, data, j);
                        if (nbCol == 0)
                        {
                            nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                            if (parentList)
                            {
                                sciErr = allocMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &real);
                            }
                            else
                            {
                                sciErr = allocMatrixOfDouble(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &real);
                            }
                            if (sciErr.iErr)
                            {
                                break;
                            }
                        }

                        element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                        if (swap)
                        {
                            for (k = 0; k < nbCol; k++)
                            {
                                real[k * nbRow + j] = element[k];
                            }
                        }
                        else
                        {
                            for (k = 0; k < nbCol; k++)
                            {
                                real[j * nbCol + k] = element[k];
                            }
                        }
                        (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                        (*jenv)->DeleteLocalRef(jenv, oneDim);
                    }
                    (*jenv)->DeleteLocalRef(jenv, data);
                }
                else
                {
                    jobject imagData = (jobject)(*jenv)->GetObjectArrayElement(jenv, var, 1);
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
                            if (parentList)
                            {
                                sciErr = allocComplexMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &real, &imag);
                            }
                            else
                            {
                                sciErr = allocComplexMatrixOfDouble(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &real, &imag);
                            }
                            if (sciErr.iErr)
                            {
                                break;
                            }
                        }

                        elementR = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimR, &isCopy1);
                        isCopy1 = JNI_FALSE;
                        elementI = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimI, &isCopy1);

                        // Get the matrix element
                        if (swap)
                        {
                            for (k = 0; k < nbCol; k++)
                            {
                                real[k * nbRow + j] = elementR[k];
                                imag[k * nbRow + j] = elementI[k];
                            }
                        }
                        else
                        {
                            for (k = 0; k < nbCol; k++)
                            {
                                real[j * nbCol + k] = elementR[k];
                                imag[j * nbCol + k] = elementI[k];
                            }
                        }
                        (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimR, elementR, JNI_ABORT);
                        (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimI, elementI, JNI_ABORT);
                        (*jenv)->DeleteLocalRef(jenv, oneDimR);
                        (*jenv)->DeleteLocalRef(jenv, oneDimI);
                    }
                    (*jenv)->DeleteLocalRef(jenv, data);
                    (*jenv)->DeleteLocalRef(jenv, imagData);
                }
                (*jenv)->DeleteLocalRef(jenv, var);
                break;
            }
            case sci_handles :
            {
                int nbRow = (*jenv)->GetArrayLength(jenv, var);
                int nbCol = 0;
                int j = 0, k;
                long long * h = 0;

                if (parentList)
                {
                    sciErr.iErr = 0;
                    sciErr.iMsgCount = 0;
                    addErrorMessage(&sciErr, API_ERROR_CREATE_HANDLE, _("Unable to create handle in list"));
                    break;
                }

                if (!var || nbRow == 0)
                {
                    if (parentList)
                    {
                        sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                    }
                    else
                    {
                        sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                    }
                    if (!var)
                    {
                        (*jenv)->DeleteLocalRef(jenv, var);
                    }
                    break;
                }

                // Get the matrix rows
                for (; j < nbRow; j++)
                {
                    jboolean isCopy1 = JNI_FALSE;
                    jlong* element = NULL;
                    jlongArray oneDim = (jlongArray)(*jenv)->GetObjectArrayElement(jenv, var, j);
                    if (nbCol == 0)
                    {
                        nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                        sciErr = allocMatrixOfHandle(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &h);
                        if (sciErr.iErr)
                        {
                            break;
                        }
                    }

                    element = (jlong*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                    // Get the matrix element
                    if (swap)
                    {
                        for (k = 0; k < nbCol; k++)
                        {
                            h[k * nbRow + j] = element[k];
                        }
                    }
                    else
                    {
                        for (k = 0; k < nbCol; k++)
                        {
                            h[j * nbCol + k] = element[k];
                        }
                    }
                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                    (*jenv)->DeleteLocalRef(jenv, oneDim);
                }
                (*jenv)->DeleteLocalRef(jenv, var);
                break;
            }
            case sci_poly :
            {
                jobject data = (*jenv)->GetObjectArrayElement(jenv, var, 1);
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
                    if (parentList)
                    {
                        sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                    }
                    else
                    {
                        sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                    }
                    if (!data)
                    {
                        (*jenv)->DeleteLocalRef(jenv, data);
                    }
                    (*jenv)->DeleteLocalRef(jenv, var);
                    break;
                }

                isReal = (*jenv)->GetArrayLength(jenv, var) == 2;
                js = (jstring)(*jenv)->GetObjectArrayElement(jenv, var, 0);
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
                        if (swap)
                        {
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
                        }
                        else
                        {
                            for (k = 0; k < nbCol; k++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jdouble* element = NULL;
                                jdoubleArray oneDim = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, dblDim, k);
                                int len = (*jenv)->GetArrayLength(jenv, oneDim);
                                nbCoef[j * nbCol + k] = len;
                                real[j * nbCol + k] = (double*)MALLOC(sizeof(double) * len);
                                element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);
                                memcpy(real[j * nbCol + k], element, sizeof(double) * len);
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, 0);
                                (*jenv)->DeleteLocalRef(jenv, oneDim);
                            }
                        }
                        (*jenv)->DeleteLocalRef(jenv, dblDim);
                    }
                    (*jenv)->DeleteLocalRef(jenv, data);

                    if (parentList)
                    {
                        sciErr = createMatrixOfPolyInList(pvApiCtx, stackPos, parentList, listPos, polyVarName, swap ? nbRow : nbCol, swap ? nbCol : nbRow, nbCoef, (const double * const*)real);
                    }
                    else
                    {
                        sciErr = createMatrixOfPoly(pvApiCtx, stackPos, polyVarName, swap ? nbRow : nbCol, swap ? nbCol : nbRow, nbCoef, (const double * const*)real);
                    }

                    for (j = 0; j < nbRow * nbCol; j++)
                    {
                        FREE(real[j]);
                    }
                    FREE(real);
                    FREE(nbCoef);
                }
                else
                {
                    jobject imagData = (*jenv)->GetObjectArrayElement(jenv, var, 2);
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
                        if (swap)
                        {
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
                        }
                        else
                        {
                            for (k = 0; k < nbCol; k++)
                            {
                                jboolean isCopy1 = JNI_FALSE;
                                jdouble* element = NULL;
                                jdoubleArray oneDimR = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, dblDimR, k);
                                jdoubleArray oneDimI = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, dblDimI, k);
                                int len = (*jenv)->GetArrayLength(jenv, oneDimR);
                                nbCoef[j * nbCol + k] = len;
                                real[j * nbCol + k] = (double*)MALLOC(sizeof(double) * len);
                                imag[j * nbCol + k] = (double*)MALLOC(sizeof(double) * len);
                                element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimR, &isCopy1);
                                memcpy(real[j * nbCol + k], element, sizeof(double) * len);
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimR, element, 0);
                                (*jenv)->DeleteLocalRef(jenv, oneDimR);
                                isCopy1 = JNI_FALSE;
                                element = (jdouble*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDimI, &isCopy1);
                                memcpy(imag[j * nbCol + k], element, sizeof(double) * len);
                                (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDimI, element, 0);
                                (*jenv)->DeleteLocalRef(jenv, oneDimI);
                            }
                        }
                        (*jenv)->DeleteLocalRef(jenv, dblDimR);
                        (*jenv)->DeleteLocalRef(jenv, dblDimI);
                    }
                    (*jenv)->DeleteLocalRef(jenv, data);
                    (*jenv)->DeleteLocalRef(jenv, imagData);

                    if (parentList)
                    {
                        sciErr = createComplexMatrixOfPolyInList(pvApiCtx, stackPos, parentList, listPos, polyVarName, swap ? nbRow : nbCol, swap ? nbCol : nbRow, nbCoef, (const double * const*)real, (const double * const*)imag);
                    }
                    else
                    {
                        sciErr = createComplexMatrixOfPoly(pvApiCtx, stackPos, polyVarName, swap ? nbRow : nbCol, swap ? nbCol : nbRow, nbCoef, (const double * const*)real, (const double * const*)imag);
                    }

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
                (*jenv)->DeleteLocalRef(jenv, var);
                break;
            }
            case sci_boolean :
            {
                int nbRow = (*jenv)->GetArrayLength(jenv, var);
                int nbCol = 0;
                int j = 0, k;
                int * b = 0;

                if (!var || nbRow == 0)
                {
                    if (parentList)
                    {
                        sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                    }
                    else
                    {
                        sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                    }
                    if (!var)
                    {
                        (*jenv)->DeleteLocalRef(jenv, var);
                    }
                    break;
                }

                // Get the matrix rows
                for (; j < nbRow; j++)
                {
                    jboolean isCopy1 = JNI_FALSE;
                    jboolean* element = NULL;
                    jdoubleArray oneDim = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, var, j);
                    if (nbCol == 0)
                    {
                        nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                        if (parentList)
                        {
                            sciErr = allocMatrixOfBooleanInList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &b);
                        }
                        else
                        {
                            sciErr = allocMatrixOfBoolean(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &b);
                        }
                        if (sciErr.iErr)
                        {
                            break;
                        }
                    }

                    element = (jboolean*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                    // Get the matrix element
                    if (swap)
                    {
                        for (k = 0; k < nbCol; k++)
                        {
                            b[k * nbRow + j] = element[k];
                        }
                    }
                    else
                    {
                        for (k = 0; k < nbCol; k++)
                        {
                            b[j * nbCol + k] = element[k];
                        }
                    }
                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                    (*jenv)->DeleteLocalRef(jenv, oneDim);
                }
                (*jenv)->DeleteLocalRef(jenv, var);
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
                int isReal = (*jenv)->GetArrayLength(jenv, var) == 4;
                jintArray jdims = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 0);
                jintArray jnbItemRow = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 1);
                jintArray jcolPos = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 2);
                jint nbNonNull = (*jenv)->GetArrayLength(jenv, jcolPos);
                jdoubleArray jdata = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, var, 3);
                jdoubleArray jimagData;

                dims = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, jdims, &isCopy1);
                isCopy1 = JNI_FALSE;
                nbItemRow = (int*)(*jenv)->GetPrimitiveArrayCritical(jenv, jnbItemRow, &isCopy1);
                isCopy1 = JNI_FALSE;
                colPos = (int*)(*jenv)->GetPrimitiveArrayCritical(jenv, jcolPos, &isCopy1);
                isCopy1 = JNI_FALSE;
                data = (double*)(*jenv)->GetPrimitiveArrayCritical(jenv, jdata, &isCopy1);
                isCopy1 = JNI_FALSE;

                if (isReal)
                {
                    if (parentList)
                    {
                        sciErr = createSparseMatrixInList(pvApiCtx, stackPos, parentList, listPos, dims[0], dims[1], nbNonNull, nbItemRow, colPos, data);
                    }
                    else
                    {
                        sciErr = createSparseMatrix(pvApiCtx, stackPos, dims[0], dims[1], nbNonNull, nbItemRow, colPos, data);
                    }
                }
                else
                {
                    jimagData = (jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, var, 4);
                    imagData = (double*)(*jenv)->GetPrimitiveArrayCritical(jenv, jimagData, &isCopy1);
                    if (parentList)
                    {
                        sciErr = createComplexSparseMatrixInList(pvApiCtx, stackPos, parentList, listPos, dims[0], dims[1], nbNonNull, nbItemRow, colPos, data, imagData);
                    }
                    else
                    {
                        sciErr = createComplexSparseMatrix(pvApiCtx, stackPos, dims[0], dims[1], nbNonNull, nbItemRow, colPos, data, imagData);
                    }
                    (*jenv)->ReleasePrimitiveArrayCritical(jenv, jimagData, imagData, JNI_ABORT);
                    (*jenv)->DeleteLocalRef(jenv, jimagData);
                }
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jdata, data, JNI_ABORT);

                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jcolPos, colPos, JNI_ABORT);
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jnbItemRow, nbItemRow, JNI_ABORT);
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jdims, dims, JNI_ABORT);

                (*jenv)->DeleteLocalRef(jenv, jdata);
                (*jenv)->DeleteLocalRef(jenv, jcolPos);
                (*jenv)->DeleteLocalRef(jenv, jnbItemRow);
                (*jenv)->DeleteLocalRef(jenv, jdims);
                (*jenv)->DeleteLocalRef(jenv, var);

                break;
            }
            case sci_boolean_sparse :
            {
                jintArray jdims = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 0);
                jintArray jnbItemRow = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 1);
                jintArray jcolPos = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 2);
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

                if (parentList)
                {
                    sciErr = createBooleanSparseMatrixInList(pvApiCtx, stackPos, parentList, listPos, dims[0], dims[1], nbNonNull, nbItemRow, colPos);
                }
                else
                {
                    sciErr = createBooleanSparseMatrix(pvApiCtx, stackPos, dims[0], dims[1], nbNonNull, nbItemRow, colPos);
                }

                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jcolPos, colPos, JNI_ABORT);
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jnbItemRow, nbItemRow, JNI_ABORT);
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jdims, dims, JNI_ABORT);

                (*jenv)->DeleteLocalRef(jenv, jcolPos);
                (*jenv)->DeleteLocalRef(jenv, jnbItemRow);
                (*jenv)->DeleteLocalRef(jenv, jdims);
                (*jenv)->DeleteLocalRef(jenv, var);

                break;
            }
            case sci_ints :
            {
                jintArray jtype = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 0);
                jint * typep = (*jenv)->GetIntArrayElements(jenv, jtype, 0);
                int type = *typep;

                (*jenv)->ReleaseIntArrayElements(jenv, jtype, typep, JNI_ABORT);
                (*jenv)->DeleteLocalRef(jenv, jtype);

                switch (type)
                {
                    case sci_int8 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        char * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
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
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfInteger8InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfInteger8(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }
                            isCopy1 = JNI_FALSE;
                            element = (jbyte*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);
                        break;
                    }
                    case sci_uint8 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        unsigned char * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
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
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfUnsignedInteger8InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfUnsignedInteger8(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }

                            element = (jbyte*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);

                        break;
                    }
                    case sci_int16 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        short * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
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
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfInteger16InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfInteger16(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }

                            element = (jshort*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);

                        break;
                    }
                    case sci_uint16 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        unsigned short * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
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
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfUnsignedInteger16InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfUnsignedInteger16(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }
                            isCopy1 = JNI_FALSE;
                            element = (jshort*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);

                        break;
                    }
                    case sci_int32 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        int * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
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
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfInteger32InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfInteger32(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }
                            isCopy1 = JNI_FALSE;
                            element = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);

                        break;
                    }
                    case sci_uint32 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        unsigned int * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
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
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfUnsignedInteger32InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfUnsignedInteger32(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }
                            isCopy1 = JNI_FALSE;
                            element = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);

                        break;
                    }
#ifdef __SCILAB_INT64__
                    case sci_int64 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        long * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
                            if (!jdata)
                            {
                                (*jenv)->DeleteLocalRef(jenv, jdata);
                            }
                            break;
                        }

                        // Get the matrix rows
                        for (; j < nbRow; j++)
                        {
                            jlongArray oneDim = (jlongArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                            if (nbCol == 0)
                            {
                                nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfInteger32InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfInteger32(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }
                            jboolean isCopy1 = JNI_FALSE;
                            jlong* element = (jlong*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);

                        break;
                    }
                    case sci_uint64 :
                    {
                        jobject jdata = (*jenv)->GetObjectArrayElement(jenv, var, 1);
                        int nbRow = (*jenv)->GetArrayLength(jenv, jdata);
                        int nbCol = 0;
                        int j = 0, k;
                        unsigned long * data = 0;

                        if (!jdata || nbRow == 0)
                        {
                            if (parentList)
                            {
                                sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                            }
                            else
                            {
                                sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                            }
                            if (!jdata)
                            {
                                (*jenv)->DeleteLocalRef(jenv, jdata);
                            }
                            break;
                        }

                        // Get the matrix rows
                        for (; j < nbRow; j++)
                        {
                            jlongArray oneDim = (jlongArray)(*jenv)->GetObjectArrayElement(jenv, jdata, j);
                            if (nbCol == 0)
                            {
                                nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                                if (parentList)
                                {
                                    sciErr = allocMatrixOfUnsignedInteger64InList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                else
                                {
                                    sciErr = allocMatrixOfUnsignedInteger64(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, &data);
                                }
                                if (sciErr.iErr)
                                {
                                    break;
                                }
                            }
                            jboolean isCopy1 = JNI_FALSE;
                            jlong* element = (jlong*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy1);

                            // Get the matrix element
                            if (swap)
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[k * nbRow + j] = element[k];
                                }
                            }
                            else
                            {
                                for (k = 0; k < nbCol; k++)
                                {
                                    data[j * nbCol + k] = element[k];
                                }
                            }
                            (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
                            (*jenv)->DeleteLocalRef(jenv, oneDim);
                        }
                        (*jenv)->DeleteLocalRef(jenv, jdata);

                        break;
                    }
#endif
                }
                (*jenv)->DeleteLocalRef(jenv, var);
                break;
            }
            case sci_strings :
            {
                int nbRow = (*jenv)->GetArrayLength(jenv, var);
                int nbCol = 0;
                int j = 0, k;
                char ** strings = 0;

                if (!var || nbRow == 0)
                {
                    if (parentList)
                    {
                        sciErr = createMatrixOfDoubleInList(pvApiCtx, stackPos, parentList, listPos, 0, 0, 0);
                    }
                    else
                    {
                        sciErr = createMatrixOfDouble(pvApiCtx, stackPos, 0, 0, 0);
                    }
                    if (!var)
                    {
                        (*jenv)->DeleteLocalRef(jenv, var);
                    }
                    break;
                }

                // Get the matrix rows
                for (; j < nbRow; j++)
                {
                    jobjectArray oneDim = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, var, j);
                    if (nbCol == 0)
                    {
                        nbCol = (*jenv)->GetArrayLength(jenv, oneDim);
                        strings = (char**)MALLOC(sizeof(char*) * nbRow * nbCol);
                    }

                    // Get the matrix element
                    if (swap)
                    {
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
                    }
                    else
                    {
                        for (k = 0; k < nbCol; k++)
                        {
                            jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, oneDim, k);
                            jboolean isCopy1 = JNI_FALSE;
                            char *str = (char *)(*jenv)->GetStringUTFChars(jenv, j_string, &isCopy1);
                            strings[j * nbCol + k] = (char*)MALLOC(sizeof(char) * (strlen(str) + 1));
                            strcpy(strings[j * nbCol + k], str);
                            if (isCopy1)
                            {
                                (*jenv)->ReleaseStringUTFChars(jenv, j_string, (const char *)str);
                            }
                            (*jenv)->DeleteLocalRef(jenv, j_string);
                        }
                    }
                    (*jenv)->DeleteLocalRef(jenv, oneDim);
                }
                (*jenv)->DeleteLocalRef(jenv, var);

                if (parentList)
                {
                    sciErr = createMatrixOfStringInList(pvApiCtx, stackPos, parentList, listPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, (const char * const*)strings);
                }
                else
                {
                    sciErr = createMatrixOfString(pvApiCtx, stackPos, swap ? nbRow : nbCol, swap ? nbCol : nbRow, (const char * const*)strings);
                }

                for (j = 0; j < nbRow * nbCol; j++)
                {
                    FREE(strings[j]);
                }
                FREE(strings);
                break;
            }
            case sci_list :
            {
                jboolean isCopy1 = JNI_FALSE;
                int i = 1;
                int * list = NULL;
                int * types = NULL;
                jintArray jtypes = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 0);
                jint len = (*jenv)->GetArrayLength(jenv, jtypes);
                if (parentList)
                {
                    sciErr = createListInList(pvApiCtx, stackPos, parentList, listPos, (int)len, &list);
                }
                else
                {
                    sciErr = createList(pvApiCtx, stackPos, (int)len, &list);
                }
                if (sciErr.iErr)
                {
                    (*jenv)->DeleteLocalRef(jenv, jtypes);
                    break;
                }

                types = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, jtypes, &isCopy1);
                for (; i <= (int)len; i++)
                {
                    jobject jobj = (*jenv)->GetObjectArrayElement(jenv, var, i);
                    sciErr = sendVar(jenv, stackPos, list, i, jobj, types[i - 1], swap);
                    if (sciErr.iErr)
                    {
                        break;
                    }
                }
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jtypes, types, JNI_ABORT);
                (*jenv)->DeleteLocalRef(jenv, jtypes);
                (*jenv)->DeleteLocalRef(jenv, var);
                break;
            }
            case sci_tlist :
            {
                jboolean isCopy1 = JNI_FALSE;
                int i = 1;
                int * list = NULL;
                int * types = NULL;
                jintArray jtypes = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 0);
                jint len = (*jenv)->GetArrayLength(jenv, jtypes);
                if (parentList)
                {
                    sciErr = createTListInList(pvApiCtx, stackPos, parentList, listPos, (int)len, &list);
                }
                else
                {
                    sciErr = createTList(pvApiCtx, stackPos, (int)len, &list);
                }
                if (sciErr.iErr)
                {
                    (*jenv)->DeleteLocalRef(jenv, jtypes);
                    break;
                }

                types = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, jtypes, &isCopy1);
                for (; i <= (int)len; i++)
                {
                    jobject jobj = (*jenv)->GetObjectArrayElement(jenv, var, i);
                    sciErr = sendVar(jenv, stackPos, list, i, jobj, types[i - 1], swap);
                    if (sciErr.iErr)
                    {
                        break;
                    }
                }
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jtypes, types, JNI_ABORT);
                (*jenv)->DeleteLocalRef(jenv, jtypes);
                (*jenv)->DeleteLocalRef(jenv, var);
                break;
            }
            case sci_mlist :
            {
                jboolean isCopy1 = JNI_FALSE;
                int i = 1;
                int * list = NULL;
                int * types = NULL;
                jintArray jtypes = (jintArray)(*jenv)->GetObjectArrayElement(jenv, var, 0);
                jint len = (*jenv)->GetArrayLength(jenv, jtypes);
                if (parentList)
                {
                    sciErr = createMListInList(pvApiCtx, stackPos, parentList, listPos, (int)len, &list);
                }
                else
                {
                    sciErr = createMList(pvApiCtx, stackPos, (int)len, &list);
                }
                if (sciErr.iErr)
                {
                    (*jenv)->DeleteLocalRef(jenv, jtypes);
                    break;
                }

                types = (jint*)(*jenv)->GetPrimitiveArrayCritical(jenv, jtypes, &isCopy1);
                for (; i <= (int)len; i++)
                {
                    jobject jobj = (*jenv)->GetObjectArrayElement(jenv, var, i);
                    sciErr = sendVar(jenv, stackPos, list, i, jobj, types[i - 1], swap);
                    if (sciErr.iErr)
                    {
                        break;
                    }
                }
                (*jenv)->ReleasePrimitiveArrayCritical(jenv, jtypes, types, JNI_ABORT);
                (*jenv)->DeleteLocalRef(jenv, jtypes);
                (*jenv)->DeleteLocalRef(jenv, var);
                break;
            }
        }

        return sciErr;
    }

#ifdef __cplusplus
}
#endif
