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
    // input position, -1 for a constant, -2 for an expression evaluated at each call (callTime)
    std::vector<std::tuple<int, types::InternalType*>> inputs;
    // this, field of this, of an input or of a class, by index in inputs
    std::map<int, ast::Exp*> callTime;
    std::tuple<std::string, int> error;
    // validator input index (content in the message) or -1 for the given string
    std::vector<std::tuple<int, std::string>> errorArgs;
};

struct ARG_CONVERTOR
{
    // name: function (or Class.method, Class.property) in messages
    std::function<types::InternalType*(types::InternalType*, const std::wstring& name)> convertor;
    // declared type (double, int8, ..., class name)
    std::wstring type;
};

struct ARG
{
    std::vector<ARG_CONVERTOR> convertors;
    std::function<types::InternalType*(types::InternalType* x)> dimsConvertor;
    // dimensions as declared, without expansion nor transposition (assignment of a property)
    std::function<bool(types::InternalType* x)> dimsCheck;
    std::function<std::wstring()> dimsStr;
    std::vector<ARG_VALIDATOR> validators;
    ast::Exp* default_value = nullptr;
    Location loc;
};

std::wstring var2str(types::InternalType* pIT);
// printf-like formatting without length limit (messages can embed user data)
EXTERN_AST std::wstring formatError(const char* fmt, ...);

EXTERN_AST std::function<types::InternalType*(types::InternalType*, const std::wstring& name)> getTypeConvertor(const std::wstring& name);
// x is of the builtin type (double, int8, ..., string), without conversion
EXTERN_AST bool isOfBuiltinType(types::InternalType* x, const std::wstring& type);
// type that is not a conversion function: a classdef, without conversion
// true if x is an object of this class or of a derived one, throws if the class does not exist
EXTERN_AST bool isOfClassType(types::InternalType* x, const std::wstring& type, const std::wstring& name);
// this, or field of a variable (this.max, opts.max, MyClass.max): evaluated when checked
EXTERN_AST bool isCallTimeArgument(ast::Exp* e);
// source of a call time argument: this or a.b.c
EXTERN_AST std::wstring callTimeArgumentText(ast::Exp* e);
// variable of a call time argument: this, opts, MyClass
EXTERN_AST std::wstring callTimeArgumentRoot(ast::Exp* e);

// how the parameters of a validator are resolved
struct ValidatorContext
{
    // function, or Class.property, in messages
    std::string where;
    // index of the checked value
    int self = 0;
    // index of an input from its name, -1 if it is not an input
    std::function<int(const std::wstring&)> input;
    // property: no object (this), expressions copied (the definition is deleted), messages of properties
    bool property = false;
};

// validator of an arguments block or of a property: name or name(parameters)
EXTERN_AST ARG_VALIDATOR buildValidator(ast::Exp* v, const ValidatorContext& ctx);
EXTERN_AST std::tuple<std::function<int(types::typed_list&)>, std::vector<int>> getFunctionValidator(const std::wstring& name);
EXTERN_AST std::tuple<std::string, int> getErrorValidator(const std::wstring& name);
EXTERN_AST std::tuple<std::string, int> getPropertyErrorValidator(const std::wstring& name);
EXTERN_AST std::vector<std::tuple<int, std::string>> getErrorArgs(const std::wstring& name);

types::InternalType* checksize(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims, bool isStatic);
// x has the declared dimensions, without expansion nor transposition
EXTERN_AST bool hasDims(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims);
std::wstring dims2str(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims);

#endif /* !__ARGUMENTS_HXX__ */
