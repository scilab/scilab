/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#ifndef AST_VISITOR_COMMON_HXX
#define AST_VISITOR_COMMON_HXX


#define ENABLE_EXTERNAL_TYPER

#include "internal.hxx"
#include "struct.hxx"
#include "list.hxx"
#include "exp.hxx"
#include "symbol.hxx"
#include <list>
#include "expHistory.hxx"
#include "dynlib_ast.h"
#define bsiz	4096

//void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList);

types::InternalType* AddElementToVariable(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols);

types::InternalType* AddElementToVariableFromCol(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols, int *_piCols);

types::InternalType* AddElementToVariableFromRow(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols, int *_piRows);

const std::wstring* getStructNameFromExp(const ast::Exp* _pExp);

bool getFieldsFromExp(ast::Exp* _pExp, std::list<ExpHistory*>& fields);

types::InternalType* evaluateFields(const ast::Exp* _pExp, std::list<ExpHistory*>& fields, types::InternalType* pIT);

types::InternalType* callOverload(const ast::Exp& e,
                                  const std::wstring& _strType,
                                  types::typed_list* _pArgs,
                                  types::InternalType* _source,
                                  types::InternalType* _dest);

types::InternalType* callOverload(const std::wstring& strType, types::InternalType* _paramL, types::InternalType* _paramR);

types::InternalType* insertionCall(const ast::Exp& e, types::typed_list* _pArgs, types::InternalType* _pVar, types::InternalType* _pInsert);

EXTERN_AST void callOnPrompt(void);
EXTERN_AST ast::Exp* callTyper(ast::Exp* _tree, std::wstring _msg = std::wstring(L""));

void printLine(const std::string& _stPrompt, const std::string& _stLine, bool _bLF);
std::string printExp(std::ifstream& _File, ast::Exp* _pExp, const std::string& _stPrompt, int* _piLine /* in/out */, int* _piCol /* in/out */, std::string& _stPreviousBuffer);


#endif //!AST_VISITOR_COMMON_HXX
