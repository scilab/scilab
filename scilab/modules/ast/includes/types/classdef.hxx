/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __CLASSDEF_HXX__
#define __CLASSDEF_HXX__

#include <map>
#include <tuple>
#include "callable.hxx"
#include "function.hxx"
#include "internal.hxx"
#include "arguments.hxx"

namespace ast
{
class ArgumentDec;
}

namespace types
{

enum AccessModifier
{
    NONE,
    PRIVATE,
    PROTECTED,
    PUBLIC
};

struct OBJ_ATTR
{
    OBJ_ATTR() : access(AccessModifier::PUBLIC), isStatic(false), hidden(true), callable(nullptr) {}
    ARG arg;
    AccessModifier access;
    bool isStatic;
    bool hidden;
    Callable* callable;
};

class Object;

class EXTERN_AST Classdef : public types::InternalType
{
public:
    Classdef(const std::wstring& name,
        const std::map<std::wstring, OBJ_ATTR>& properties,
        const std::map<std::wstring, OBJ_ATTR>& methods,
        const std::map<std::wstring, std::vector<types::InternalType*>>& enumerations,
        const std::vector<std::wstring>& superclass);

    virtual ~Classdef();

    ScilabType getType(void) { return ScilabClassdef; }
    ScilabId getId(void) { return IdClassdef; }

    bool isClassdef() { return true; }
    virtual bool toString(std::wostringstream& ostr) override;

    bool isA(const std::wstring& type)
    {
        if (type == L"classdef" || type == name)
        {
            return true;
        }

        for (auto&& s : supers)
        {
            if (std::get<1>(s)->isA(type))
            {
                return true;
            }
        }

        return false;
    }

    virtual std::wstring getTypeStr() const override { return name; };
    virtual std::wstring getShortTypeStr() const override { return name; }

    InternalType* clone(void)
    {
        return this;
    }

    std::wstring getName() { return name; }

    std::vector<std::wstring> getPublicProperties();
    std::vector<std::wstring> getPublicMethods();
    std::vector<std::wstring> getEnumeration();

    bool getAccessProperty(const std::wstring& name, AccessModifier& access);
    bool getAccessMethod(const std::wstring& name, AccessModifier& access);
    // class defining the property or method, nullptr if it does not exist
    Classdef* getOwner(const std::wstring& name, AccessModifier& access);
    bool isAccessible(const std::wstring& name);
    // access to a member declared in owner, from a method of caller (nullptr outside of methods)
    static bool canAccess(AccessModifier access, Classdef* owner, Classdef* caller);

    std::vector<std::tuple<std::wstring, Classdef*>> getSuperclass() const;

    bool isInvokable() const
    {
        return true;
    }

    bool invoke(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e);
    bool extract(const std::wstring& name, InternalType*& out);
    Classdef* insert(typed_list* _pArgs, InternalType* _pSource);

    // class defining the method or constructor name
    Classdef* getMethodOwner(const std::wstring& name);

    bool isAncestorOf(const Classdef* maybeDerived);
    // direct parent
    bool isSubclassOf(const Classdef* parent) const;
    // parents whose constructor is not called by the constructor of this class
    const std::vector<Classdef*>& getImplicitSupers() const { return implicitSupers; }

    // loads the parents and builds the member tables, once
    void LoadClassdef();
    bool isLoaded() const { return initialized; }
    // back to the declarations, reloaded at next use (parent class redefined)
    void unload();

    void addStaticProperty(const std::wstring& name, const OBJ_ATTR& attr);
    void addStaticMethod(const std::wstring& name, const OBJ_ATTR& attr);
    InternalType* getStatic(const std::wstring& name);
    bool hasStatic(const std::wstring& name);
    bool setStatic(const std::wstring& name, InternalType* pIT);
    bool isStaticMethod(const std::wstring& name);
    InternalType* getStaticMethod(const std::wstring& name);

    InternalType* instantiateProperty(const std::wstring& name, const OBJ_ATTR& attr);
    // size, type and validators of a property: x (1,1) double {mustBePositive} = 1
    static void parsePropertyArgument(const std::wstring& where, const ast::ArgumentDec& dec, ARG& arg);
    // value of the declared size and type, error if a validator fails. Only a default value
    // (written by the class) is converted: an assigned value is stored as given
    InternalType* checkProperty(const std::wstring& name, InternalType* value, bool convert = false);
    Object* createEmptyInstance();

private:
    std::wstring name;
    std::map<std::wstring, OBJ_ATTR> props;
    std::map<std::wstring, OBJ_ATTR> meths;
    std::map<std::wstring, std::vector<types::InternalType*>> enumerations;
    std::vector<std::wstring> superclass;
    std::map<std::wstring, InternalType*> instances;
    std::vector<std::tuple<std::wstring, Classdef*>> supers;
    std::vector<Classdef*> implicitSupers;
    bool initialized;
    bool initializing;

    void resetLoading();
    void internalCall(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e);

    std::vector<Object*> objects;
    /*****************************/

    //new implementation following rules of shadowing of scripting languages (js/python/matlab)
    std::map<std::wstring, std::tuple<OBJ_ATTR, Classdef*>> methods;
    std::map<std::wstring, std::tuple<OBJ_ATTR, Classdef*>> properties;
    std::map<std::wstring, std::tuple<OBJ_ATTR, Classdef*>> constructors;

  public:

    void addHelpers();
    void addHelper(const std::wstring& name, Function::ReturnValue (Classdef::*_pFunc)(typed_list&, int, typed_list&));
    
    const std::map<std::wstring, std::tuple<OBJ_ATTR, Classdef*>>& getMethods() const { return methods; }
    const std::map<std::wstring, std::tuple<OBJ_ATTR, Classdef*>>& getConstructors() const { return constructors; }

    Callable* getMethod(const std::wstring& name);

    OBJ_ATTR getConstructor() { return std::get<0>(constructors[getName()]); }
    void showMethodTable();

    const std::map<std::wstring, std::tuple<OBJ_ATTR, Classdef*>>& getProperties() const { return properties; }

    void addObject(Object* obj)
    {
        LoadClassdef();
        objects.push_back(obj);
    }

    void removeObject(Object* obj)
    {
        auto it = std::find(objects.begin(), objects.end(), obj);
        if (it != objects.end())
        {
            *it = objects.back(); //replace found object by last one
            objects.pop_back(); //remove last object (duplicate) and remove O(1) instead of O(n) with erase.
        }
    }

    std::vector<Object*> getObjects() { return objects; }
  protected:
    Function::ReturnValue object_disp(typed_list& in, int _iRetCount, typed_list& out);
    Function::ReturnValue object_eq(typed_list& in, int _iRetCount, typed_list& out);
    Function::ReturnValue object_ne(typed_list& in, int _iRetCount, typed_list& out);
    Function::ReturnValue object_outline(typed_list& in, int _iRetCount, typed_list& out);
    Function::ReturnValue object_clone(typed_list& in, int _iRetCount, typed_list& out);
    Function::ReturnValue object_delete(typed_list& in, int _iRetCount, typed_list& out);
};
} // namespace types

#endif /* __CLASSDEF_HXX__ */
