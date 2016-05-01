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

#include <cwchar>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "list.hxx"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "GraphicsAdapter.hxx"
#include "ports_management.hxx"
#include "controller_helpers.hxx"

#include "var2vec.hxx"
#include "vec2var.hxx"

extern "C" {
#include "localization.h"
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::wstring MBLOCK (L"MBLOCK");
const std::wstring MPBLOCK (L"MPBLOCK");
const std::wstring in (L"in");
const std::wstring intype (L"intype");
const std::wstring out (L"out");
const std::wstring outtype (L"outtype");
const std::wstring param (L"param");
const std::wstring paramv (L"paramv");
const std::wstring pprop (L"pprop");
const std::wstring nameF (L"nameF");
const std::wstring funtxt (L"funtxt");

struct orig
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        data[0] = geom[0];
        data[1] = geom[1];

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real matrix expected.\n"), "graphics", "orig");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 2)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: %d-by-%d expected.\n"), "graphics", "orig", 1, 2);
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        geom[0] = current->get(0);
        geom[1] = current->get(1);

        controller.setObjectProperty(adaptee, BLOCK, GEOMETRY, geom);
        return true;
    }
};

struct sz
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        data[0] = geom[2];
        data[1] = geom[3];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real matrix expected.\n"), "graphics", "sz");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 2)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: %d-by-%d expected.\n"), "graphics", "sz", 1, 2);
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee, BLOCK, GEOMETRY, geom);

        geom[2] = current->get(0);
        geom[3] = current->get(1);

        controller.setObjectProperty(adaptee, BLOCK, GEOMETRY, geom);
        return true;
    }
};

struct exprs
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> exprs;
        controller.getObjectProperty(adaptee, BLOCK, EXPRS, exprs);

        types::InternalType* res;
        if (!vec2var(exprs, res))
        {
            return 0;
        }
        return res;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        // Corner-case the content is an empty matrix
        if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (!current->isEmpty())
            {
                return false;
            }
        }

        std::vector<double> exprs;
        if (!var2vec(v, exprs))
        {
            return false;
        }
        controller.setObjectProperty(adaptee, BLOCK, EXPRS, exprs);
        return true;
    }
};

struct pin
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, INPUTS, controller, v);
    }
};

struct pout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, OUTPUTS, controller, v);
    }
};

struct pein
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_INPUTS, controller, v);
    }
};

struct peout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, EVENT_OUTPUTS, controller, v);
    }
};

struct gr_i
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getGrIContent();
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setGrIContent(v->clone());
        return true;
    }
};

struct id
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string id;
        controller.getObjectProperty(adaptee, BLOCK, DESCRIPTION, id);

        types::String* o = new types::String(1, 1);
        o->set(0, id.data());

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: string expected.\n"), "graphics", "id");
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (!current->isScalar())
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: %d-by-%d expected.\n"), "graphics", "id", 1, 1);
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();

        char* c_str = wide_string_to_UTF8(current->get(0));
        std::string id(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee, BLOCK, DESCRIPTION, id);
        return true;
    }
};

struct in_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, INPUTS, controller, v);
    }
};

struct out_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, IMPLICIT>(adaptor, OUTPUTS, controller, v);
    }
};

struct in_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, STYLE>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, STYLE>(adaptor, INPUTS, controller, v);
    }
};

struct out_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, STYLE>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, STYLE>(adaptor, OUTPUTS, controller, v);
    }
};

struct in_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, LABEL>(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, LABEL>(adaptor, INPUTS, controller, v);
    }
};

struct out_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<GraphicsAdapter, LABEL>(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_ports_property<GraphicsAdapter, LABEL>(adaptor, OUTPUTS, controller, v);
    }
};

struct style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string style;
        controller.getObjectProperty(adaptee, BLOCK, STYLE, style);

        return new types::String(style.c_str());
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (!current->isScalar())
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: %d-by-%d expected.\n"), "graphics", "style", 1, 1);
                return false;
            }

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string style(c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee, BLOCK, STYLE, style);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: string expected.\n"), "graphics", "style");
                return false;
            }

            std::string style;
            controller.setObjectProperty(adaptee, BLOCK, STYLE, style);
            return true;
        }

        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: string expected.\n"), "graphics", "style");
        return false;
    }
};

} /* namespace */

template<> property<GraphicsAdapter>::props_t property<GraphicsAdapter>::fields = property<GraphicsAdapter>::props_t();
static void initialize_fields()
{
    if (property<GraphicsAdapter>::properties_have_not_been_set())
    {
        property<GraphicsAdapter>::fields.reserve(16);
        property<GraphicsAdapter>::add_property(L"orig", &orig::get, &orig::set);
        property<GraphicsAdapter>::add_property(L"sz", &sz::get, &sz::set);
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
        property<GraphicsAdapter>::add_property(L"out_style", &out_style::get, &out_style::set);
        property<GraphicsAdapter>::add_property(L"in_label", &in_label::get, &in_label::set);
        property<GraphicsAdapter>::add_property(L"out_label", &out_label::get, &out_label::set);
        property<GraphicsAdapter>::add_property(L"style", &style::get, &style::set);
    }
}

GraphicsAdapter::GraphicsAdapter() :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(),
    gr_i_content(types::Double::Empty())
{
    initialize_fields();
}

GraphicsAdapter::GraphicsAdapter(const Controller& c, model::Block* adaptee) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    gr_i_content(types::Double::Empty())
{
    initialize_fields();
}

GraphicsAdapter::~GraphicsAdapter()
{
    gr_i_content->DecreaseRef();
    gr_i_content->killMe();
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
    gr_i_content->IncreaseRef();
    return gr_i_content;
}

void GraphicsAdapter::setGrIContent(types::InternalType* v)
{
    gr_i_content->DecreaseRef();
    gr_i_content->killMe();

    v->IncreaseRef();
    gr_i_content = v;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
