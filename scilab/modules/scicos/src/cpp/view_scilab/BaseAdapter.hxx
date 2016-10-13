/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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

#include "view_scilab/Adapters.hxx"
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


    property(const std::wstring& prop, getter_t g, setter_t s) : original_index(fields.size()), name(prop), get(g), set(s) {};
    ~property() {};

    size_t original_index;
    std::wstring name;
    getter_t get;
    setter_t set;

    bool operator< (const std::wstring& v) const
    {
        return name < v;
    }

    static bool original_index_cmp(property<Adaptor> p1, property<Adaptor> p2)
    {
        return p1.original_index < p2.original_index;
    }

    /*
     * Static properties accessors
     */
    static props_t fields;

    /**
     * @return true if the properties have already been setup, false otherwise.
     */
    static bool properties_have_not_been_set()
    {
        return fields.empty();
    }

    /**
     * Add a property to an Adaptor
     */
    static void add_property(const std::wstring& name, getter_t g, setter_t s)
    {
        property<Adaptor>::props_t_it pos = std::lower_bound(fields.begin(), fields.end(), name);
        fields.insert(pos, property(name, g, s));
    }
};

/**
 * Base definition of the adapter pattern, implement the get / set dispatch.
 *
 * Note that sub-classes are responsible to fill the fields accordingly to theirs interfaces.
 */
template<typename Adaptor, typename Adaptee>
class BaseAdapter : public types::UserType
{

public:
    BaseAdapter() : m_adaptee(nullptr) {};
    BaseAdapter(const Controller& /*c*/, Adaptee* adaptee) : m_adaptee(adaptee) {}
    BaseAdapter(const BaseAdapter& adapter) : BaseAdapter(adapter, true) {}
    BaseAdapter(const BaseAdapter& adapter, bool cloneChildren) : m_adaptee(nullptr)
    {
        Controller controller;
        ScicosID id = controller.cloneObject(adapter.getAdaptee()->id(), cloneChildren, true);
        m_adaptee = controller.getObject< Adaptee >(id);
    };
    ~BaseAdapter()
    {
        if (m_adaptee != nullptr)
        {
            Controller controller;
            controller.deleteObject(m_adaptee->id());
        }
    };

    /*
     * property accessors
     */

    bool hasProperty(const std::wstring& _sKey) const
    {
        typename property<Adaptor>::props_t_it found = std::lower_bound(property<Adaptor>::fields.begin(), property<Adaptor>::fields.end(), _sKey);
        return found != property<Adaptor>::fields.end() && !(_sKey < found->name);
    }

    types::InternalType* getProperty(const std::wstring& _sKey, Controller controller = Controller()) const
    {
        typename property<Adaptor>::props_t_it found = std::lower_bound(property<Adaptor>::fields.begin(), property<Adaptor>::fields.end(), _sKey);
        if (found != property<Adaptor>::fields.end() && !(_sKey < found->name))
        {
            return found->get(static_cast<Adaptor*>(this), controller);
        }
        return 0;
    }

    bool setProperty(const std::wstring& _sKey, types::InternalType* v, Controller controller = Controller())
    {
        typename property<Adaptor>::props_t_it found = std::lower_bound(property<Adaptor>::fields.begin(), property<Adaptor>::fields.end(), _sKey);
        if (found != property<Adaptor>::fields.end() && !(_sKey < found->name))
        {
            return found->set(*static_cast<Adaptor*>(this), v, controller);
        }
        return false;
    }

    /**
     * property as TList accessors
     */

    types::InternalType* getAsTList(types::TList* tlist, const Controller& controller)
    {
        typename property<Adaptor>::props_t properties = property<Adaptor>::fields;
        std::sort(properties.begin(), properties.end(), property<Adaptor>::original_index_cmp);

        // create the header
        types::String* header = new types::String(1, 1 + (int)properties.size());
        header->set(0, Adaptor::getSharedTypeStr().c_str());
        int index = 1;
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it, ++index)
        {
            header->set(index, it->name.c_str());
        }
        tlist->append(header);

        // set the tlist field value
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it)
        {
            types::InternalType* field = it->get(*static_cast<Adaptor*>(this), controller);
            tlist->append(field);
            if (field->isList())
            {
                field->killMe();
            }
        }

        return tlist;
    }

    bool setAsTList(types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabTList && v->getType() != types::InternalType::ScilabMList)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: Tlist or Mlist expected.\n"), Adaptor::getSharedTypeStr().c_str());
            return false;
        }
        types::TList* current = v->getAs<types::TList>();
        // The input TList cannot be empty
        if (current->getSize() < 1)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong length for field %s: at least %d element expected.\n"), Adaptor::getSharedTypeStr().c_str(), 1);
            return false;
        }

        // Check the header
        types::String* header = current->getFieldNames();
        if (header->getSize() < 1)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong length for header of field %s: at least %d element expected.\n"), Adaptor::getSharedTypeStr().c_str(), 1);
            return false;
        }
        // Make sure it is the same type as the Adapter
        if (header->get(0) != Adaptor::getSharedTypeStr())
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong value for header of field %s: %s expected.\n"), Adaptor::getSharedTypeStr().c_str(), Adaptor::getSharedTypeStr().c_str());
            return false;
        }

        // Retrieve the Adapter's properties
        typename property<Adaptor>::props_t properties = property<Adaptor>::fields;

        // For each input property, if it corresponds to an Adapter's property, set it.
        for (int index = 1; index < header->getSize(); ++index)
        {
            typename property<Adaptor>::props_t_it found = std::lower_bound(properties.begin(), properties.end(), header->get(index));
            if (found != properties.end() && !(header->get(index) < found->name))
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

        typename property<Adaptor>::props_t properties = property<Adaptor>::fields;
        std::sort(properties.begin(), properties.end(), property<Adaptor>::original_index_cmp);

        types::Bool* ret = new types::Bool(1, 1 + properties.size());
        ret->set(0, true); // First field is just the Adapter's name, which has been checked by the above conditions

        Controller controller;
        int index = 1;
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it, ++index)
        {
            types::InternalType* ith_prop1 = it->get(*static_cast<Adaptor*>(this), controller);
            types::InternalType* ith_prop2 = it->get(*static_cast<Adaptor*>(ut), controller);
            ret->set(index, *ith_prop1 == *ith_prop2);

            // Getting a property allocates data, so free it
            ith_prop1->killMe();
            ith_prop2->killMe();
        }

        return ret;
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

    virtual std::wstring getTypeStr() = 0;
    virtual std::wstring getShortTypeStr() = 0;

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

    bool extract(const std::wstring & name, types::InternalType *& out) override final
    {
        typename property<Adaptor>::props_t_it found = std::lower_bound(property<Adaptor>::fields.begin(), property<Adaptor>::fields.end(), name);
        if (found != property<Adaptor>::fields.end() && !(name < found->name))
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

                    // Sort the properties before extracting them
                    typename property<Adaptor>::props_t properties = property<Adaptor>::fields;
                    std::sort(properties.begin(), properties.end(), property<Adaptor>::original_index_cmp);

                    // Allocate the return
                    types::String* pOut = new types::String(1, static_cast<int>(properties.size()));

                    int i = 0;
                    for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it, ++i)
                    {
                        pOut->set(i, it->name.data());
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
                types::String* pStr = (*_pArgs)[i]->getAs<types::String>();
                std::wstring name = pStr->get(0);

                Controller controller;
                typename property<Adaptor>::props_t_it found = std::lower_bound(property<Adaptor>::fields.begin(), property<Adaptor>::fields.end(), name);
                if (found != property<Adaptor>::fields.end() && !(name < found->name))
                {
                    found->set(*static_cast<Adaptor*>(this), _pSource, controller);
                }

                return this;
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
        typename property<Adaptor>::props_t properties = property<Adaptor>::fields;
        std::sort(properties.begin(), properties.end(), property<Adaptor>::original_index_cmp);

        ostr << L"scicos_" <<  getTypeStr() << L" type :" << '\n';
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it)
        {
            ostr << L"  " << it->name << '\n';
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
