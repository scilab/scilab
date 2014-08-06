/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef BASEADAPTER_HXX_
#define BASEADAPTER_HXX_

#include <algorithm>
#include <string>
#include <sstream>

#include "user.hxx"
#include "internal.hxx"
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
     * @return true if the properties has already been setup, false otherwise.
     */
    static bool properties_has_not_been_set()
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
class BaseAdapter : public types::User<Adaptor>
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

    virtual std::wstring getTypeStr() = 0;
    virtual std::wstring getShortTypeStr() = 0;

    /*
     * Implement a specific types::User
     */
private:
    types::InternalType* clone()
    {
        return new Adaptor(*static_cast<Adaptor*>(this));
    }

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

    void whoAmI(void)
    {
        std::cout << "scicos object";
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
