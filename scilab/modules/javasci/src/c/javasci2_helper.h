/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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

#ifndef __JAVASCI2_HELPER_H__
#define __JAVASCI2_HELPER_H__

/**
 * This header describes a serie of helper functions.
 * This function should be only used when writing a wrapper of Scilab into an
 * other language (Java).
 * All these functions are based on api_scilab. They provide a simple profile
 * to be easily integrate into a SWIG wrapper.
 */

#ifndef ENABLE_HELPERS
#error "This file cannot be used directly"
#endif
#include "BOOL.h"
#include "sci_types.h"

typedef unsigned char byte;

/**
 * If the variable is complex or not
 *
 * @param variableName The name of the variable
 * @return TRUE is complex, FALSE otherwise
 */
BOOL isComplex(char *variableName);

/**
 * Return the precision of an integer
 *
 * @param variableName The name of tbe variable
 * @return the type of the integer (see sci_int_types)
 */
sci_int_types getIntegerPrecision(char* variableName);


/**
 * Return a matrix of double (the default type in Scilab)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of double (column sorted)
 */
double * getDouble(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of double (the default type in Scilab)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putDouble(char* variableName, double *variable, int nbRow, int nbCol);

/**
 * Return the real part of a matrix of complex double
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The real part of a matrix of complex double (column sorted)
 */
double * getDoubleComplexReal(char* variableName, int *nbRow, int *nbCol);

/**
 * Return the imaginary part of a matrix of complex double
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The imaginary part of a matrix of complex double (column sorted)
 */
double * getDoubleComplexImg(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of complex double
 *
 * @param variableName The name of the variable
 * @param variable The complex values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putDoubleComplex(char* variableName, double *variable, int nbRow, int nbCol, double * imag, int nbRowI, int nbColI);

/**
 * Return a matrix of boolean
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of boolean (column sorted)
 */
BOOL * getBoolean(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of boolean
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putBoolean(char* variableName, BOOL *variable, int nbRow, int nbCol);

/**
 * Return a matrix of byte (int8)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of byte (column sorted)
 */
byte * getByte(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of byte (int8)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putByte(char* variableName, byte *variable, int nbRow, int nbCol);

/**
 * Return a matrix of unsigned byte (uint8)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of unsigned byte (column sorted)
 */
byte * getUnsignedByte(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of unsigned byte (uint8)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putUnsignedByte(char* variableName, byte *variable, int nbRow, int nbCol);

/**
 * Return a matrix of short (int16)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of short (column sorted)
 */
short * getShort(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of short (int16)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putShort(char* variableName, short *variable, int nbRow, int nbCol);

/**
 * Return a matrix of unsigned short (uint16)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of unsigned short (column sorted)
 */
unsigned short * getUnsignedShort(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of unsigned short (uint16)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putUnsignedShort(char* variableName, unsigned short *variable, int nbRow, int nbCol);

/**
 * Return a matrix of integer (int32)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of integer (column sorted)
 */
int * getInt(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of integer (int32)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putInt(char* variableName, int *variable, int nbRow, int nbCol);

/**
 * Return a matrix of unsigned integer (uint32)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of unsigned integer (column sorted)
 */
unsigned int * getUnsignedInt(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of unsigned integer (uint32)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putUnsignedInt(char* variableName, unsigned int *variable, int nbRow, int nbCol);

/**
 * Return a matrix of long (int64)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of long (column sorted)
 */
long long * getLong(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of long (int64)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putLong(char* variableName, long *variable, int nbRow, int nbCol);

/**
 * Return a matrix of unsigned long (uint64)
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of unsigned long (column sorted)
 */
unsigned long * getUnsignedLong(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of unsigned long (int64)
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putUnsignedLong(char* variableName, unsigned long *variable, int nbRow, int nbCol);

/**
 * Return a matrix of string
 *
 * @param variableName The name of the variable
 * @param[out] nbRow Number of rows
 * @param[out] nbCol Number of cols
 * @return The matrix of string (column sorted)
 */
char ** getString(char* variableName, int *nbRow, int *nbCol);

/**
 * Set a matrix of string
 *
 * @param variableName The name of the variable
 * @param variable The values
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @return 0 if successfull, != 0 otherwise
 */
int putString(char* variableName, char **variable, int nbRow, int nbCol);

/**
 * Set a sparse matrix
 *
 * @param variableName The name of the variable
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @param nbRowItem the number of non null items by row
 * @param nbRowItemL the length of the previous array
 * @param colPos the column position of each non null item
 * @param colPosL the length of the previous array
 * @param data the double data
 * @param dataL the length of the previous array
 * @return 0 if successfull, != 0 otherwise
 */
int putSparse(char * variableName, int nbRow, int nbCol, int * nbRowItem, int nbRowItemL, int * colPos, int colPosL, double * data, int dataL);

/**
 * Set a complex sparse matrix
 *
 * @param variableName The name of the variable
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @param nbRowItem the number of non null items by row
 * @param nbRowItemL the length of the previous array
 * @param colPos the column position of each non null item
 * @param colPosL the length of the previous array
 * @param data the double data
 * @param dataL the length of the previous array
 * @param imag the double data
 * @param imagL the length of the previous array
 * @return 0 if successfull, != 0 otherwise
 */
int putComplexSparse(char * variableName, int nbRow, int nbCol, int * nbRowItem, int nbRowItemL, int * colPos, int colPosL, double * data, int dataL, double * imag, int imagL);

/**
 * Set a boolean sparse matrix
 *
 * @param variableName The name of the variable
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @param nbRowItem the number of non null items by row
 * @param nbRowItemL the length of the previous array
 * @param colPos the column position of each non null item
 * @param colPosL the length of the previous array
 * @return 0 if successfull, != 0 otherwise
 */
int putBooleanSparse(char * variableName, int nbRow, int nbCol, int * nbRowItem, int nbRowItemL, int * colPos, int colPosL);

/**
 * Set a polynomial matrix
 *
 * @param variableName The name of the variable
 * @param polyVarName The name of the polynomial variable
 * @param data the double data
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @param nbCoef the number of coef of each polynomial
 * @return 0 if successfull, != 0 otherwise
 */
int putPolynomial(char * variableName, char * polyVarName, double ** data, int nbRow, int nbCol, int * nbCoef);

/**
 * Set a complex polynomial matrix
 *
 * @param variableName The name of the variable
 * @param polyVarName The name of the polynomial variable
 * @param data the double data
 * @param nbRow Number of rows
 * @param nbCol Number of cols
 * @param imag the double data
 * @param nbRowI Number of rows
 * @param nbColI Number of cols
 * @param nbCoef the number of coef of each polynomial
 * @return 0 if successfull, != 0 otherwise
 */
int putComplexPolynomial(char * variableName, char * polyVarName, double ** data, int nbRow, int nbCol, int * nbCoef, double ** imag, int nbRowI, int nbColI, int * nbCoefI);

/**
 * Check if a variable exists in Context
 *
 * @param variableName The name of the variable
 * @return TRUE if the given variable exists, FALSE otherwise
 */
BOOL isExistingVariable(char* variableName);

/**
 * Check if a variable is complex.
 *
 * @param variableName The name of the variable
 * @return TRUE if the given variable is complex, FALSE otherwise
 */
BOOL isComplexVar(char *variableName);
#endif /* __JAVASCI2_HELPER_H__ */
