/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __OBJECT_HXX__
#define __OBJECT_HXX__

#include <map>
#include <set>
#include "function.hxx"
#include "classdef.hxx"
#include "double.hxx"
#include "user.hxx"

namespace types
{
class EXTERN_AST Object : public UserType
{
public:
    Object(Classdef* classdef);
    Object(const Object& obj);

    virtual ~Object();

    ScilabType getType(void) { return ScilabObject; }
    ScilabId getId(void) { return IdObject; }

    bool isObject() { return true; }
    bool isA(const std::wstring& type)
    {
        if (type == L"object" || type == def->getName())
        {
            return true;
        }

        return def->isA(type);
    }

    virtual bool hasToString() override
    {
        return bHasToString;
    }

    // overload this method if hasToString method return true
    virtual bool toString(std::wostringstream& ostr) override;

    std::wstring getTypeStr() const override { return def->getName(); };
    std::wstring getShortTypeStr() const override { return def->getName(); }

    bool isAssignable() { return true; }
    bool isInvokable() const override { return true; }

    bool hasMethod(const std::wstring& method);
    virtual bool hasGetFields() { return true; }

    // overload this method if hasGetFields method return true
    virtual String* getFields();

    bool hasProperty(const std::wstring& property);
    InternalType* getProperty(const std::wstring& name);
    std::map<std::wstring, InternalType*> getProperties() const { return properties; }
    bool setProperty(const std::wstring& prop, InternalType* value);

    Function::ReturnValue callConstructor(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e);
    Function::ReturnValue callMethod(const std::wstring& method, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e);
    Function::ReturnValue callMethod(const std::wstring& method, Callable* call, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e, bool constructor = false);
    Function::ReturnValue callSuperclassContructor(Classdef* super, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e);

    bool extract(const std::wstring& name, InternalType*& out);
    using UserType::insert;
    Object* insert(typed_list* _pArgs, InternalType* _pSource, const ast::Exp& e);

    Object* clone()
    {
        //IncreaseRef();
        return this;
        //return new Object(*this);
    }

    void loadClassdef(Classdef* def, int level = 0);
    // properties of the classdef: removed ones are dropped, new ones instantiated
    void updateProperties();
    Classdef* getClassdef() const { return def; }
    void updateClassdef(Classdef* classdef);

    void setParent(Object* p) { parent = p; }
    Object* getParent() { return parent; }

    InternalType* serialize();
    bool deserialize(InternalType* data);

    bool getMemory(long long* _piSize, long long* _piSizePlusType);
  private:
    // constructor of cls (class of the object or a parent) called from caller, after the implicit
    // call of the parents it does not initialize itself (done: classes already initialized)
    Function::ReturnValue runConstructor(Classdef* cls, Classdef* caller, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e, std::set<Classdef*>& done);

    Classdef* def;
    std::map<std::wstring, InternalType*> properties;
    Object* parent;
    bool bHasToString;
};
} // namespace types

#endif /* __OBJECT_HXX__ */