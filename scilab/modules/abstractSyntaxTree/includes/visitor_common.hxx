/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef AST_VISITOR_COMMON_HXX
#define AST_VISITOR_COMMON_HXX

#include "internal.hxx"
#include "exp.hxx"

#define bsiz	4096
EXTERN_AST bool bConditionState(types::InternalType *_pITResult);

EXTERN_AST void ExpandList(size_t ** _piList, size_t *_piListSize, size_t _iListSizeSize, size_t *_piResultList);
EXTERN_AST int GetVarMaxDim(types::InternalType *_pIT, size_t _iCurrentDim, size_t _iMaxDim);

EXTERN_AST types::InternalType* AddElementToVariable(
						types::InternalType* _poDest,
						types::InternalType* _poSource,
						size_t _iRows, size_t _iCols, size_t *_piRows, size_t *_piCols);

EXTERN_AST types::InternalType* AddElementToVariableFromCol(
                        types::InternalType* _poDest,
                        types::InternalType* _poSource,
                        size_t _iRows, size_t _iCols, size_t *_piCols);

EXTERN_AST types::InternalType* AddElementToVariableFromRow(
                        types::InternalType* _poDest,
                        types::InternalType* _poSource,
                        size_t _iRows, size_t _iCols, size_t *_piRows);


EXTERN_AST const std::wstring* getStructNameFromExp(const ast::Exp* _pExp);

EXTERN_AST types::Struct* getStructFromExp(const ast::Exp* _pExp);

#endif //!AST_VISITOR_COMMON_HXX
