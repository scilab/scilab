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

#include <string>
#include <vector>
#include <algorithm>

#include "list.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "ModelAdapter.hxx"
#include "ports_management.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct sim
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        // First, extact the function Name
        std::string name;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
        types::String* Name = new types::String(1, 1);
        Name->set(0, name.data());

        // Then the Api. If it is zero, then just return the Name. Otherwise, return a list containing both.
        int api;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api);

        if (api == 0)
        {
            return Name;
        }
        else
        {
            types::Double* Api = new types::Double(1, 1, static_cast<double>(api));
            types::List* o = new types::List();
            o->set(0, Name);
            o->set(1, Api);
            return o;
        }
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string name = std::string(c_str);
            FREE(c_str);

            // If the input is a scalar string, then the functionApi is 0.
            int api = 0;

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api);
        }
        else if (v->getType() == types::InternalType::ScilabList)
        {
            // If the input is a 2-sized list, then it must be string and positive integer.
            types::List* current = v->getAs<types::List>();
            if (current->getSize() != 2)
            {
                return false;
            }
            if (current->get(0)->getType() != types::InternalType::ScilabString || current->get(1)->getType() != types::InternalType::ScilabDouble)
            {
                return false;
            }

            types::String* Name = current->get(0)->getAs<types::String>();
            if (Name->getSize() != 1)
            {
                return false;
            }
            char* c_str = wide_string_to_UTF8(Name->get(0));
            std::string name = std::string(c_str);
            FREE(c_str);

            types::Double* Api = current->get(1)->getAs<types::Double>();
            if (Api->getSize() != 1)
            {
                return false;
            }
            double api = Api->get(0);
            if (floor(api) != api)
            {
                return false;
            }
            int api_int = static_cast<int>(api);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api_int);
        }
        else
        {
            return false;
        }
        return true;
    }
};

struct in
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, INPUTS, controller, v);
    }
};

struct in2
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, INPUTS, controller, v);
    }
};

struct intyp
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, INPUTS, controller, v);
    }
};

struct out
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, OUTPUTS, controller, v);
    }
};

struct out2
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_COLS>(adaptor, OUTPUTS, controller, v);
    }
};

struct outtyp
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, DATATYPE_TYPE>(adaptor, OUTPUTS, controller, v);
    }
};

struct evtin
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_INPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_INPUTS, controller, v);
    }
};

struct evtout
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<ModelAdapter, DATATYPE_ROWS>(adaptor, EVENT_OUTPUTS, controller, v);
    }
};

struct state
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> state;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), STATE, state);

        double* data;
        types::Double* o = new types::Double(state.size(), 1, &data);

        std::copy(state.begin(), state.end(), data);

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getCols() != 0 && current->getCols() != 1)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> state;
        std::copy(current->getReal(), current->getReal() + current->getSize(), state.begin());

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), STATE, state);
        return true;
    }
};

struct dstate
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> dstate;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), DSTATE, dstate);

        double* data;
        types::Double* o = new types::Double(dstate.size(), 1, &data);

        std::copy(dstate.begin(), dstate.end(), data);

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getCols() != 0 && current->getCols() != 1)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> dstate;
        std::copy(current->getReal(), current->getReal() + current->getSize(), dstate.begin());

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), DSTATE, dstate);
        return true;
    }
};

struct odstate
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        // FIXME: get odstate
        return 0;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        types::List* current = v->getAs<types::List>();

        if (current->getSize() == 0)
        {
            return true;
        }
        else
        {
            // FIXME: get the input list and store it in the odstate field
            return false;
        }
    }
};

struct blocktype
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        int type;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_BLOCKTYPE, type);

        wchar_t Type = type;
        types::String* o = new types::String(&Type);

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            return false;
        }
        // The input must be a character
        if (current->get(0)[0] == '\0')
        {
            return false;
        }
        if (current->get(0)[1] != '\0')
        {
            return false;
        }

        int type = current->get(0)[0];

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_BLOCKTYPE, type);
        return true;
    }
};

} /* namespace */

template<> property<ModelAdapter>::props_t property<ModelAdapter>::fields = property<ModelAdapter>::props_t();

ModelAdapter::ModelAdapter(const ModelAdapter& o) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(o) { }

ModelAdapter::ModelAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(o)
{
    if (property<ModelAdapter>::properties_has_not_been_set())
    {
        property<ModelAdapter>::fields.reserve(13);
        property<ModelAdapter>::add_property(L"sim", &sim::get, &sim::set);
        property<ModelAdapter>::add_property(L"in", &in::get, &in::set);
        property<ModelAdapter>::add_property(L"in2", &in2::get, &in2::set);
        property<ModelAdapter>::add_property(L"intyp", &intyp::get, &intyp::set);
        property<ModelAdapter>::add_property(L"out", &out::get, &out::set);
        property<ModelAdapter>::add_property(L"out2", &out2::get, &out2::set);
        property<ModelAdapter>::add_property(L"outtyp", &outtyp::get, &outtyp::set);
        property<ModelAdapter>::add_property(L"evtin", &evtin::get, &evtin::set);
        property<ModelAdapter>::add_property(L"evtout", &evtout::get, &evtout::set);
        property<ModelAdapter>::add_property(L"state", &state::get, &state::set);
        property<ModelAdapter>::add_property(L"dstate", &dstate::get, &dstate::set);
        property<ModelAdapter>::add_property(L"odstate", &odstate::get, &odstate::set);
        property<ModelAdapter>::add_property(L"blocktype", &blocktype::get, &blocktype::set);
    }
}

ModelAdapter::~ModelAdapter()
{
}

bool ModelAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"ModelAdapter.hxx: Dunno what to display there" << std::endl;
    return true;
}

std::wstring ModelAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring ModelAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
