/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "api_scilab.h"

enum type_check
{
    CHECK_NONE,
    CHECK_SIZE,
    CHECK_MIN,
    CHECK_MAX,
    CHECK_BOTH,
    CHECK_VALUES
};

/**
 * Get a pointer to an existing plist
 * @param[in] _iVar position on the stack
 * @param[out] _piAddress variable address
 * @return a SciErr structure
 */
SciErr initPList(void* _pvCtx, int _iVar, int ** _piAddress);

/**
 * Check that a pointer points to an existing plist
 * @param[in] _piAddress variable address
 * @return !0 if it's a plist 0 otherwise
 */
int checkPList(void* _pvCtx, int * _piAddress);

/**
 * Check that a label exists in a plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabel the label to be checked
 * @return the position of the label in the plist -1 otherwise
 */
int hasLabelInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel);

/**
 * Check that a label is partially matched in a plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabel the label to be checked
 * @return the position of the label in the plist -1 otherwise
 */
int hasPartialLabelInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel);

/**
 * Get a C int associated to a label from a plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabel the label to be checked
 * @param[out] _piValue the value stored in the plist
 * @param[out] _piFound !-1 if the label has been found -1 otherwise
 * @param[in] _iDefaultValue the value returned in '_piValue' if the label has not been found
 * @param[in] _iLog display information messages during type_check
 * @param[in] check a type of checking in the value returned in '_piValue'
 * @return a SciErr structure
 */
SciErr getIntInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, int * _piValue, int * _piFound,
                     int _iDefaultValue, int _iLog, enum type_check check, ...);

/**
 * Get a C double associated to a label from a plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabel the label to be checked
 * @param[out] _pdblValue the value stored in the plist
 * @param[out] _piFound !-1 if the label has been found -1 otherwise
 * @param[in] _dblDefaultValue the value returned in '_pdblValue' if the label has not been found
 * @param[in] _iLog display information messages during type_check
 * @param[in] check a type of checking in the value returned in '_pdblValue'
 * @return a SciErr structure
 */
SciErr getDoubleInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, double * _pdblValue, int * _piFound,
                        double _dblDefaultValue, int _iLog, enum type_check check, ...);

/**
 * Get a C string associated to a label from a plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabel the label to be checked
 * @param[out] _pstValue the value stored in the plist
 * @param[out] _piFound !-1 if the label has been found -1 otherwise
 * @param[in] _pstDefaultValue the value returned in '_pstValue' if the label has not been found
 * @param[in] _iLog display information messages during type_check
 * @param[in] check a type of checking in the value returned in '_pstValue'
 * @return a SciErr structure
 */
SciErr getStringInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, char ** _pstValue, int * _piFound,
                        const char * _pstDefaultValue, int _iLog, enum type_check check, ...);

/**
 * Get a C column vector of int associated to a label from a plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabel the label to be checked
 * @param[out] _piValue the value stored in the plist
 * @param[out] _piFound !-1 if the label has been found -1 otherwise
 * @param[in] _iDefaultValue the value returned in '_piValue' if the label has not been found
 * @param[in] _iDefaultSize the size of the vector of int that must be returned
 * @param[out] _piSize the size of the vector of int that has been read
 * @param[in] _iLog display information messages during type_check
 * @param[in] check a type of checking in the value returned in '_piValue'
 * @return a SciErr structure
 */
SciErr getColVectorOfIntInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, int * _piValue, int * _piFound,
                                int _iDefaultValue, int _iDefaultSize, int * _piSize, int _iLog, enum type_check check, ...);

/**
 * Get a C column vector of double associated to a label from a plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabel the label to be checked
 * @param[out] _pdblValue the value stored in the plist
 * @param[out] _piFound !-1 if the label has been found -1 otherwise
 * @param[in] _dblDefaultValue the value returned in '_pdblValue' if the label has not been found
 * @param[in] _iDefaultSize the size of the vector of int that must be returned
 * @param[out] _piSize the size of the vector of int that has been read
 * @param[in] _iLog display information messages during type_check
 * @param[in] check a type of checking in the value returned in '_pdblValue'
 * @return a SciErr structure
 */
SciErr getColVectorOfDoubleInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, double * _pdblValue, int * _piFound,
                                   double _dblDefaultValue, int _iDefaultSize, int * _piSize, int _iLog, enum type_check check, ...);

/**
 * Create a plist
 * @param[in] _iVar the position on the stack of the plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabelNames a vector of labels for the plist
 * @param[in] _iNbParams the number of labels
 * @return a SciErr structure
 */
SciErr createPList(void* _pvCtx, int _iVar, int ** _piAddress, char ** _pstLabelNames, int _iNbParams);

/**
 * Store a C int value in a plist and associates it to the given label
 * @param[in] _iVar the position on the stack of the plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabelName the label to which the value must be associated
 * @param[in] _iValue the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createIntInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, int _iValue);

/**
 * Store a C double value in a plist and associates it to the given label
 * @param[in] _iVar the position on the stack of the plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabelName the label to which the value must be associated
 * @param[in] _dblValue the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createDoubleInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, double _dblValue);

/**
 * Store a C string value in a plist and associates it to the given label
 * @param[in] _iVar the position on the stack of the plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabelName the label to which the value must be associated
 * @param[in] _pstValue the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createStringInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, char * _pstValue);

/**
 * Store a C column vector of int values in a plist and associates it to the given label
 * @param[in] _iVar the position on the stack of the plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabelName the label to which the value must be associated
 * @param[in] _iNbValues the size of the vector to be stored in the plist
 * @param[in] _piValue the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createColVectorOfIntInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, int _iNbValues, int * _piValue);

/**
 * Store a C column vector of double values in a plist and associates it to the given label
 * @param[in] _iVar the position on the stack of the plist
 * @param[in] _piAddress variable address
 * @param[in] _pstLabelName the label to which the value must be associated
 * @param[in] _iNbValues the size of the vector to be stored in the plist
 * @param[in] _pdblValue the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createColVectorOfDoubleInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, int _iNbValues, double * _pdblValue);

#endif
