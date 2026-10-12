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
ObjectMethod::ObjectMethod(const symbol::ObjectScope& object, const std::wstring& funcname, Callable* call) : Callable(), object(object), callable(call), name(funcname)
{
    object.self->IncreaseRef();
    // kept alive if the class is redefined while the method is stored
    object.cls->IncreaseRef();
}

ObjectMethod::~ObjectMethod()
{
    object.self->DecreaseRef();
    object.self->killMe();
    object.cls->DecreaseRef();
    object.cls->killMe();
}

bool ObjectMethod::invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e)
{
    callable->invokeCall(e, [&]() { return callIn(object, callable, in, opt, _iRetCount, out); });
    return true;
}

Callable::ReturnValue ObjectMethod::call(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out)
{
    return callIn(object, callable, in, opt, _iRetCount, out);
}

Callable::ReturnValue ObjectMethod::callIn(const symbol::ObjectScope& object, Callable* call, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out)
{
    if (call->isMacro())
    {
        return call->getAs<Macro>()->call(in, opt, _iRetCount, out, object);
    }

    if (call->isMacroFile())
    {
        return call->getAs<MacroFile>()->call(in, opt, _iRetCount, out, object);
    }

    // gateway: getCurrentObject() gives the object
    symbol::Context* ctx = symbol::Context::getInstance();
    ctx->scope_object_begin(object);
    try
    {
        Callable::ReturnValue ret = call->call(in, opt, _iRetCount, out);
        ctx->scope_object_end();
        return ret;
    }
    catch (...)
    {
        ctx->scope_object_end();
        throw;
    }
}
} // namespace type