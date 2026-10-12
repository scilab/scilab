/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 * 
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __OBJECTMETHOD_HXX__
#define __OBJECTMETHOD_HXX__

#include "callable.hxx"
#include "context.hxx"
#include "object.hxx"

namespace types
{
class EXTERN_AST ObjectMethod : public Callable
{
public:
    // object: instance (classdef for a static method) and class defining the method
    ObjectMethod(const symbol::ObjectScope& object, const std::wstring& funcname, Callable* call);
    virtual ~ObjectMethod();
    virtual std::wstring getTypeStr() const override
    {
        return callable->getTypeStr();
    }
    virtual std::wstring getShortTypeStr() const override
    {
        return callable->getShortTypeStr();
    }

    bool isA(const std::wstring& type)
    {
        return type == getTypeStr();
    }
    
    bool isObjectMethod(void) override
    {
        return true;
    }

    ObjectMethod* clone() override { return new ObjectMethod(object, name, callable); }

    inline ScilabType getType(void) override
    {
        return callable->getType();
    }
    inline ScilabId getId(void) override
    {
        return callable->getId();
    }

    bool toString(std::wostringstream& ostr) override
    {
        return callable->toString(ostr);
    }

    Callable* getCallable()
    {
        return callable;
    }

    virtual ReturnValue call(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out);
    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e);

    // call as method of object: `this` in a macro, current object in a gateway
    static ReturnValue callIn(const symbol::ObjectScope& object, Callable* call, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out);

private:
    symbol::ObjectScope object;
    Callable* callable;
    std::wstring name;
};

} // namespace types

#endif /* !__OBJECTMETHOD_HXX__ */
