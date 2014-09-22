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

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "list.hxx"
#include "mlist.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "ModelAdapter.hxx"
#include "ports_management.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::wstring diagram (L"diagram");

struct sim
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        // First, extract the function Name
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
            types::Double* Api = new types::Double(static_cast<double>(api));
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
        types::Double* o = new types::Double((int)state.size(), 1, &data);

#ifdef _MSC_VER
        std::copy(state.begin(), state.end(), stdext::checked_array_iterator<double*>(data, state.size()));
#else
        std::copy(state.begin(), state.end(), data);
#endif
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

        std::vector<double> state (current->getSize());
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
        types::Double* o = new types::Double((int)dstate.size(), 1, &data);

#ifdef _MSC_VER
        std::copy(dstate.begin(), dstate.end(), stdext::checked_array_iterator<double*>(data, dstate.size()));
#else
        std::copy(dstate.begin(), dstate.end(), data);
#endif
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

        std::vector<double> dstate (current->getSize());
        std::copy(current->getReal(), current->getReal() + current->getSize(), dstate.begin());

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), DSTATE, dstate);
        return true;
    }
};

struct odstate
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) adaptor;
        (void) controller;

        // FIXME: implement as a scicos encoded list of values

        // Return a default empty list.
        return new types::List();
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
            // silent unused parameter warnings
            (void) adaptor;
            (void) v;
            (void) controller;

            // FIXME: implement as a scicos encoded list of values
            return false;
        }
    }
};

struct rpar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        if (children.empty())
        {
            std::vector<double> rpar;
            controller.getObjectProperty(adaptee->id(), adaptee->kind(), RPAR, rpar);

            double *data;
            types::Double* o = new types::Double((int)rpar.size(), 1, &data);
#ifdef _MSC_VER
            std::copy(rpar.begin(), rpar.end(), stdext::checked_array_iterator<double*>(data, rpar.size()));
#else
            std::copy(rpar.begin(), rpar.end(), data);
#endif
            return o;
        }
        else
        {
            types::MList* o = new types::MList();
            types::String* Diagram = new types::String(diagram.c_str());
            o->set(0, Diagram);

            // FIXME: return the full diagram contained in children
            return o;
        }
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getCols() != 0 && current->getCols() != 1)
            {
                return false;
            }

            std::vector<double> rpar (current->getSize());
            for (int i = 0; i < current->getSize(); ++i)
            {
                rpar[i] = current->get(i);
            }

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), RPAR, rpar);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabString)
        {
            // Allow Text blocs to define strings in rpar
            return true;
        }
        else
        {
            // FIXME: set rpar when input is a diagram (MList)
            return false;
        }
    }
};

static double toDouble(const int a)
{
    return static_cast<double>(a);
}

struct ipar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<int> ipar;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), IPAR, ipar);

        double *data;
        types::Double* o = new types::Double((int)ipar.size(), 1, &data);

#ifdef _MSC_VER
        std::transform(ipar.begin(), ipar.end(), stdext::checked_array_iterator<double*>(data, ipar.size()), toDouble);
#else
        std::transform(ipar.begin(), ipar.end(), data, toDouble);
#endif
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

        std::vector<int> ipar (current->getSize());
        for (int i = 0; i < current->getSize(); ++i)
        {
            if (floor(current->get(i)) != current->get(i))
            {
                return false;
            }
            ipar[i] = static_cast<int>(current->get(i));
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), IPAR, ipar);
        return true;
    }
};

struct opar
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) adaptor;
        (void) controller;

        // FIXME: implement as a scicos encoded list of values

        // Return a default empty list.
        return new types::List();
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
            // silent unused parameter warnings
            (void) adaptor;
            (void) v;
            (void) controller;

            // FIXME: implement as a scicos encoded list of values
            return false;
        }
    }
};

struct blocktype
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string type;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_BLOCKTYPE, type);

        types::String* o = new types::String(type.c_str());
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

        char* c_str = wide_string_to_UTF8(current->get(0));
        std::string type (c_str);
        FREE(c_str);

        // the value validation is performed on the model
        return controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_BLOCKTYPE, type) != FAIL;
    }
};

struct firing
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter, FIRING>(adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<ModelAdapter, FIRING>(adaptor, EVENT_OUTPUTS, controller, v);
    }
};

struct dep_ut
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<int> dep_ut;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_DEP_UT, dep_ut);

        int* dep;
        types::Bool* o = new types::Bool(1, 2, &dep);

        dep[0] = dep_ut[0];
        dep[1] = dep_ut[1];

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabBool)
        {
            return false;
        }

        types::Bool* current = v->getAs<types::Bool>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        std::vector<int> dep_ut (2);
        dep_ut[0] = current->get(0);
        dep_ut[1] = current->get(1);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_DEP_UT, dep_ut);
        return true;
    }
};

struct label
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string label;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), LABEL, label);

        types::String* o = new types::String(1, 1);
        o->set(0, label.data());

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();

        std::string label;
        char* c_str = wide_string_to_UTF8(current->get(0));
        label = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), LABEL, label);
        return true;
    }
};

struct nzcross
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        int nzcross;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), NZCROSS, nzcross);

        types::Double* o = new types::Double(static_cast<double>(nzcross));

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 1)
        {
            return false;
        }
        if (floor(current->get(0)) != current->get(0))
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();

        int nzcross = static_cast<int>(current->get(0));

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), NZCROSS, nzcross);
        return true;
    }
};

struct nmode
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        int nmode;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), NMODE, nmode);

        types::Double* o = new types::Double(static_cast<double>(nmode));

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 1)
        {
            return false;
        }
        if (floor(current->get(0)) != current->get(0))
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();

        int nmode = static_cast<int>(current->get(0));

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), NMODE, nmode);
        return true;
    }
};

struct equations
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) adaptor;
        (void) controller;

        // FIXME: implement as a stored modelica equations

        // Return a default empty list.
        return new types::List();
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
            // silent unused parameter warnings
            (void) adaptor;
            (void) v;
            (void) controller;

            // FIXME: implement as a stored modelica equations
            // FIXME: get the input list and store it in the equations field
            return false;
        }
    }
};

struct uid
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string uid;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), UID, uid);

        types::String* o = new types::String(1, 1);
        o->set(0, uid.data());

        return o;
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();

        std::string uid;
        char* c_str = wide_string_to_UTF8(current->get(0));
        uid = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), UID, uid);
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
    if (property<ModelAdapter>::properties_have_not_been_set())
    {
        property<ModelAdapter>::fields.reserve(23);
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
        property<ModelAdapter>::add_property(L"rpar", &rpar::get, &rpar::set);
        property<ModelAdapter>::add_property(L"ipar", &ipar::get, &ipar::set);
        property<ModelAdapter>::add_property(L"opar", &opar::get, &opar::set);
        property<ModelAdapter>::add_property(L"blocktype", &blocktype::get, &blocktype::set);
        property<ModelAdapter>::add_property(L"firing", &firing::get, &firing::set);
        property<ModelAdapter>::add_property(L"dep_ut", &dep_ut::get, &dep_ut::set);
        property<ModelAdapter>::add_property(L"label", &label::get, &label::set);
        property<ModelAdapter>::add_property(L"nzcross", &nzcross::get, &nzcross::set);
        property<ModelAdapter>::add_property(L"nmode", &nmode::get, &nmode::set);
        property<ModelAdapter>::add_property(L"equations", &equations::get, &equations::set);
        property<ModelAdapter>::add_property(L"uid", &uid::get, &uid::set);
    }
}

ModelAdapter::~ModelAdapter()
{
}

std::wstring ModelAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring ModelAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
