/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "object.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "objectmethod.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "commentexp.hxx"

extern "C"
{
#include "sciprint.h"
}

namespace types
{

Object::Object(Classdef* classdef) : def(classdef), parent(nullptr), bHasToString(true)
{
    loadClassdef(classdef);
    if (hasMethod(L"disp"))
    {
        bHasToString = false;
    }
}

//copy cstor called by clone
Object::Object(const Object& obj)
{
    def = obj.getClassdef();
    loadClassdef(def);
    for (auto&& prop : obj.getProperties())
    {
        InternalType* old = properties[prop.first];
        old->DecreaseRef();
        old->killMe();

        InternalType* val = prop.second->clone();
        properties[prop.first] = val;
        val->IncreaseRef();
    }
}

Object::~Object()
{
    //sciprint("delete %ls\n", def->getName().data());
    try
    {
        typed_list in;
        optional_list opt;
        typed_list out;
        IncreaseRef();
        callMethod(L"delete", in, opt, 0, out, ast::CommentExp(Location(), new std::wstring(L"")));
        DecreaseRef();
    }
    catch (ast::InternalError& ie)
    {
        sciprint(_("Warning: 'delete' method of class '%ls' failed: %ls"), def->getName().data(), ie.GetErrorMessage().data());
    }
    catch (...)
    {
        sciprint(_("Warning: 'delete' method of class '%ls' failed.\n"), def->getName().data());
    }

    for (auto&& prop : properties)
    {
        prop.second->DecreaseRef();
        prop.second->killMe();
    }

    def->removeObject(this);
}

void Object::loadClassdef(Classdef* def, int level)
{
    def->addObject(this);
    auto props = def->getProperties();

    //remove properties from previous classdef that not exist in current classdef
    std::vector<std::wstring> toRemove;
    for (auto&& prop : properties)
    {
        if (props.find(prop.first) == props.end())
        {
            properties[prop.first]->DecreaseRef();
            properties[prop.first]->killMe();
            toRemove.push_back(prop.first);
        }
    }

    for (auto&& r : toRemove)
    {
        properties.erase(r);
    }


    for (auto&& prop :props)
    {
        if (std::get<0>(prop.second).isStatic == false)
        {
            //do not replace variable from previous classdef (override of classdef)
            if (properties.find(prop.first) == properties.end())
            {
                properties[prop.first] = def->instantiateProperty(prop.first, std::get<0>(prop.second));
                properties[prop.first]->IncreaseRef();
            }
        }
    }
}

void Object::updateClassdef(Classdef* classdef)
{
    def = classdef;
    loadClassdef(classdef);
}

bool Object::toString(std::wostringstream& ostr)
{
    typed_list in, out;
    optional_list opt;
    //call overload macro %object_string
    in.push_back(this);
    IncreaseRef();
    auto res = Overload::call(L"%object_string", in, 0, out);
    DecreaseRef();

    if (res == Function::OK)
    {
        if (out.size() == 1)
        {
            if (out[0]->isString())
            {
                String* pS = out[0]->getAs<String>();

                ostr << L"With properties:" << std::endl;
                for (int i = 0; i < pS->getSize(); ++i)
                {
                    ostr << L"  " << pS->get(i) << std::endl;
                }
            }
            else if (out[0]->isDouble())
            {
                ostr << L"With no properties" << std::endl;
            }

            out[0]->killMe();
        }

        return true;
    }

    return false;
}

Classdef* Object::getScopeClassdef()
{
    // access rights are those of the class defining the running method
    // (inherited method or parent constructor), not of the object class
    if (scope.size() > 0)
    {
        const auto& methods = def->getMethods();
        auto it = methods.find(scope.top());
        if (it != methods.end())
        {
            return std::get<1>(it->second);
        }

        const auto& constructors = def->getConstructors();
        auto itc = constructors.find(scope.top());
        if (itc != constructors.end())
        {
            return std::get<1>(itc->second);
        }
    }

    return def;
}

bool Object::extract(const std::wstring& name, InternalType*& out)
{
    Classdef* ref = getScopeClassdef();
    AccessModifier access;

    if (ref->getAccessProperty(name, access))
    {
        if (access == AccessModifier::PUBLIC || (scope.size() > 0 && access != AccessModifier::NONE))
        {
            out = getProperty(name);
            if (out == nullptr)
            {
                out = def->getStatic(name);
            }
            return true;
        }
        else
        {
            wchar_t szError[128];
            os_swprintf(szError, 128, _W("Wrong extraction: property '%ls' is not accessible.\n").c_str(), name.data());
            throw ast::InternalError(szError);
        }
    }

    if (ref->getAccessMethod(name, access))
    {
        if (access == AccessModifier::PUBLIC || (scope.size() > 0 && access != AccessModifier::NONE))
        {
            if (def->isStaticMethod(name))
            {
                out = def->getStatic(name);
            }
            else
            {
                out = new ObjectMethod(this, name, def->getMethod(name));
            }
            return true;
        }
        else
        {
            wchar_t szError[128];
            os_swprintf(szError, 128, _W("Wrong extraction: method '%ls' is not accessible.\n").c_str(), name.data());
            throw ast::InternalError(szError);
        }
    }

    return false;
}

Object* Object::insert(typed_list* _pArgs, InternalType* _pSource, const ast::Exp& e)
{
    // update of property
    if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
    {
        std::wstring field((*_pArgs)[0]->getAs<String>()->get(0));
        setProperty(field, _pSource);
        return this;
    }

    //insertion
    //obj(1,2) = 42

    std::wstring fname = L"insert";
    std::wstring type = _pSource->getShortTypeStr();

    std::wstring s = fname + L"_" + type;
    if (hasMethod(s))
    {
        fname = s;
    }

    if (hasMethod(fname))
    {
        typed_list in;
        optional_list opt;
        typed_list out;

        for (auto&& p : *_pArgs)
        {
            in.push_back(p);
            p->IncreaseRef();
        }

        in.push_back(_pSource);
        _pSource->IncreaseRef();

        Function::ReturnValue ret = callMethod(fname, in, opt, 0, out, e);
        for (auto&& p : *_pArgs)
        {
            p->DecreaseRef();
        }

        _pSource->DecreaseRef();

        if(ret == Function::Error)
        {
            throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), Location());
        }

        return this;
    }

    wchar_t szError[bsiz];
    os_swprintf(szError, bsiz, _W("Wrong insertion: method '%ls' does not exist.\n").c_str(), L"insert");
    throw ast::InternalError(szError);
}

bool Object::setProperty(const std::wstring& prop, InternalType* value)
{
    AccessModifier access;
    if (getScopeClassdef()->getAccessProperty(prop, access))
    {
        if (access == AccessModifier::PUBLIC ||
            (scope.size() > 0 && access != AccessModifier::NONE))
        {
            if (properties.find(prop) != properties.end())
            {
                InternalType* old = properties[prop];
                old->DecreaseRef();
                old->killMe();

                properties[prop] = value;
                value->IncreaseRef();
                return true;
            }
            else if (def->setStatic(prop, value))
            {
                return true;
            }
            else
            {
                wchar_t szError[128];
                os_swprintf(szError, 128, _W("Wrong insertion: property '%ls' does not exist.\n").c_str(), prop.data());
                throw ast::InternalError(szError);
            }
        }
        else
        {
            wchar_t szError[128];
            os_swprintf(szError, 128, _W("Wrong insertion: property '%ls' is not accessible.\n").c_str(), prop.data());
            throw ast::InternalError(szError);
        }
    }
    else
    {
        wchar_t szError[128];
        os_swprintf(szError, 128, _W("Wrong insertion: property '%ls' does not exist.\n").c_str(), prop.data());
        throw ast::InternalError(szError);
    }
    return false;
}

Function::ReturnValue Object::callConstructor(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e)
{
    OBJ_ATTR attr = def->getConstructor();
    if (attr.callable == nullptr)
    {
        return Function::OK_NoResult;
    }

    if (attr.access != AccessModifier::PUBLIC)
    {
        InternalType* obj = symbol::Context::getInstance()->getCurrentObject();
        if (obj == nullptr)
        {
            if (attr.access != AccessModifier::PUBLIC)
            {
                wchar_t szError[128];
                os_swprintf(szError, 128, _W("Constructor of \'%ls\' is not accessible.\n").c_str(), def->getName().data());
                throw ast::InternalError(szError);
            }
        }
        else
        {
        }
    }

    Function::ReturnValue ret = callMethod(def->getName(), attr.callable, in, opt, _iRetCount, out, e);
    return ret == Function::Error ? ret : Function::OK;
}

Function::ReturnValue Object::callSuperclassContructor(Classdef* super, typed_list & in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e)
{
    OBJ_ATTR attr = super->getConstructor();
    if (attr.callable == nullptr)
    {
        return Function::OK_NoResult;
    }

    if (scope.size() == 0 || scope.top() == L"" || scope.top() == def->getName())
    {
        if (attr.access == AccessModifier::PRIVATE || attr.access == AccessModifier::NONE)
        {
            wchar_t szError[128];
            os_swprintf(szError, 128, _W("Constructor of \'%ls\' is not accessible.\n").c_str(), super->getName().data());
            throw ast::InternalError(szError);
        }
    }

    Function::ReturnValue ret = callMethod(super->getName(), attr.callable, in, opt, _iRetCount, out, e);
    if (ret == Function::Error)
    {
        return Function::Error;
    }

    return Function::OK;
}

Function::ReturnValue Object::callMethod(const std::wstring& method, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e)
{
    Callable* call = def->getMethod(method);
    if (call == nullptr)
    {
        // function not found
        return Function::OK_NoResult;
    }

    return callMethod(method, call, in, opt, _iRetCount, out, e);
}

Function::ReturnValue Object::callMethod(const std::wstring& method, Callable* call, typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e)
{
    if (call == nullptr)
    {
        // function not found
        return Function::OK_NoResult;
    }

    if (call->isMacro())
    {
        call->getAs<Macro>()->setParent(this);
    }
    else if (call->isMacroFile())
    {
        call->getAs<MacroFile>()->getMacro()->setParent(this);
    }

    if (ConfigVariable::increaseRecursion())
    {
        //reset previous error before call function
        ConfigVariable::resetError();
        //update verbose";" flag
        ConfigVariable::setVerbose(e.isVerbose());
        // add line and function name in where
        int iFirstLine = e.getLocation().first_line;
        ConfigVariable::where_begin(iFirstLine + 1 - ConfigVariable::getMacroFirstLines(), call, e.getLocation());
        Callable::ReturnValue res = Callable::OK;

        scope.push(def->getMethodClassdef(method));
        try
        {
            res = call->call(in, opt, _iRetCount, out);
        }
        catch (ast::InternalError & ie)
        {
            scope.pop();
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
            throw ie;
        }
        catch (ast::InternalAbort & ia)
        {
            scope.pop();
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
            throw ia;
        }

        scope.pop();

        // remove function name in where
        ConfigVariable::where_end();
        ConfigVariable::decreaseRecursion();

        if (res == Callable::Error)
        {
            throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        return res;
    }
    else
    {
        throw ast::RecursionException();
    }
}

bool Object::hasMethod(const std::wstring& method)
{
    return def->getMethod(method) != nullptr;
}

bool Object::hasProperty(const std::wstring& property)
{
    return getProperty(property) != nullptr;
}

InternalType* Object::getProperty(const std::wstring& name)
{
    if (properties.find(name) != properties.end())
    {
        return properties[name];
    }

    return nullptr;
}

String* Object::getFields()
{
    std::vector<std::wstring> fields;

    for (auto&& p : def->getProperties())
    {
        OBJ_ATTR attr = std::get<0>(p.second);
        if (attr.hidden == false)
        {
            fields.push_back(p.first);
        }
    }

    for (auto&& m : def->getMethods())
    {
        OBJ_ATTR attr = std::get<0>(m.second);
        if (attr.hidden == false)
        {
            fields.push_back(m.first);
        }
    }

    String* pFields = new String(fields.size(), 1);
    for (int i = 0; i < fields.size(); ++i)
    {
        pFields->set(i, fields[i].data());
    }

    return pFields;
}

InternalType* Object::serialize()
{
    InternalType* data = nullptr;
    if (hasMethod(L"saveobj"))
    {
        typed_list in, out;
        optional_list opt;
        if (callMethod(L"saveobj", in, opt, 1, out, ast::CommentExp(Location(), new std::wstring(L""))) == Function::OK)
        {
            if (out.size() == 1)
            {
                return out[0];
            }
        }
    }

    return data;
}

bool Object::deserialize(InternalType* data)
{
    if (hasMethod(L"loadobj"))
    {
        typed_list in, out;
        IncreaseRef();
        data->IncreaseRef();
        in.push_back(data);
        optional_list opt;
        auto ret = callMethod(L"loadobj", in, opt, 1, out,ast::CommentExp(Location(), new std::wstring(L"")));
        data->DecreaseRef();
        DecreaseRef();
        return ret == Function::OK;
    }

    return false;
}

bool Object::getMemory(long long* _piSize, long long* _piSizePlusType)
{
    *_piSize = 0;
    *_piSizePlusType = 0;
    for (auto&& p : getProperties())
    {
        long long s1;
        long long s2;
        if (p.second)
        {
            p.second->getMemory(&s1, &s2);
            *_piSize += s1;
            *_piSizePlusType += s2;
        }
    }

    return true;
}

} // namespace types
