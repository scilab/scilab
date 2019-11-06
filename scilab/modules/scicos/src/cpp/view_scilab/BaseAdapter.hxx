/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2017 - ESI Group - Clement DAVID
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

#ifndef BASEADAPTER_HXX_
#define BASEADAPTER_HXX_

#include <cstring>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <sstream>

#include "bool.hxx"
#include "double.hxx"
#include "user.hxx"
#include "int.hxx"
#include "internal.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "callable.hxx"
#include "overload.hxx"
#include "configvariable.hxx"
#include "exp.hxx"
#include "types_comparison_eq.hxx"

#include "view_scilab/Adapters.hxx"
#include "view_scilab/AdapterView.hxx"
#include "controller_helpers.hxx"
#include "utilities.hxx"
#include "Controller.hxx"
#include "model/BaseObject.hxx"

extern "C" {
#include "localization.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/**
 * A property used as a getter/setter for a specific name
 */
template<typename Adaptor>
struct property
{
public:
    typedef types::InternalType* (*getter_t)(const Adaptor& adaptor, const Controller& controller);
    typedef bool (*setter_t)(Adaptor& adaptor, types::InternalType* v, Controller& controller);

    typedef std::vector< property<Adaptor> > props_t;
    typedef typename props_t::iterator props_t_it;

    /*
     * Static properties accessors
     */
    static props_t fields;

    property(const std::wstring& prop, getter_t g, setter_t s) : original_index(fields.size()), name(prop), get(g), set(s) {};
    property(const property& p) :
        original_index(p.original_index),
        name(p.name),
        get(p.get),
        set(p.set)
    {};
    ~property() {};
    property(property&& p) :
        original_index(std::move(p.original_index)),
        name(std::move(p.name)),
        get(std::move(p.get)),
        set(std::move(p.set))
    {};
    property<Adaptor>& operator= (property<Adaptor>&& p)
    {
        original_index = std::move(p.original_index);
        name = std::move(p.name);
        get = std::move(p.get);
        set = std::move(p.set);
        return *this;
    };

    int original_index;
    std::wstring name;
    getter_t get;
    setter_t set;

public:

    /**
     * @return true if the properties have already been setup, false otherwise.
     */
    static bool properties_have_not_been_set()
    {
        return fields.empty();
    }

    /** reserve an amount of property */
    static void reserve_properties(size_t count)
    {
        fields.reserve(count);
    }

    /** compact the properties and setup for a fast lookup */
    static void shrink_to_fit()
    {
        fields.shrink_to_fit();
        std::sort(fields.begin(), fields.end());
    }

    /** add a property to an Adaptor */
    static void add_property(const std::wstring& name, getter_t g, setter_t s)
    {
        fields.emplace_back(property(name, g, s));
    }

    /** lookup for a name */
    static props_t_it find(const std::wstring& name)
    {
        props_t_it found = std::lower_bound(fields.begin(), fields.end(), name);
        if (found != fields.end() && *found == name)
        {
            return found;
        }
        else
        {
            return fields.end();
        }
    }
};

template<typename Adaptor>
inline bool
operator<(const property<Adaptor>& x, const property<Adaptor>& y)
{
    return x.name < y.name;
}

template<typename Adaptor>
inline bool
operator<(const property<Adaptor>& x, const std::wstring& y)
{
    return x.name < y;
}

template<typename Adaptor>
inline bool
operator==(const property<Adaptor>& x, const property<Adaptor>& y)
{
    return x.name == y.name;
}

template<typename Adaptor>
inline bool
operator==(const property<Adaptor>& x, const std::wstring& y)
{
    return x.name == y;
}

/**
 * Base definition of the adapter pattern, implement the get / set dispatch.
 *
 * Note that sub-classes are responsible to fill the fields accordingly to theirs interfaces.
 */
template<typename Adaptor, typename Adaptee>
class BaseAdapter : public types::UserType
{
private:
    using BaseObject = org_scilab_modules_scicos::model::BaseObject;

public:
    explicit BaseAdapter() : m_adaptee(nullptr) {}
    BaseAdapter(const Controller& /*c*/, Adaptee* adaptee) : m_adaptee(adaptee) {}
    BaseAdapter(const BaseAdapter& adapter) : BaseAdapter(adapter, true) {}
    BaseAdapter(const BaseAdapter& adapter, bool cloneChildren) : m_adaptee(nullptr)
    {
        if (adapter.getAdaptee() != nullptr)
        {
            AdapterView update_partial_information;
            Controller controller;

            Controller::cloned_t mapped;
            BaseObject* clone = controller.cloneBaseObject(mapped, adapter.getAdaptee(), cloneChildren, true);
            m_adaptee = static_cast<Adaptee*>(clone);
        }
    };
    BaseAdapter(const BaseAdapter&& adapter) : BaseAdapter(Controller(), adapter.m_adaptee) {}
    ~BaseAdapter()
    {
        if (m_adaptee != nullptr)
        {
            AdapterView update_partial_information;

            Controller controller;
            controller.deleteObject(m_adaptee->id());
        }
    };

    /*
     * property accessors
     */

    bool hasProperty(const std::wstring& _sKey) const
    {
        typename property<Adaptor>::props_t_it found = property<Adaptor>::find(_sKey);
        return found != property<Adaptor>::fields.end();
    }

    types::InternalType* getProperty(const std::wstring& _sKey, Controller controller = Controller()) const
    {
        typename property<Adaptor>::props_t_it found = property<Adaptor>::find(_sKey);
        if (found != property<Adaptor>::fields.end())
        {
            Adaptor& adapter = *static_cast<Adaptor*>(this);
            return found->get(adapter, controller);
        }
        return nullptr;
    }

    bool setProperty(const std::wstring& _sKey, types::InternalType* v, Controller controller = Controller())
    {
        typename property<Adaptor>::props_t_it found = property<Adaptor>::find(_sKey);
        if (found != property<Adaptor>::fields.end())
        {
            Adaptor& adapter = *static_cast<Adaptor*>(this);
            return found->set(adapter, v, controller);
        }
        return false;
    }

    void copyProperties(const Adaptor& adaptor, Controller controller = Controller())
    {
        for (const auto& p : property<Adaptor>::fields)
        {
            types::InternalType* pIT = p.get(adaptor, controller);
            p.set(*static_cast<Adaptor*>(this), pIT, controller);
            pIT->killMe();
        }
    }

    /**
     * property as TList accessors
     */

    types::InternalType* getAsTList(types::TList* tlist, const Controller& controller)
    {
        const typename property<Adaptor>::props_t& properties = property<Adaptor>::fields;

        // create the header
        types::String* header = new types::String(1, 1 + (int)properties.size());
        header->set(0, Adaptor::getSharedTypeStr().c_str());
        for (const auto& p : properties)
        {
            header->set(1 + p.original_index, p.name.c_str());

            types::InternalType* field = p.get(*static_cast<Adaptor*>(this), controller);
            tlist->set(1 + p.original_index, field);
            if (field->isList())
            {
                field->killMe();
            }

        }
        tlist->set(0, header);

        return tlist;
    }

    bool setAsTList(types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabTList && v->getType() != types::InternalType::ScilabMList)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %ls: Tlist or Mlist expected.\n"), Adaptor::getSharedTypeStr().c_str());
            return false;
        }
        types::TList* current = v->getAs<types::TList>();
        // The input TList cannot be empty
        if (current->getSize() < 1)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong length for field %ls: at least %d element expected.\n"), Adaptor::getSharedTypeStr().c_str(), 1);
            return false;
        }

        // Check the header
        types::String* header = current->getFieldNames();
        if (header->getSize() < 1)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong length for header of field \"%ls\": at least %d element expected.\n"), Adaptor::getSharedTypeStr().c_str(), 1);
            return false;
        }
        // Make sure it is the same type as the Adapter
        if (header->get(0) != Adaptor::getSharedTypeStr())
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong value for header of field \"%ls\": \"%ls\" expected.\n"), Adaptor::getSharedTypeStr().c_str(), Adaptor::getSharedTypeStr().c_str());
            return false;
        }

        // Retrieve the Adapter's properties
        const typename property<Adaptor>::props_t& properties = property<Adaptor>::fields;

        // For each input property, if it corresponds to an Adapter's property, set it.
        for (int index = 1; index < header->getSize(); ++index)
        {
            std::wstring name(header->get(index));
            typename property<Adaptor>::props_t_it found = property<Adaptor>::find(name);
            if (found != properties.end())
            {
                bool status = found->set(*static_cast<Adaptor*>(this), current->get(index), controller);
                if (!status)
                {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * property comparison
     */

    bool operator==(const types::InternalType& o) override final
    {

        const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(o.getShortTypeStr());
        // Check that 'ut' is an Adapter of the same type as *this
        if (adapter_index == Adapters::INVALID_ADAPTER)
        {
            return false;
        }
        if (this->getTypeStr() != o.getTypeStr())
        {
            return false;
        }

        const typename property<Adaptor>::props_t& properties = property<Adaptor>::fields;

        bool internal_equal;
        Controller controller;
        for (const auto& p : properties)
        {
            types::InternalType* ith_prop1 = p.get(*static_cast<const Adaptor*>(this), controller);
            types::InternalType* ith_prop2 = p.get(*static_cast<const Adaptor*>(&o), controller);

            // loop while the inner types are equals
            internal_equal = *ith_prop1 == *ith_prop2;

            // Getting a property allocates data, so free it
            ith_prop1->killMe();
            ith_prop2->killMe();

            if (!internal_equal)
            {
                return false;
            }
        }

        return true;
    }

    types::Bool* equal(types::UserType*& ut) override final
    {
        const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(ut->getShortTypeStr());
        // Check that 'ut' is an Adapter of the same type as *this
        if (adapter_index == Adapters::INVALID_ADAPTER)
        {
            return new types::Bool(false);
        }
        if (this->getTypeStr() != ut->getTypeStr())
        {
            return new types::Bool(false);
        }

        const typename property<Adaptor>::props_t& properties = property<Adaptor>::fields;

        types::Bool* ret = new types::Bool(1, 1 + (int)properties.size());
        ret->set(0, true); // First field is just the Adapter's name, which has been checked by the above conditions

        Controller controller;
        for (const auto& p : properties)
        {
            types::InternalType* ith_prop1 = p.get(*static_cast<Adaptor*>(this), controller);
            types::InternalType* ith_prop2 = p.get(*static_cast<Adaptor*>(ut), controller);
            ret->set(p.original_index + 1, *ith_prop1 == *ith_prop2);

            // Getting a property allocates data, so free them
            ith_prop1->killMe();
            ith_prop2->killMe();
        }

        return ret;
    }

    /**
     * Return a default constructed Scilab value
     */
    template<typename T>
    static types::InternalType* default_value()
    {
        T* o = new T();
        o->IncreaseRef();
        return o;
    }

    /**
     * Increase reference count to store a Scilab value
     */
    template<typename T>
    static T* reference_value(T* o)
    {
        o->IncreaseRef();
        return o;
    }


    /**
     * @return the Adaptee
     */
    Adaptee* getAdaptee() const
    {
        return m_adaptee;
    }

    /*
     * All following methods should be implemented by each template instance
     */

    virtual std::wstring getTypeStr() const override = 0;
    virtual std::wstring getShortTypeStr() const override = 0;

private:
    virtual UserType* clone() override final
    {
        return new Adaptor(*static_cast<Adaptor*>(this));
    }

    /*
     * Implement a specific types::User
     */

    bool isAssignable() override final
    {
        return true;
    }

    // sb.model.rpar.contrib will return a reference to contrib
    bool isContainer() override final
    {
        return true;
    }

    bool extract(const std::wstring & name, types::InternalType *& out) override final
    {
        typename property<Adaptor>::props_t_it found = property<Adaptor>::find(name);
        if (found != property<Adaptor>::fields.end())
        {
            Controller controller;
            types::InternalType* value = found->get(*static_cast<Adaptor*>(this), controller);
            if (value == 0)
            {
                return false;
            }

            out = value;
            return true;
        }

        // specific case : to ease debugging let the user retrieve the model ID
        if (name == L"modelID")
        {
            out = new types::Int64(m_adaptee->id());
            return true;
        }

        return false;
    }

    types::InternalType* extract(types::typed_list* _pArgs) override final
    {
        if (_pArgs->size() == 0)
        {
            // call overload
            return NULL;
        }

        if ((*_pArgs)[0]->isString())
        {
            types::String* pStr = (*_pArgs)[0]->getAs<types::String>();
            types::InternalType* pOut = NULL;
            extract(std::wstring(pStr->get(0)), pOut);
            return pOut;
        }
        else
        {
            if ((*_pArgs)[0]->isDouble())
            {
                types::Double* index = (*_pArgs)[0]->getAs<types::Double>();

                if (index->get(0) == 1)
                {
                    // When _pArgs is '1', return the list of the property names of the Adaptor

                    const typename property<Adaptor>::props_t& properties = property<Adaptor>::fields;

                    // Allocate the return
                    types::String* pOut = new types::String(1, static_cast<int>(properties.size()));

                    for (const auto& p : properties)
                    {
                        pOut->set(p.original_index, p.name.data());
                    }
                    return pOut;
                }
            }
            else
            {
                // TO DO : management other type for arguments like a scalar or matrix of double
            }
        }

        return NULL;
    }

    types::UserType* insert(types::typed_list* _pArgs, types::InternalType* _pSource) override final
    {
        for (size_t i = 0; i < _pArgs->size(); i++)
        {
            if ((*_pArgs)[i]->isString())
            {
                Controller controller;

                types::String* pStr = (*_pArgs)[i]->getAs<types::String>();

                Adaptor* work;
                if (getAdaptee()->refCount() > 0)
                {
                    // clone()
                    work = new Adaptor(*static_cast<Adaptor*>(this));
                }
                else
                {
                    work = static_cast<Adaptor*>(this);
                }

                typename property<Adaptor>::props_t_it found = property<Adaptor>::find(std::wstring(pStr->get(0)));
                if (found != property<Adaptor>::fields.end())
                {
                    found->set(*work, _pSource, controller);
                }

                return work;
            }
            else
            {
                return NULL;
            }
        }

        // call overload
        return NULL;
    }

    void whoAmI(void) override
    {
        std::cout << "scicos object";
    }

    bool hasToString() override final
    {
        // Do not allow scilab to call toString of this class
        return false;
    }

    bool toString(std::wostringstream& ostr) override final
    {
        // Deprecated, use the overload instead
        const typename property<Adaptor>::props_t& properties = property<Adaptor>::fields;

        ostr << L"scicos_" <<  getTypeStr() << L" type :" << '\n';
        for (const auto& p : properties)
        {
            ostr << L"  " << p.name << '\n';
        }
        return true;
    }

    bool isInvokable() const override final
    {
        return true;
    }

    bool invoke(types::typed_list & in, types::optional_list & /*opt*/, int /*_iRetCount*/, types::typed_list & out, const ast::Exp & e) override final
    {
        if (in.size() == 0)
        {
            out.push_back(this);
            return true;
        }
        else if (in.size() == 1)
        {
            types::InternalType* _out = nullptr;
            types::InternalType*  arg = in[0];
            if (arg->isString())
            {
                types::String* pString = arg->getAs<types::String>();
                for (int i = 0; i < pString->getSize(); ++i)
                {
                    if (!extract(pString->get(i), _out))
                    {
                        return false;
                    }
                    out.push_back(_out);
                }
            }

            if (!out.empty())
            {
                return true;
            }
        }

        types::Callable::ReturnValue ret;
        // Overload of extraction needs the BaseAdapter from where we extract
        this->IncreaseRef();
        in.push_back(this);

        try
        {
            ret = Overload::call(L"%" + getShortTypeStr() + L"_e", in, 1, out);
        }
        catch (ast::InternalError & /*se*/)
        {
            ret = Overload::call(L"%l_e", in, 1, out);
        }

        // Remove this from "in" to keep "in" unchanged.
        this->DecreaseRef();
        in.pop_back();

        if (ret == types::Callable::Error)
        {
            throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        return true;
    }

private:
    Adaptee* m_adaptee;
};


} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* BASEADAPTER_HXX_ */
