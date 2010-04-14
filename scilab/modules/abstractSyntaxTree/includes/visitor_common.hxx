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

#include "conditionvisitor.hxx"

#define bsiz	4096
bool bConditionState(ast::ConditionVisitor *exec);

void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList);
int GetVarMaxDim(types::InternalType *_pIT, int _iCurrentDim, int _iMaxDim);

types::InternalType* AddElementToVariable(
						types::InternalType* _poDest, 
						types::InternalType* _poSource, 
						int _iRows, int _iCols, int *_piRows, int *_piCols);

types::InternalType* AddElementToVariableFromCol(
                        types::InternalType* _poDest, 
                        types::InternalType* _poSource, 
                        int _iRows, int _iCols, int *_piCols);

types::InternalType* AddElementToVariableFromRow(
                        types::InternalType* _poDest, 
                        types::InternalType* _poSource, 
                        int _iRows, int _iCols, int *_piRows);

#endif //!AST_VISITOR_COMMON_HXX