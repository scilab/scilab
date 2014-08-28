/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef BASEADAPTER_HXX_
#define BASEADAPTER_HXX_

#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

#include "user.hxx"
#include "internal.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "Adapters.hxx"
#include "model/BaseObject.hxx"

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
    BaseAdapter(Adaptee* o) : adaptee(o) {};
    BaseAdapter(const BaseAdapter& o) : adaptee(o.adaptee) {};
    virtual ~BaseAdapter() {};

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
        types::String* header = new types::String(1 + (int)properties.size(), 1);
        header->set(0, Adaptor::getSharedTypeStr().c_str());
        int index = 1;
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it, ++index)
        {
            header->set(index, it->name.c_str());
        }
        tlist->set(0, header);

        // set the tlist field value
        index = 1;
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it, ++index)
        {
            tlist->set(index, it->get(*static_cast<Adaptor*>(this), controller));
        }

        return tlist;
    }

    bool setAsTList(types::InternalType* v, Controller& controller)
    {
        typename property<Adaptor>::props_t properties = property<Adaptor>::fields;
        std::sort(properties.begin(), properties.end(), property<Adaptor>::original_index_cmp);

        if (v->getType() != types::InternalType::ScilabTList && v->getType() != types::InternalType::ScilabMList)
        {
            return false;
        }
        types::TList* current = v->getAs<types::TList>();
        if (current->getSize() != static_cast<int>(1 + properties.size()))
        {
            return false;
        }

        // check the header
        types::String* header = current->getFieldNames();
        if (header->getSize() != static_cast<int>(1 + properties.size()))
        {
            return false;
        }
        if (header->get(0) != Adaptor::getSharedTypeStr())
        {
            return false;
        }
        int index = 1;
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it, ++index)
        {
            if (header->get(index) != it->name)
            {
                return false;
            }
        }

        // this is a valid tlist, get each tlist field value and pass it to the right property decoder
        index = 1;
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it, ++index)
        {
            // DEBUG LOG:
            // std::wcerr << Adaptor::getSharedTypeStr() << L" set " << it->name << std::endl;

            bool status = it->set(*static_cast<Adaptor*>(this), current->get(index), controller);
            if (!status)
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @return the Adaptee instance
     */
    Adaptee* getAdaptee() const
    {
        return adaptee;
    };

    /**
     * set the adaptee
     */
    void setAdaptee(Adaptee* adaptee)
    {
        this->adaptee = adaptee;
    }

    /*
     * All following methods should be implemented by each template instance
     */

    std::wstring getTypeStr()
    {
        return L"ba";
    }

    std::wstring getShortTypeStr()
    {
        return L"BaseAdapter";
    }

    types::InternalType* clone()
    {
        return new Adaptor(*static_cast<Adaptor*>(this));
    }

    /*
     * Implement a specific types::User
     */

    bool isAssignable()
    {
        return true;
    }

    bool extract(const std::wstring & name, types::InternalType *& out)
    {
        typename property<Adaptor>::props_t_it found = std::lower_bound(property<Adaptor>::fields.begin(), property<Adaptor>::fields.end(), name);
        if (found != property<Adaptor>::fields.end() && !(name < found->name))
        {
            Controller controller = Controller();
            types::InternalType* value = found->get(*static_cast<Adaptor*>(this), controller);
            if (value == 0)
            {
                return false;
            }

            out = value;
            return true;
        }
        return false;
    }

    types::InternalType* extract(types::typed_list* _pArgs)
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
            // TO DO : management other type for arguments like a scalar or matrix of double
        }

        return NULL;
    }

    types::InternalType* insert(types::typed_list* _pArgs, InternalType* _pSource)
    {
        for (int i = 0; i < _pArgs->size(); i++)
        {
            if ((*_pArgs)[i]->isString())
            {
                types::String* pStr = (*_pArgs)[i]->getAs<types::String>();
                std::wstring name = pStr->get(0);
                Controller controller = Controller();
                typename property<Adaptor>::props_t_it found = std::lower_bound(property<Adaptor>::fields.begin(), property<Adaptor>::fields.end(), name);
                if (found != property<Adaptor>::fields.end() && !(name < found->name))
                {
                    found->set(*static_cast<Adaptor*>(this), _pSource, controller);
                }

                return this;
            }
            //else if(_pArgs[i]->isDouble())
            //{
            //
            //}
            else
            {
                return NULL;
            }
        }

        // call overload
        return NULL;
    }

    void whoAmI(void)
    {
        std::cout << "scicos object";
    }

    bool hasToString()
    {
        // allow scilab to call toString of this class
        return true;
    }

    bool toString(std::wostringstream& ostr)
    {
        typename property<Adaptor>::props_t properties = property<Adaptor>::fields;
        std::sort(properties.begin(), properties.end(), property<Adaptor>::original_index_cmp);

        ostr << L"scicos_" <<  getTypeStr() << L" type :" << std::endl;
        for (typename property<Adaptor>::props_t_it it = properties.begin(); it != properties.end(); ++it)
        {
            ostr << L"  " << it->name << std::endl;
        }
        return true;
    }


private:
    Adaptee* adaptee;
};


} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* BASEADAPTER_HXX_ */
