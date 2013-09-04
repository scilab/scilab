/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#ifndef __HYPERMAT_API__
#define __HYPERMAT_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_hypermat.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "doublecomplex.h"

    /**************************************/
    /*   Hypermatrices matrix functions   */
    /**************************************/

    /**
     * Check if the variable type is double
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isHypermatType(void* _pvCtx, int* _piAddress);

    /**
     * Check if the variable is complex
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isHypermatComplex(void* _pvCtx, int* _piAddress);

    /**
     * Get the base type
     * @param[in] _piAddress variable address
     * @param[out] _piType return the base type
     */
    SciErr getHypermatType(void *_pvCtx, int *_piAddress, int *_piType);

    /**
     * Get integer precision ( SCI_xINTx )
     * @param[in] _piAddress variable address
     * @param[out] _piPrecison return integer precision ( SCI_xINTx )
     */
    SciErr getHypermatOfIntegerPrecision(void *_pvCtx, int *_piAddress, int *_piPrecision);

    /**
     * Get the hypermatrix dimensions
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     */
    SciErr getHypermatDimensions(void *_pvCtx, int *_piAddress, int **_dims, int *_ndims);

    /**
     * Get the hypermatrix entries address
     * @param[in] _piAddress variable address
     * @param[out] return _piEntriesAddress entries address
     */
    SciErr getHypermatEntries(void* _pvCtx, int* _piAddress, int ** _piEntriesAddress);

    /**
     * Get polynomial variable name
     * @param[in] _piAddress variable address
     * @param[out] _pstVarName return variable name of polynomials
     * @param[out] _piVarNameLen return length of _pstVarName
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getHypermatPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return real coefficients
     */
    SciErr getHypermatOfPoly(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piNbCoef, double** _pdblReal);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return real coefficients
     * @param[out] _pdblImg return imaginary coefficients
     */
    SciErr getComplexHypermatOfPoly(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _piLength return strings lengths
     * @param[out] _pstStrings return the strings data
     */
    SciErr getHypermatOfString(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piLength, char** _pstStrings);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _piLength return strings lengths
     * @param[out] _pwstStrings return the strings data
     */
    SciErr getHypermatOfWideString(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piLength, wchar_t** _pwstStrings);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _pdblReal return the double data
     */
    SciErr getHypermatOfDouble(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, double** _pdblReal);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _pdblReal return the real data
     * @param[out] _pdblImg return the imaginary data
     */
    SciErr getComplexHypermatOfDouble(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, double** _pdblReal, double** _pdblImg);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _pucData8 return the unsigned char data
     */
    SciErr getHypermatOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned char** _pucData8);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _pcData8 return the char data
     */
    SciErr getHypermatOfInteger8(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, char** _pcData8);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _pusData16 return the unsigned short data
     */
    SciErr getHypermatOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned short** _pusData16);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _psData16 return the short data
     */
    SciErr getHypermatOfInteger16(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, short** _psData16);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _puiData32 return the unsigned int data
     */
    SciErr getHypermatOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned int** _puiData32);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _piData32 return the int data
     */
    SciErr getHypermatOfInteger32(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int** _piData32);

    /**
     * Get the hypermatrix dimensions and data
     * @param[in] _piAddress variable address
     * @param[out] _dims return the dimensions
     * @param[out] _ndims return the number of dimensions
     * @param[out] _piBool return the boolean data
     */
    SciErr getHypermatOfBoolean(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int** _piBool);

    /**
     * Create a polynomial hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int *_dims, int _ndims, const int* _piNbCoef, const double* const* _pdblReal);

    /**
     * Create a complex polynomial hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real coefficients
     * @param[in] _pdblImg pointer to imaginary coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexHypermatOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int *_dims, int _ndims, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);

    /**
     * Create a double hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfDouble(void* _pvCtx, int _iVar, int *_dims, int _ndims, const double* _pdblReal);
    SciErr allocHypermatOfDouble(void *_pvCtx, int _iVar, int * _dims, int _ndims, double** _pdblReal);

    /**
     * Create a complex hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexHypermatOfDouble(void* _pvCtx, int _iVar, int *_dims, int _ndims, const double* _pdblReal, const double* _pdblImg);

    /**
     * Create a boolean hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _piBool pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfBoolean(void* _pvCtx, int _iVar, int *_dims, int _ndims, const int* _piBool);

    /**
     * Create a string hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _pstStrings pointer to string data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfString(void* _pvCtx, int _iVar, int *_dims, int _ndims, const char* const* _pstStrings);

    /**
     * Create a int8 hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _pcData8 pointer to char data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfInteger8(void* _pvCtx, int _iVar, int *_dims, int _ndims, const char* _pcData8);

    /**
     * Create a uint8 hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _pucData8 pointer to unsigned char data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfUnsignedInteger8(void* _pvCtx, int _iVar, int *_dims, int _ndims, const unsigned char* _pucData8);

    /**
     * Create a int16 hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _psData16 pointer to short data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfInteger16(void* _pvCtx, int _iVar, int *_dims, int _ndims, const short* _psData16);

    /**
     * Create a uint16 hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _pusData16 pointer to unsigned short data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfUnsignedInteger16(void* _pvCtx, int _iVar, int *_dims, int _ndims, const unsigned short* _pusData16);

    /**
     * Create a int32 hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _piData32 pointer to int data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfInteger32(void* _pvCtx, int _iVar, int *_dims, int _ndims, const int* _piData32);

    /**
     * Create a uint32 hypermatrix
     * @param[in] _iVar variable number
     * @param[in] _dims the dimensions
     * @param[in] _ndims the number of dimensions
     * @param[in] _puiData32 pointer to unsigned int data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createHypermatOfUnsignedInteger32(void* _pvCtx, int _iVar, int *_dims, int _ndims, const unsigned int* _puiData32);

#ifdef __cplusplus
}
#endif
#endif /* __HYPERMAT_API__ */
