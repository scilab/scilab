/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __TYPES_TOOLS_HXX__
#define __TYPES_TOOLS_HXX__

#include "types.hxx"
#include "function.hxx"

namespace types
{
bool getScalarIndex(GenericType* _pRef, typed_list* _pArgsIn, int* index);
bool getImplicitIndex(GenericType* _pRef, typed_list* _pArgsIn, std::vector<int>& index, std::vector<int>& dims);
bool getScalarImplicitIndex(GenericType* _pRef, typed_list* _pArgsIn, std::vector<double>& index);

EXTERN_AST int checkIndexesArguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim);
EXTERN_AST void cleanIndexesArguments(typed_list* _pArgsOrig, typed_list* _pArgsNew);
EXTERN_AST void getIndexesWithDims(int _iIndex, int* _piIndexes, int* _piDims, int _iDims);
EXTERN_AST int getIndexWithDims(int* _piIndexes, int* _piDims, int _iDims);
EXTERN_AST types::Function::ReturnValue VariableToString(types::InternalType* pIT, const wchar_t* wcsVarName);
}

#endif /* !__TYPES_TOOLS_HXX__ */
