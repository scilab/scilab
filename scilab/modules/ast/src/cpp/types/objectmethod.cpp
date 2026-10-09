/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "objectmethod.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "function.hxx"

extern "C"
{
#include "sciprint.h"
}

namespace types
{
ObjectMethod::ObjectMethod(InternalType* obj, const std::wstring& funcname, Callable* call) : Callable(), object(obj), callable(call), name(funcname)
{
    obj->IncreaseRef();
}

ObjectMethod::~ObjectMethod()
{
    object->DecreaseRef();
    object->killMe();
}

bool ObjectMethod::invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e)
{
    if (callable->isFunction())
    {
        symbol::Context::getInstance()->scope_object_begin(object, callable->getName());
        Callable::ReturnValue ret = callable->call(in, opt, _iRetCount, out);
        symbol::Context::getInstance()->scope_object_end();
        return ret != Callable::ReturnValue::Error;
    }

    MacroParentScope scope(callable, object);
    return callable->invoke(in, opt, _iRetCount, out, e);
}

Callable::ReturnValue ObjectMethod::call(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out)
{
    if (callable->isFunction())
    {
        symbol::Context::getInstance()->scope_object_begin(object, callable->getName());
        Callable::ReturnValue ret = callable->call(in, opt, _iRetCount, out);
        symbol::Context::getInstance()->scope_object_end();
        return ret;
    }

    MacroParentScope scope(callable, object);
    return callable->call(in, opt, _iRetCount, out);
}
} // namespace type