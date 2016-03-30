/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#ifndef __TYPES_TOOLS_HXX__
#define __TYPES_TOOLS_HXX__

#include "types.hxx"
#include "function.hxx"
#include "double.hxx"

namespace types
{
bool getScalarIndex(GenericType* _pRef, typed_list* _pArgsIn, int* index);
bool getImplicitIndex(GenericType* _pRef, typed_list* _pArgsIn, std::vector<int>& index, std::vector<int>& dims);
bool getScalarImplicitIndex(GenericType* _pRef, typed_list* _pArgsIn, std::vector<double>& index);

EXTERN_AST int checkIndexesArguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim);
EXTERN_AST void cleanIndexesArguments(typed_list* _pArgsOrig, typed_list* _pArgsNew);
EXTERN_AST void getIndexesWithDims(int _iIndex, int* _piIndexes, const int* _piDims, int _iDims);
EXTERN_AST int getIndexWithDims(int* _piIndexes, const int* _piDims, int _iDims);
EXTERN_AST types::Function::ReturnValue VariableToString(types::InternalType* pIT, const wchar_t* wcsVarName);

//commom function
EXTERN_AST int computeTuples(int* _piCountDim, int _iDims, int _iCurrentDim, int* _piIndex);
EXTERN_AST Double* createEmptyDouble();
EXTERN_AST Double* createDoubleVector(int _iSize);
EXTERN_AST int getIntValueFromDouble(InternalType* _pIT, int _iPos);
EXTERN_AST double* getDoubleArrayFromDouble(InternalType* _pIT);
EXTERN_AST bool checkArgValidity(typed_list& _pArg);
}

#endif /* !__TYPES_TOOLS_HXX__ */
