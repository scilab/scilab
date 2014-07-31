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
#include <cassert>

#include "list.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "GraphicsAdapter.hxx"
#include "ports_management.hxx"

extern "C" {
#include "sci_malloc.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

static const wchar_t E[] = L"E";
static const wchar_t I[] = L"I";

struct orig
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> geom;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        data[0] = geom[0];
        data[1] = geom[1];

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        geom[0] = current->get(0);
        geom[1] = current->get(1);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);
        return true;
    }
};

struct sz
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> geom;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        data[0] = geom[2];
        data[1] = geom[3];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        geom[2] = current->get(0);
        geom[3] = current->get(1);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);
        return true;
    }
};

struct flip
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        int* data;
        types::Bool* o = new types::Bool(1, 1, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> angle;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        data[0] = static_cast<int>(angle[0]);
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabBool)
        {
            return false;
        }

        types::Bool* current = v->getAs<types::Bool>();
        if (current->isScalar() != true)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> angle;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        angle[0] = (current->get(0) == false) ? 0 : 1;

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);
        return true;
    }
};

struct theta
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 1, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> angle;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        data[0] = angle[1];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->isScalar() != true)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> angle;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        angle[1] = current->get(0);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);
        return true;
    }
};

struct exprs
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<std::string> exprs;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);

        types::String* o = new types::String(exprs.size(), 1);
        for (size_t i = 0; i < exprs.size(); ++i)
        {
            o->set(i, exprs[i].data());
        }

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getCols() != 0 && current->getCols() != 1)
            {
                return false;
            }

            model::Block* adaptee = adaptor.getAdaptee();
            std::vector<std::string> exprs = std::vector<std::string>(current->getSize());
            for (size_t i = 0; i < exprs.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(current->get(i));
                exprs[i] = std::string(c_str);
                FREE(c_str);
            }

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getRows() != 0 || current->getCols() != 0)
            {
                return false;
            }

            model::Block* adaptee = adaptor.getAdaptee();
            std::vector<std::string> exprs;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);
            return true;
        }
        return false;
    }
};

struct pin
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, INPUTS, controller, CONNECTED_SIGNALS);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_with_property(adaptor, INPUTS, controller, CONNECTED_SIGNALS, v);
    }
};

struct pout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, OUTPUTS, controller, CONNECTED_SIGNALS);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_with_property(adaptor, OUTPUTS, controller, CONNECTED_SIGNALS, v);
    }
};

struct pein
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, EVENT_INPUTS, controller, CONNECTED_SIGNALS);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_with_property(adaptor, EVENT_INPUTS, controller, CONNECTED_SIGNALS, v);
    }
};

struct peout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, EVENT_OUTPUTS, controller, CONNECTED_SIGNALS);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_with_property(adaptor, EVENT_OUTPUTS, controller, CONNECTED_SIGNALS, v);
    }
};

struct gr_i
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return adaptor.getGrIContent();
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        adaptor.setGrIContent(v->clone());
        return true;
    }
};

struct id
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string id;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), LABEL, id);

        types::String* o = new types::String(1, 1);
        o->set(0, id.data());

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
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
        std::string id;
        char* c_str = wide_string_to_UTF8(current->get(0));
        id = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), LABEL, id);
        return true;
    }
};

struct in_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, INPUTS, controller, IMPLICIT);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property(adaptor, INPUTS, controller, IMPLICIT, v);
    }
};

struct out_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, OUTPUTS, controller, IMPLICIT);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property(adaptor, OUTPUTS, controller, IMPLICIT, v);
    }
};

struct in_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, INPUTS, controller, STYLE);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property(adaptor, INPUTS, controller, STYLE, v);
    }
};

struct out_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, OUTPUTS, controller, STYLE);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property(adaptor, OUTPUTS, controller, STYLE, v);
    }
};

struct in_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, INPUTS, controller, LABEL);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property(adaptor, INPUTS, controller, LABEL, v);
    }
};

struct out_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property(adaptor, OUTPUTS, controller, LABEL);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property(adaptor, OUTPUTS, controller, LABEL, v);
    }
};

struct style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string style;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), STYLE, style);

        types::String* o;
        if (style.empty())
        {
            o = new types::String(0, 0);
        }
        else
        {
            o = new types::String(1, 1);
            o->set(0, style.data());
        }
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            std::string style;
            char* c_str = wide_string_to_UTF8(current->get(0));
            style = std::string(c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), STYLE, style);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            std::string style;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), STYLE, style);
            return true;
        }
        return false;
    }
};

template<> property<GraphicsAdapter>::props_t property<GraphicsAdapter>::fields = property<GraphicsAdapter>::props_t();

GraphicsAdapter::GraphicsAdapter(const GraphicsAdapter& o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(o),
    gr_i_content(o.gr_i_content->clone())
{}

GraphicsAdapter::GraphicsAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(o)
{
    if (property<GraphicsAdapter>::properties_has_not_been_set())
    {
        property<GraphicsAdapter>::fields.reserve(17);
        property<GraphicsAdapter>::add_property(L"orig", &orig::get, &orig::set);
        property<GraphicsAdapter>::add_property(L"sz", &sz::get, &sz::set);
        property<GraphicsAdapter>::add_property(L"flip", &flip::get, &flip::set);
        property<GraphicsAdapter>::add_property(L"theta", &theta::get, &theta::set);
        property<GraphicsAdapter>::add_property(L"exprs", &exprs::get, &exprs::set);
        property<GraphicsAdapter>::add_property(L"pin", &pin::get, &pin::set);
        property<GraphicsAdapter>::add_property(L"pout", &pout::get, &pout::set);
        property<GraphicsAdapter>::add_property(L"pein", &pein::get, &pein::set);
        property<GraphicsAdapter>::add_property(L"peout", &peout::get, &peout::set);
        property<GraphicsAdapter>::add_property(L"gr_i", &gr_i::get, &gr_i::set);
        property<GraphicsAdapter>::add_property(L"id", &id::get, &id::set);
        property<GraphicsAdapter>::add_property(L"in_implicit", &in_implicit::get, &in_implicit::set);
        property<GraphicsAdapter>::add_property(L"out_implicit", &out_implicit::get, &out_implicit::set);
        property<GraphicsAdapter>::add_property(L"in_style", &in_style::get, &in_style::set);
        property<GraphicsAdapter>::add_property(L"in_label", &in_label::get, &in_label::set);
        property<GraphicsAdapter>::add_property(L"out_label", &out_label::get, &out_label::set);
        property<GraphicsAdapter>::add_property(L"style", &style::get, &style::set);
    }

    gr_i_content = new types::List();
}

GraphicsAdapter::~GraphicsAdapter()
{
    delete gr_i_content;
}

bool GraphicsAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"GraphicsAdapter.hxx: Dunno what to display there" << std::endl;
    return true;
}

std::wstring GraphicsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring GraphicsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* GraphicsAdapter::getGrIContent() const
{
    return gr_i_content;
}

void GraphicsAdapter::setGrIContent(types::InternalType* v)
{
    delete gr_i_content;
    gr_i_content = v->clone();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
