/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "classdef.hxx"
#include "object.hxx"
#include "objectmethod.hxx"
#include "context.hxx"
#include "runvisitor.hxx"
#include "argumentvisitor.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "sciprint.h"
}

namespace types
{
Classdef::Classdef(const std::wstring& name,
    const std::map<std::wstring, OBJ_ATTR>& properties,
    const std::map<std::wstring, OBJ_ATTR>& methods,
    const std::map<std::wstring, std::vector<types::InternalType*>>& enums,
    const std::vector<std::wstring>& super)
    : name(name), props(properties), meths(methods), enumerations(enums), superclass(super), initialized(false), initializing(false)
{
    std::reverse(superclass.begin(), superclass.end());
}

void Classdef::LoadClassdef()
{
    if (initialized == true)
    {
        return;
    }

    if (initializing)
    {
        char msg[128];
        os_sprintf(msg, _("Inheritance cycle detected for class '%s'.\n"), scilab::UTF8::toUTF8(name).data());
        throw ast::InternalError(scilab::UTF8::toWide(msg));
    }

    initializing = true;

    // on failure, back to the not loaded state: the next access reports the error again
    struct LoadGuard
    {
        Classdef* def;
        ~LoadGuard()
        {
            if (def)
            {
                def->resetLoading();
            }
        }
    } guard{this};

    for (auto&& s : superclass)
    {
        Classdef* def = symbol::Context::getInstance()->getClassdef(s);
        if (def)
        {
            supers.push_back({s, def});
        }
        else
        {
            char msg[128];
            os_sprintf(msg, _("'%s' does not exist\n"), scilab::UTF8::toUTF8(s).data());
            throw ast::InternalError(scilab::UTF8::toWide(msg));
        }
    }

    //classdef A < B & C => C in first and after B
    for (auto&& s : superclass)
    {
        Classdef* def = symbol::Context::getInstance()->getClassdef(s);
        def->LoadClassdef();
        auto supermethods = def->getMethods();
        for (auto&& v : supermethods)
        {
            OBJ_ATTR attr = std::get<0>(v.second);
            // reduce level
            attr.access = reduceAccess(attr.access);
            this->methods[v.first] = {attr, def};
        }

        auto superproperties = def->getProperties();
        for (auto&& v : superproperties)
        {
            OBJ_ATTR attr = std::get<0>(v.second);
            attr.access = reduceAccess(attr.access);
            this->properties[v.first] = {attr, def};
        }

        auto superconstructors = def->getConstructors();
        for (auto&& v : superconstructors)
        {
            OBJ_ATTR attr = std::get<0>(v.second);
            attr.access = reduceAccess(attr.access);
            this->constructors[v.first] = {attr, def};
        }
    }

    // fill vtable
    for (auto&& m : meths)
    {
        if (m.first == getName())
        {
            OBJ_ATTR attr;
            attr.access = m.second.access;
            attr.callable = m.second.callable;
            constructors[getName()] = {attr, this};
            continue;
        }

        m.second.hidden = false;
        methods[m.first] = {m.second, this};
    }

    for (auto&& p : props)
    {
        p.second.hidden = false;
        properties[p.first] = {p.second, this};
    }

    addHelpers();
    //showMethodTable();

    //default empty constructor information
    if (constructors.find(getName()) == constructors.end())
    {
        OBJ_ATTR attr;
        attr.access = AccessModifier::PUBLIC;
        attr.callable = nullptr;
        constructors[getName()] = {attr, this};
    }

    for (auto&& p : properties)
    {
        if (std::get<0>(p.second).isStatic && std::get<1>(p.second) == this)
        {
            addStaticProperty(p.first, std::get<0>(p.second));
        }
    }

    for (auto&& m : methods)
    {
        if (std::get<0>(m.second).isStatic && std::get<1>(m.second) == this)
        {
            addStaticMethod(m.first, std::get<0>(m.second));
        }
    }

    //clean constructor information
    guard.def = nullptr;
    props.clear();
    meths.clear();
    superclass.clear();
    initializing = false;
    initialized = true;
}

void Classdef::resetLoading()
{
    for (auto&& i : instances)
    {
        // static methods are owned by their declaration
        i.second->DecreaseRef();
        auto m = methods.find(i.first);
        if (m == methods.end() || std::get<0>(m->second).isStatic == false)
        {
            i.second->killMe();
        }
    }

    // helpers created by this class
    for (auto&& m : methods)
    {
        const OBJ_ATTR& attr = std::get<0>(m.second);
        if (std::get<1>(m.second) == this && attr.hidden && attr.callable)
        {
            attr.callable->DecreaseRef();
            attr.callable->killMe();
        }
    }

    instances.clear();
    supers.clear();
    methods.clear();
    properties.clear();
    constructors.clear();
    initializing = false;
}

bool Classdef::toString(std::wostringstream& ostr)
{
    /*
    if (properties.size() > 0)
    {
        std::wostringstream prop_ostr;
        for (auto&& p : properties)
        {
            OBJ_ATTR attr = std::get<0>(p.second);
            if (attr.access == AccessModifier::PUBLIC)
            {
                prop_ostr << "    " << p.first << std::endl;
            }
        }

        if (prop_ostr.str().size() != 0)
        {
            ostr << L"with properties :" << std::endl;
            ostr << prop_ostr.str();
        }
    }

    if (methods.size() > 0)
    {
        std::wostringstream method_ostr;
        for (auto&& m : methods)
        {
            OBJ_ATTR attr = std::get<0>(m.second);
            if (attr.hidden == false && attr.access == AccessModifier::PUBLIC)
            {
                method_ostr << "    " << m.first << std::endl;
            }
        }

        if (method_ostr.str().size() != 0)
        {
            ostr << L"with methods:" << std::endl;
            ostr << method_ostr.str();
        }
    }

    if (enumerations.size() > 0)
    {
        ostr << L"with enumeration :" << std::endl;
        for (auto&& e : enumerations)
        {
            ostr << "    " << e.first << std::endl;
        }
    }
    */

    ostr << L"Constructor of classdef \"" << name << L"\"" << std::endl;
    return true;
}

void Classdef::showMethodTable()
{
    int max_len = 0;
    for (auto&& v : methods)
    {
        max_len = std::max(max_len, (int)v.first.length());
    }

    for (auto&& v : methods)
    {
        std::wstring access;
        switch (std::get<0>(v.second).access)
        {
            case AccessModifier::PUBLIC:
            access = L"public";
            break;
        case AccessModifier::PROTECTED:
            access = L"protected";
            break;
        case AccessModifier::PRIVATE:
            access = L"private";
            break;
        case AccessModifier::NONE:
            continue;
        }

        Classdef* def = std::get<1>(v.second);
        sciprint("%*ls [%-9ls] (%ls)\n", -max_len, v.first.data(), access.data(), def->getName().data());
    }

    sciprint("\n");
}

Classdef::~Classdef()
{
    //sciprint("delete Classdef\n");
    for (auto&& i : instances)
    {
        i.second->DecreaseRef();
        i.second->killMe();
    }

    for (auto&& e : enumerations)
    {
        for (auto&& it : e.second)
        {
            it->DecreaseRef();
            it->killMe();
        }
    }
}

std::vector<std::tuple<std::wstring, Classdef*>> Classdef::getSuperclass() const
{
    return supers;
}

bool Classdef::isAncestorOf(const Classdef* derived)
{
    if (this == derived)
    {
        return true;
    }

    for (auto&& t : derived->getSuperclass())
    {
        auto* base = std::get<1>(t);
        if (base == this || this->isAncestorOf(base))
        {
            return true;
        }
    }
    return false;
}

bool Classdef::invoke(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e)
{
    AccessModifier ctorAccess;
    if (getAccessMethod(getName(), ctorAccess))
    {
        switch (ctorAccess)
        {
            case AccessModifier::PUBLIC:
            {
                internalCall(in, opt, _iRetCount, out, e);
                return true;
            }
            case AccessModifier::PROTECTED:
            {
                types::InternalType* pIT = symbol::Context::getInstance()->getCurrentObject();
                if (pIT != nullptr && pIT->isObject())
                {
                    types::Object* currentObj = pIT->getAs<types::Object>();
                    if (currentObj)
                    {
                        types::Classdef* caller = currentObj->getClassdef();
                        if (caller == this || isAncestorOf(caller))
                        {
                            internalCall(in, opt, _iRetCount, out, e);
                            return true;
                        }
                    }
                }
                break;
            }
            case AccessModifier::PRIVATE:
            {
                //continue to error
                break;
            }
            default: {}
        }

        wchar_t szError[128];
        os_swprintf(szError, 128, _W("Wrong call: constructor of '%ls' is not accessible.\n").c_str(), name.data());
        throw ast::InternalError(szError);
    }

    /*
    if (methods[name].access != AccessModifier::PUBLIC)
    {
        InternalType* pIT = symbol::Context::getInstance()->getCurrentObject();
        if (pIT == nullptr || (pIT->isClassdef() && pIT != this) || (pIT->isObject() && pIT->getAs<Object>()->getClassdef() != this))
        {
            wchar_t szError[128];
            os_swprintf(szError, 128, _W("Wrong call: constructor of '%ls' is not accessible.\n").c_str(), name.data());
            throw ast::InternalError(szError);
        }
    }
    */

    return false;
}

void Classdef::internalCall(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out, const ast::Exp& e)
{
    //call of class constructor in constructor (A < B  A() { B() }
    InternalType* current = symbol::Context::getInstance()->getCurrentObject();
    if (current && current->isObject())
    {
        if (_iRetCount == 0) //not an assignation
        {
            Object* obj = current->getAs<Object>();
            if (isAncestorOf(obj->getClassdef()))
            {
                obj->IncreaseRef();
                obj->callSuperclassContructor(this, in, opt, _iRetCount, out, e);
                obj->DecreaseRef();
                return;
            }
        }
    }

    Object* obj = createEmptyInstance();
    obj->IncreaseRef();
    obj->callConstructor(in, opt, _iRetCount, out, e);
    obj->DecreaseRef();
    out.push_back(obj);
}

Object* Classdef::createEmptyInstance()
{
    LoadClassdef();
    return new Object(this);
}

Classdef* Classdef::insert(typed_list* _pArgs, InternalType* _pSource)
{
    LoadClassdef();

    if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
    {
        std::wstring field((*_pArgs)[0]->getAs<types::String>()->get(0));
        AccessModifier access;
        if (getAccessProperty(field, access))
        {
            if (isAccessible(field))
            {
                InternalType* value = checkProperty(field, _pSource);
                if (setStatic(field, value))
                {
                    return this;
                }

                if (value != _pSource)
                {
                    value->killMe();
                }

                wchar_t szError[128];
                os_swprintf(szError, 128, _W("Wrong insertion: property '%ls' is not a static property.\n").c_str(), field.data());
                throw ast::InternalError(szError);
            }
            else
            {
                wchar_t szError[128];
                os_swprintf(szError, 128, _W("Wrong insertion: property '%ls' is not accessible.\n").c_str(), field.data());
                throw ast::InternalError(szError);
            }
        }
        else
        {
            wchar_t szError[128];
            os_swprintf(szError, 128, _W("Wrong insertion: property '%ls' does not exist.\n").c_str(), field.data());
            throw ast::InternalError(szError);
        }
    }
    else
    {
        wchar_t szError[128];
        os_swprintf(szError, 128, _W("Wrong insertion: invalid index.\n").c_str());
        throw ast::InternalError(szError);
    }

    return this;
}

bool Classdef::getAccessProperty(const std::wstring& name, AccessModifier& access)
{
    auto p = properties.find(name);
    if (p == properties.end())
    {
        return false;
    }

    access = std::get<0>(p->second).access;
    return true;
}

bool Classdef::getAccessMethod(const std::wstring& name, AccessModifier& access)
{
    auto p = methods.find(name);
    if (p == methods.end())
    {
        // default cstor
        if (name == getName())
        {
            access = AccessModifier::PUBLIC;
            return true;
        }

        return false;
    }

    access = std::get<0>(p->second).access;
    return true;
}

Classdef* Classdef::getOwner(const std::wstring& name, AccessModifier& access)
{
    auto p = properties.find(name);
    if (p != properties.end())
    {
        access = std::get<0>(p->second).access;
        return std::get<1>(p->second);
    }

    auto m = methods.find(name);
    if (m != methods.end())
    {
        access = std::get<0>(m->second).access;
        return std::get<1>(m->second);
    }

    return nullptr;
}

bool Classdef::isAccessible(const std::wstring& name)
{
    AccessModifier access;
    Classdef* owner = getOwner(name, access);
    if (owner == nullptr || access == AccessModifier::PUBLIC)
    {
        return true;
    }

    // access rights are those of the class of the running code,
    // the member must be the same one seen from this class
    Classdef* caller = getCallerClassdef();
    if (caller == nullptr)
    {
        return false;
    }

    AccessModifier callerAccess;
    return caller->getOwner(name, callerAccess) == owner && callerAccess != AccessModifier::NONE;
}

Classdef* Classdef::getCallerClassdef()
{
    InternalType* current = symbol::Context::getInstance()->getCurrentObject();
    if (current == nullptr)
    {
        return nullptr;
    }

    if (current->isObject())
    {
        return current->getAs<Object>()->getScopeClassdef();
    }

    // static method
    if (current->isClassdef())
    {
        return current->getAs<Classdef>();
    }

    return nullptr;
}

bool Classdef::extract(const std::wstring& name, InternalType*& out)
{
    LoadClassdef();

    // static properties and methods: local then parents
    InternalType* staticVal = getStatic(name);
    if (staticVal)
    {
        if (isAccessible(name) == false)
        {
            wchar_t szError[128];
            os_swprintf(szError, 128, _W("Wrong extraction: '%ls' is not accessible.\n").c_str(), name.data());
            throw ast::InternalError(szError);
        }

        out = isStaticMethod(name) ? getStaticMethod(name) : staticVal;
        return true;
    }

    auto e = enumerations.find(name);
    if (e != enumerations.end())
    {
        optional_list opt;
        typed_list out1;

        Object* obj = createEmptyInstance();
        obj->IncreaseRef();

        // enumeration values are built by the class itself (constructor can be private)
        symbol::Context::getInstance()->scope_object_begin(this, getName());
        try
        {
            obj->callConstructor(e->second, opt, 0, out1, ast::CommentExp(Location(), new std::wstring(L"")));
        }
        catch (...)
        {
            symbol::Context::getInstance()->scope_object_end();
            throw;
        }

        symbol::Context::getInstance()->scope_object_end();
        instances[e->first] = obj;

        out = obj;
        return true;
    }

    // enumeration of a parent class not yet built
    for (auto&& s : supers)
    {
        if (std::get<1>(s)->extract(name, out))
        {
            return true;
        }
    }

    return false;
}

Callable* Classdef::getMethod(const std::wstring& name)
{
    if (methods.find(name) == methods.end())
    {
        return nullptr;
    }

    return std::get<0>(methods[name]).callable;
}

std::wstring Classdef::getMethodClassdef(const std::wstring& name)
{
    if (name == getName())
    {
        return name;
    }

    auto p = methods.find(name);
    if (p != methods.end())
    {
        return std::get<1>(p->second)->getName();
    }

    return L"";
}

std::vector<std::wstring> Classdef::getPublicProperties()
{
    std::vector<std::wstring> props;
    for (auto&& p : properties)
    {
        OBJ_ATTR attr = std::get<0>(p.second);
        if (attr.hidden == false && attr.access == AccessModifier::PUBLIC)
        {
            props.push_back(p.first);
        }
    }

    return props;
}

std::vector<std::wstring> Classdef::getPublicMethods()
{
    std::vector<std::wstring> m;
    for (auto&& p : methods)
    {
        OBJ_ATTR attr = std::get<0>(p.second);
        if (attr.hidden == false && attr.access == AccessModifier::PUBLIC)
        {
            m.push_back(p.first);
        }
    }

    return m;
}
std::vector<std::wstring> Classdef::getEnumeration()
{
    std::vector<std::wstring> m;
    for (auto&& p : enumerations)
    {
        m.push_back(p.first);
    }

    for (auto&& s : supers)
    {
        auto p = std::get<1>(s)->getEnumeration();
        m.insert(m.end(), p.begin(), p.end());
    }

    return m;
}

void Classdef::addStaticProperty(const std::wstring& name, const OBJ_ATTR& attr)
{
    instances[name] = instantiateProperty(name, attr);
    instances[name]->IncreaseRef();
}

void Classdef::addStaticMethod(const std::wstring& name, const OBJ_ATTR& attr)
{
    if (attr.callable)
    {
        attr.callable->IncreaseRef();
        instances[name] = attr.callable;
    }
}

bool Classdef::isStaticMethod(const std::wstring& name)
{
    auto it = methods.find(name);
    if (it != methods.end())
    {
        return std::get<0>(it->second).isStatic;
    }

    // look in parent classes
    for (auto&& s : supers)
    {
        if (std::get<1>(s)->isStaticMethod(name))
        {
            return true;
        }
    }

    return false;
}

InternalType* Classdef::getStaticMethod(const std::wstring& name)
{
    InternalType* call = getStatic(name);
    AccessModifier access;
    Classdef* owner = getOwner(name, access);
    if (call == nullptr || owner == nullptr)
    {
        return call;
    }

    // run in the context of the class defining the method (no `this`)
    return new ObjectMethod(owner, name, call->getAs<Callable>());
}

bool Classdef::hasStatic(const std::wstring& name)
{
    return instances.find(name) != instances.end();
}

InternalType* Classdef::getStatic(const std::wstring& name)
{
    if (hasStatic(name))
    {
        return instances[name];
    }

    // look in parent classes
    for (auto&& s : supers)
    {
        InternalType* val = std::get<1>(s)->getStatic(name);
        if (val)
        {
            return val;
        }
    }

    return nullptr;
}

bool Classdef::setStatic(const std::wstring& name, InternalType* pIT)
{
    if (hasStatic(name))
    {
        instances[name]->DecreaseRef();
        instances[name]->killMe();

        pIT->IncreaseRef();
        instances[name] = pIT;
        return true;
    }

    // look in parent classes
    for (auto&& s : supers)
    {
        if (std::get<1>(s)->setStatic(name, pIT))
        {
            return true;
        }
    }

    return false;
}
InternalType* Classdef::instantiateProperty(const std::wstring& name, const OBJ_ATTR& attr)
{
    if (attr.arg.default_value == nullptr)
    {
        return types::Double::Empty();
    }
    else
    {
        ast::RunVisitor* exec = (ast::RunVisitor*)ConfigVariable::getDefaultVisitor();
        attr.arg.default_value->accept(*exec);
        InternalType* pIT = exec->getResult();
        if (pIT == nullptr || pIT->isAssignable() == false)
        {
            char msg[128];
            os_sprintf(msg, _("%s: Unable to evaluate default value.\n"), scilab::UTF8::toUTF8(name).data());
            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, attr.arg.default_value->getLocation());
        }

        pIT->IncreaseRef();
        delete exec;
        pIT->DecreaseRef();

        try
        {
            return checkProperty(name, pIT);
        }
        catch (const ast::InternalError&)
        {
            pIT->killMe();
            throw;
        }
    }
}

void Classdef::parsePropertyArgument(const std::wstring& where, const ast::ArgumentDec& dec, ARG& arg)
{
    std::string name = scilab::UTF8::toUTF8(where);
    std::wstring prop = dec.getArgumentName()->getAs<ast::SimpleVar>()->getSymbol().getName();
    arg.loc = dec.getLocation();

    // dims: constants only, there is no other input to refer to
    std::vector<std::tuple<std::vector<int>, symbol::Variable*>> dims;
    for (auto&& dim : dec.getArgumentDims()->getExps())
    {
        if (dim->isColonVar())
        {
            dims.push_back({{-1}, nullptr});
        }
        else if (dim->isDoubleExp())
        {
            dims.push_back({{static_cast<int>(dim->getAs<ast::DoubleExp>()->getValue())}, nullptr});
        }
        else if (dim->isMatrixExp() && dim->getAs<ast::MatrixExp>()->getLines().size() == 1)
        {
            std::vector<int> d;
            for (auto&& c : dim->getAs<ast::MatrixExp>()->getLines()[0]->getAs<ast::MatrixLineExp>()->getColumns())
            {
                if (c->isDoubleExp() == false)
                {
                    throw ast::InternalError(formatError(_("%s: Dimension must be a number, row vector or ':'.\n"), name.data()), 999, dim->getLocation());
                }

                d.push_back(static_cast<int>(c->getAs<ast::DoubleExp>()->getValue()));
            }

            dims.push_back({d, nullptr});
        }
        else
        {
            throw ast::InternalError(formatError(_("%s: Dimension must be a number, row vector or ':'.\n"), name.data()), 999, dim->getLocation());
        }
    }

    if (dims.size() != 0)
    {
        arg.dimsConvertor = [dims](InternalType* x) { return checksize(x, dims, false); };
        arg.dimsStr = [dims]() { return dims2str(dims); };
    }

    // type conversion
    if (dec.getArgumentType()->isSimpleVar())
    {
        std::wstring type = dec.getArgumentType()->getAs<ast::SimpleVar>()->getSymbol().getName();
        auto f = getTypeConvertor(type);
        if (f == nullptr)
        {
            throw ast::InternalError(formatError(_("%s: Unknown conversion function '%s'\n"), name.data(), scilab::UTF8::toUTF8(type).data()), 999, dec.getArgumentType()->getLocation());
        }

        ARG_CONVERTOR conv;
        conv.convertor = [f, where](InternalType* x) { return f(x, where); };
        arg.convertors.push_back(conv);
    }

    // validators: the property itself (0) or constants (-1)
    for (auto&& v : dec.getArgumentValidators()->getExps())
    {
        ARG_VALIDATOR validator;
        std::wstring vname;
        std::vector<int> rhs;
        if (v->isSimpleVar())
        {
            vname = v->getAs<ast::SimpleVar>()->getSymbol().getName();
            std::tie(validator.validator, rhs) = getFunctionValidator(vname);
            if (validator.validator == nullptr)
            {
                throw ast::InternalError(formatError(_("%s: Unknown validation function '%s'\n"), name.data(), scilab::UTF8::toUTF8(vname).data()), 999, v->getLocation());
            }

            if (rhs[0] != 1)
            {
                throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): %d expected.\n"), name.data(), 1), 999, v->getLocation());
            }

            validator.inputs.push_back({0, nullptr});
        }
        else if (v->isCallExp() && v->getAs<ast::CallExp>()->getName().isSimpleVar())
        {
            ast::CallExp* c = v->getAs<ast::CallExp>();
            vname = c->getName().getAs<ast::SimpleVar>()->getSymbol().getName();
            std::tie(validator.validator, rhs) = getFunctionValidator(vname);
            if (validator.validator == nullptr)
            {
                throw ast::InternalError(formatError(_("%s: \"%ls\" is not a validation function.\n"), name.data(), vname.data()), 999, v->getLocation());
            }

            int size = static_cast<int>(c->getArgs().size());
            if (rhs.size() == 1 && rhs[0] != size && rhs[0] != -1)
            {
                throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): %d expected.\n"), name.data(), rhs[0]), 999, v->getLocation());
            }

            if (rhs.size() > 1 && (size < rhs[0] || size > rhs[1]))
            {
                throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): between %d and %d expected.\n"), name.data(), rhs[0], rhs[1]), 999, v->getLocation());
            }

            for (auto&& in : c->getArgs())
            {
                if (in->isSimpleVar() && in->getAs<ast::SimpleVar>()->getSymbol().getName() == prop)
                {
                    validator.inputs.push_back({0, nullptr});
                    continue;
                }

                ast::ArgumentVisitor check;
                in->accept(check);
                if (check.getStatus() == false)
                {
                    throw ast::InternalError(formatError(_("%s: argument must be constant expression.\n"), name.data()), 999, in->getLocation());
                }

                std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());
                InternalType* pIT = nullptr;
                try
                {
                    in->accept(*exec);
                    pIT = ((ast::RunVisitor*)exec.get())->getResult();
                }
                catch (const ast::InternalError&)
                {
                }

                if (pIT == nullptr || pIT->isAssignable() == false)
                {
                    throw ast::InternalError(formatError(_("%s: Unable to evaluate argument.\n"), name.data()), 999, in->getLocation());
                }

                // shared by derived classes, kept alive
                pIT->IncreaseRef();
                validator.inputs.push_back({-1, pIT});
            }
        }
        else
        {
            throw ast::InternalError(formatError(_("%s: argument must be constant expression.\n"), name.data()), 999, v->getLocation());
        }

        // error message arguments: constant content or the property value
        validator.error = getPropertyErrorValidator(vname);
        for (auto&& e : getErrorArgs(vname))
        {
            int idx = std::abs(std::get<0>(e));
            if (idx >= (int)validator.inputs.size())
            {
                continue;
            }

            InternalType* pIT = std::get<1>(validator.inputs[idx]);
            validator.errorArgs.push_back(pIT ? std::make_tuple(-1, scilab::UTF8::toUTF8(var2str(pIT))) : std::make_tuple(0, std::string()));
        }

        arg.validators.push_back(validator);
    }
}

InternalType* Classdef::checkProperty(const std::wstring& name, InternalType* value)
{
    auto p = properties.find(name);
    if (p == properties.end())
    {
        return value;
    }

    const ARG& arg = std::get<0>(p->second).arg;
    std::string cls = scilab::UTF8::toUTF8(getName());
    std::string prop = scilab::UTF8::toUTF8(name);
    InternalType* cur = value;
    try
    {
        // check size + expand + transpose
        if (arg.dimsConvertor)
        {
            InternalType* x = arg.dimsConvertor(cur);
            if (x == nullptr)
            {
                throw ast::InternalError(formatError(_("%s: Wrong size of property '%s': %ls expected.\n"), cls.data(), prop.data(), arg.dimsStr().c_str()));
            }

            cur = x;
        }

        for (auto&& convertor : arg.convertors)
        {
            InternalType* x = convertor.convertor(cur);
            if (x && x != cur)
            {
                if (cur != value)
                {
                    cur->killMe();
                }

                cur = x;
            }
        }

        for (auto&& validator : arg.validators)
        {
            typed_list args;
            for (auto&& input : validator.inputs)
            {
                args.push_back(std::get<0>(input) == 0 ? cur : std::get<1>(input));
            }

            if (validator.validator(args) == false)
            {
                std::string strs[3];
                for (int k = 0; k < validator.errorArgs.size() && k < 3; ++k)
                {
                    strs[k] = std::get<0>(validator.errorArgs[k]) == 0 ? scilab::UTF8::toUTF8(var2str(cur)) : std::get<1>(validator.errorArgs[k]);
                }

                throw ast::InternalError(formatError(_(std::get<0>(validator.error).data()), cls.data(), prop.data(), strs[0].data(), strs[1].data(), strs[2].data()));
            }
        }
    }
    catch (const ast::InternalError&)
    {
        if (cur != value)
        {
            cur->killMe();
        }

        throw;
    }

    return cur;
}

AccessModifier Classdef::reduceAccess(AccessModifier access)
{
    // protected members stay visible to all derived classes, private ones are not inherited
    switch (access)
    {
        case AccessModifier::PRIVATE:
            access = AccessModifier::NONE;
            break;
        default: {}
    }

    return access;
}

void Classdef::addHelpers()
{
    // add undefined useful overload
    //addHelper(L"disp", &Classdef::object_disp);
    addHelper(L"outline", &Classdef::object_outline);
    addHelper(L"eq", &Classdef::object_eq);
    addHelper(L"ne", &Classdef::object_ne);
    addHelper(L"clone", &Classdef::object_clone);
    addHelper(L"delete", &Classdef::object_delete);
}

void Classdef::addHelper(const std::wstring& name, Function::ReturnValue (Classdef::*_pFunc)(typed_list&, int, typed_list&))
{
    auto m = methods.find(name);
    if (m == methods.end() || std::get<0>((*m).second).hidden == true)
    {
        if (m != methods.end())
        {
            /*
            OBJ_ATTR attr = std::get<0>((*m).second);
            if (attr.callable != nullptr)
            {
                attr.callable->DecreaseRef();
                attr.callable->killMe();
            }
            */
        }

        OBJ_ATTR attr;
        attr.access = AccessModifier::PUBLIC;
        attr.isStatic = false;
        attr.hidden = true;
        Function* f = Function::createFunction(name, this, _pFunc, getName());
        f->IncreaseRef();
        attr.callable = f;
        methods[name] = {attr, this};
    }
}

/* gateways for default overload*/
Function::ReturnValue Classdef::object_disp(typed_list& in, int _iRetCount, typed_list& out)
{
    return Function::OK;
}

Function::ReturnValue Classdef::object_outline(typed_list& in, int _iRetCount, typed_list& out)
{
    std::wostringstream ostr;
    ostr << " (" << getName() << " object)";
    out.push_back(new String(ostr.str().data()));
    return Function::OK;
}

Function::ReturnValue Classdef::object_eq(typed_list& in, int _iRetCount, typed_list& out)
{
    // sciprint("Object::object_eq\n");
    out.push_back(new Bool(in[0] == in[1]));
    return Function::OK;
}

Function::ReturnValue Classdef::object_ne(typed_list& in, int _iRetCount, typed_list& out)
{
    // sciprint("Object::object_ne\n");
    out.push_back(new Bool(in[0] != in[1]));
    return Function::OK;
}

Function::ReturnValue Classdef::object_clone(typed_list& in, int _iRetCount, typed_list& out)
{
    // sciprint("Object::object_clone\n");
    InternalType* obj = symbol::Context::getInstance()->getCurrentObject();
    if (obj && obj->isObject())
    {
        out.push_back(new Object(*obj->getAs<Object>()));
        return Function::OK;
    }

    return Function::Error;
}

Function::ReturnValue Classdef::object_delete(typed_list& in, int _iRetCount, typed_list& out)
{
    // sciprint("Object::object_delete\n");
    return Function::OK;
}

} // namespace types