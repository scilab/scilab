/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SCILABTOJAVA_HXX__
#define __SCILABTOJAVA_HXX__

#include <string>
#include <vector>

#include "../src/jni/ScilabVariables.hxx"
#include "../src/jni/ScilabVariablesRefresh.hxx"
#include "../src/jni/ScilabPolynomialToJava.hxx"

extern "C"
{
#include "Scierror.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "GetScilabVariable.h"
}

#ifndef GIWSEXPORT
# if defined(_MSC_VER) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define GIWSEXPORT
#   else
#     define GIWSEXPORT __declspec(dllexport)
#   endif
# else
#   if __GNUC__ >= 4
#     define GIWSEXPORT __attribute__ ((visibility ("default")))
#   else
#     define GIWSEXPORT
#   endif
# endif
#endif

using namespace org_scilab_modules_types;

namespace org_modules_types
{

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to send Scilab datas in Java environment
 */
class ScilabToJava
{

    static int refreshId;

    /**
     * Get the pointer on the indexes
     * @param indexes a vector containing indexes
     * @return the pointer
     */
    static int * getIndexesPointer(std::vector<int> & indexes);

    /**
     * Get a double dimension matrix from a one dimension one
     * The result must be delete in using deleteMatrix
     *
     * @param T the data type
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @return the double dimension matrix
     */
    template<typename T>
    static T ** getMatrix(const int row, const int col, T * data, const bool swaped);

    /**
     * Get a double dimension matrix from a one dimension one
     * The result must be delete in using deleteMatrix
     * Each data element is casted from T to U
     *
     * @param T the in data type
     * @param U the out data type
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @return the double dimension matrix
     */
    template<typename T, typename U>
    static T ** getConvertedMatrix(const int row, const int col, U * data, const bool swaped);

    /**
     * Convert a single row matrix into a double dimension one
     *
     * @param row the row number
     * @param col the col number
     * @return the double dimension matrix
     */
    template<typename T>
    static T ** convertMatrix(const int row, const int col, T * data);

    /**
     * Delete a matrix previously created with getMatrix or getConvertedMatrix
     * or convertMatrix
     *
     * @param T the in data type
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     */
    template<typename T>
    static void deleteMatrix(T ** data, const bool swaped);

public :

    /**
     * Send the list items
     *
     * @param name the variable name
     * @param nbItems the number of items
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param addr the address of the variable in the Scilab stack
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     * @param pvApiCtx a pointer to the context
     */
    GIWSEXPORT static bool sendItems(const std::string & name, const int nbItems, std::vector<int> & indexes, int * addr, const bool swaped, const bool byref, const int handlerId, void * pvApiCtx);

    /**
     * Send a variable in the Java environment
     *
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param addr the address of the variable in the Scilab stack
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     * @param pvApiCtx a pointer to the context
     */
    GIWSEXPORT static bool sendVariable(const std::string & name, std::vector<int> & indexes, int * addr, const bool swaped, const bool byref, const int handlerId, void * pvApiCtx);

    /**
     * Send a list or a tlist or mlist in the Java environment
     *
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param type a char with the value 'l' or 'm' or 't'
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    GIWSEXPORT static void sendVariable(const std::string & name, const int nbItems, std::vector<int> & indexes, const char type, const bool byref, const int handlerId);

    /**
     * Close a list or a tlist or mlist in the Java environment
     *
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param handlerId the handler id
     */
    GIWSEXPORT static void closeList(std::vector<int> & indexes, const int handlerId);

    /**
     * Send double sparse matrix
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param nbItem the number of non null elements
     * @param nbItemRow the number by row of non null elements
     * @param colPos the column position of the non null elements
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    template<typename T>
    GIWSEXPORT static void sendVariable(const std::string & name, std::vector<int> & indexes, const int nbItem, int * nbItemRow, int * colPos, const int row, const int col, T * data, const bool byref, const int handlerId);

    /**
     * Send double, int* and uint64 matrices
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    template<typename T>
    GIWSEXPORT static void sendVariable(const std::string & name, std::vector<int> & indexes, const int row, const int col, T * data, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send String matrices
     *
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    GIWSEXPORT static void sendStringVariable(const std::string & name, std::vector<int> & indexes, const int row, const int col, char ** data, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send boolean sparse matrix
     *
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param nbItem the number of true elements
     * @param nbItemRow the number by row of true elements
     * @param colPos the column position of the true elements
     * @param row the row number
     * @param col the col number
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    GIWSEXPORT static void sendBooleanSparseVariable(const std::string & name, std::vector<int> & indexes, const int nbItem, int * nbItemRow, int * colPos, const int row, const int col, const bool byref, const int handlerId);

    /**
     * Send uint* matrices (datas are converted to have the good Java type)
     *
     * @param T the type of the data
     * @param U the type of the converted data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @param handlerId the handler id
     */
    template<typename T, typename U>
    GIWSEXPORT static void sendUnsignedVariableWithCast(const std::string & name, std::vector<int> & indexes, const int row, const int col, U * data, const bool swaped, const int handlerId);

    /**
     * Send uint* matrices
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    template<typename T>
    GIWSEXPORT static void sendUnsignedVariable(const std::string & name, std::vector<int> & indexes, const int row, const int col, T * data, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send boolean matrix (data are converted from int to bool)
     *
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    GIWSEXPORT static void sendConvertedBooleanVariable(const std::string & name, std::vector<int> & indexes, const int row, const int col, int * data, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send complex sparse matrix
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param nbItem the number of non null elements
     * @param nbItemRow the number by row of non null elements
     * @param colPos the column position of the non null elements
     * @param row the row number
     * @param col the col number
     * @param real the real data
     * @param img the imaginary data
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    template<typename T>
    GIWSEXPORT static void sendVariable(const std::string & name, std::vector<int> & indexes, const int nbItem, int * nbItemRow, int * colPos, const int row, const int col, T * real, T * img, const bool byref, const int handlerId);

    /**
     * Send complex matrices
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param real the real data
     * @param img the imaginary data
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    template<typename T>
    GIWSEXPORT static void sendVariable(const std::string & name, std::vector<int> & indexes, const int row, const int col, T * real, T * img, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send double polynomial matrices
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param nbcoeff the number of coefficient of each polynomial
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    template<typename T>
    GIWSEXPORT static void sendVariable(const std::string & name, std::vector<int> & indexes, const char * varName, const int row, const int col, int * nbcoeff, T ** data, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send complex polynomial matrices
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param nbcoeff the number of coefficient of each polynomial
     * @param real the real data
     * @param img the imaginary data
     * @param swaped true if the matrix is stored row by row
     * @param byref true if the variable is passed by reference
     * @param handlerId the handler id
     */
    template<typename T>
    GIWSEXPORT static void sendVariable(const std::string & name, std::vector<int> & indexes, const char * varName, const int row, const int col, int * nbcoeff, T ** real, T ** img, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send handle matrices
     *
     * @param T the type of the data
     * @param name the variable name
     * @param indexes an integer array with the indexes of the (sub)*-list which will contain the data
     * @param row the row number
     * @param col the col number
     * @param data the data
     * @param swaped true if the matrix is stored row by row
     * @param handlerId the handler id
     */
    GIWSEXPORT static void sendHandleVariable(const std::string & name, std::vector<int> & indexes, const int row, const int col, long long * data, const bool swaped, const bool byref, const int handlerId);

    /**
     * Send all the listened variables
     */
    GIWSEXPORT static void sendAllListenedVariables();

    /**
     * Send a Scilab variable to the Java environment
     *
     * @param name the variable name
     * @param swaped true if the matrix is stored row by row
     * @param handlerId the handler id
     * @return true if all was ok
     */
    GIWSEXPORT static bool sendVariable(const std::string & name, const bool swaped, const int handlerId);

    /**
     * Send a Scilab variable to the Java environment as a reference when it is possible
     *
     * @param name the variable name
     * @param handlerId the handler id
     * @return true if all was ok
     */
    GIWSEXPORT static bool sendVariableAsReference(const std::string & name, const int handlerId);

    /**
     * Send a Scilab variable to the Java environment
     *
     * @param name the variable name
     * @param addr the variable address in the Scilab stack
     * @param swaped true if the matrix is stored row by row
     * @param handlerId the handler id
     * @param pvApiCtx a pointer to the context
     * @return true if all was ok
     */
    GIWSEXPORT static bool sendVariable(const std::string & name, int * addr, const bool swaped, const int handlerId, void * pvApiCtx);
};
}

#endif
