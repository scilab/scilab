/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __ARGUMENTS_HXX__
#define __ARGUMENTS_HXX__

#include <map>
#include <functional>
#include "UTF8.hxx"
#include "function.hxx"
#include "variables.hxx"

#include "dynlib_ast.h"

struct ARG_VALIDATOR
{
    std::function<int(std::vector<types::InternalType*>&)> validator;
    std::vector<std::tuple<int, types::InternalType*>> inputs;
    std::tuple<std::string, int> error;
    std::vector<std::tuple<int, std::string>> errorArgs;
};

struct ARG_CONVERTOR
{
    std::function<types::InternalType*(types::InternalType*)> convertor;
};

struct ARG
{
    std::vector<ARG_CONVERTOR> convertors;
    std::function<types::InternalType*(types::InternalType* x)> dimsConvertor;
    std::function<std::wstring()> dimsStr;
    std::vector<ARG_VALIDATOR> validators;
    ast::Exp* default_value = nullptr;
    Location loc;
};

std::wstring var2str(types::InternalType* pIT);
// printf-like formatting without length limit (messages can embed user data)
EXTERN_AST std::wstring formatError(const char* fmt, ...);

EXTERN_AST std::function<types::InternalType*(types::InternalType*, const std::wstring& name)> getTypeConvertor(const std::wstring& name);
EXTERN_AST std::tuple<std::function<int(types::typed_list&)>, std::vector<int>> getFunctionValidator(const std::wstring& name);
EXTERN_AST std::tuple<std::string, int> getErrorValidator(const std::wstring& name);
EXTERN_AST std::vector<std::tuple<int, std::string>> getErrorArgs(const std::wstring& name);

types::InternalType* checksize(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims, bool isStatic);
std::wstring dims2str(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims);

#endif /* !__ARGUMENTS_HXX__ */
