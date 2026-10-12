/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __OVERLOAD_HXX__
#define __OVERLOAD_HXX__

#include "function.hxx"
#include "visitor.hxx"

class EXTERN_AST Overload
{

public:
    /**
     ** This function will build an overload function name
     ** depending on input parameters
     ** and maybe on number of result expected <further implementation>
     */
    static std::wstring buildOverloadName(const std::wstring& _stFunctionName, types::typed_list &in, int _iRetCount, bool _isOperator = false, bool _truncated = false);

    /**
     */
    static types::Function::ReturnValue generateNameAndCall(const std::wstring& _stFunctionName, types::typed_list &in, int _iRetCount,
            types::typed_list &out, bool _isOperator = false, bool errorOnUndefined = true, const Location& _Location = Location());

    /**
     */
    static types::Function::ReturnValue call(const std::wstring& _stFunctionName, types::typed_list &in, int _iRetCount,
            types::typed_list &out, bool _isOperator = false, bool errorOnUndefined = true, const Location& _Location = Location());

    static std::wstring getNameFromOper(const int);
    static std::pair<std::wstring, int> getMethodFromOper(const int _oper);

    /**
     ** operator implemented by a method of a classdef operand (plus, plus_s, uminus, and, ...):
     ** left operand first, name_<type of the other operand> before name.
     ** false if no operand implements it (or returns a result)
     */
    static bool callMethodOperator(const std::wstring& name, types::typed_list& in, types::typed_list& out, const Location& location);
};

#endif /* !__OVERLOAD_HXX__ */
