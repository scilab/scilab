/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
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
#include <algorithm>
#include <functional>

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

// shared informations for relinking across adapters hierarchy
std::map<ScicosID, std::vector<int> > partial_pin;
std::map<ScicosID, std::vector<int> > partial_pout;
std::map<ScicosID, std::vector<int> > partial_pein;
std::map<ScicosID, std::vector<int> > partial_peout;

struct orig
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> geom;
        controller.getObjectProperty(adaptee, GEOMETRY, geom);

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

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee, GEOMETRY, geom);

        geom[0] = current->get(0);
        geom[1] = current->get(1);

        controller.setObjectProperty(adaptee, GEOMETRY, geom);
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
        controller.getObjectProperty(adaptee, GEOMETRY, geom);

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

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> geom;
        controller.getObjectProperty(adaptee, GEOMETRY, geom);

        geom[2] = current->get(0);
        geom[3] = current->get(1);

        controller.setObjectProperty(adaptee, GEOMETRY, geom);
        return true;
    }
};

struct exprs
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> exprs;
        controller.getObjectProperty(adaptee, EXPRS, exprs);

        types::InternalType* res;
        if (!vec2var(exprs, res))
        {
            return 0;
        }
        return res;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

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
        controller.setObjectProperty(adaptee, EXPRS, exprs);
        return true;
    }
};

std::vector<int> cached_ports_init(std::map<ScicosID, std::vector<int> >& cache, model::Block* adaptee, const object_properties_t port_kind, const Controller& controller)
{
    auto it = cache.find(adaptee->id());
    if (it != cache.end())
    {
        // if already present, do not refresh it !
        return it->second;
    }

    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee, port_kind, ids);

    std::vector<ScicosID> children;
    ScicosID parentBlock;
    controller.getObjectProperty(adaptee, PARENT_BLOCK, parentBlock);
    if (parentBlock == ScicosID())
    {
        // Adding to a diagram
        ScicosID parentDiagram;
        controller.getObjectProperty(adaptee, PARENT_DIAGRAM, parentDiagram);

        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }
    else
    {
        // Adding to a superblock
        controller.getObjectProperty(parentBlock, BLOCK, CHILDREN, children);
    }

    std::vector<int> ret(ids.size());
    // foreach ports, resolve it or discard
    int i = 0;
    for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
    {
        ScicosID id;
        controller.getObjectProperty(*it, PORT, CONNECTED_SIGNALS, id);

        if (id == ScicosID())
        {
            // Unconnected port, no need to search in 'children'
            ret[i] = 0;
        }
        else
        {
            std::vector<ScicosID>::iterator found = std::find(children.begin(), children.end(), id);
            if (found != children.end())
            {
                ret[i] = static_cast<int>(std::distance(children.begin(), found)) + 1;
            }
            else
            {
                // connected link not found ; discard it !
                ret[i] = 0;
            }
        }
    }

    cache.insert({adaptee->id(), ret});
    return ret;
}

types::InternalType* cached_ports_get(std::map<ScicosID, std::vector<int> >& cache, const GraphicsAdapter& adaptor, const object_properties_t port_kind, const Controller& controller)
{
    auto it = cache.find(adaptor.getAdaptee()->id());
    if (it == cache.end())
    {
        return get_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, port_kind, controller);
    }

    std::vector<int> const& ports = it->second;

    double* data;
    types::Double* ret = new types::Double(static_cast<int>(ports.size()), 1, &data);

#ifdef _MSC_VER
    std::transform(ports.begin(), ports.end(), stdext::checked_array_iterator<double*>(data, ports.size()), [](int p)
    {
        return p;
    });
#else
    std::transform(ports.begin(), ports.end(), data, [](int p)
    {
        return p;
    });
#endif

    return ret;
}
bool cached_ports_set(std::map<ScicosID, std::vector<int> >& cache, GraphicsAdapter& adaptor, const object_properties_t port_kind, Controller& controller, types::InternalType* v)
{
    auto it = cache.find(adaptor.getAdaptee()->id());
    if (it == cache.end())
    {
        return update_ports_property<GraphicsAdapter, CONNECTED_SIGNALS>(adaptor, port_kind, controller, v);
    }

    if (v->getType() != types::InternalType::ScilabDouble)
    {
        return false;
    }
    types::Double* value = v->getAs<types::Double>();

    // store the updated value locally
    {
        std::vector<int>& ports = it->second;

        ports.resize(value->getSize());
        for (int i = 0; i < value->getSize(); ++i)
        {
            ports[i] = static_cast<int>(value->get(i));
        }
    }

    // enforce a the same number of port on the Model
    {
        const std::vector<int>& ports = it->second;

        std::vector<ScicosID> objects;
        controller.getObjectProperty(adaptor.getAdaptee(), port_kind, objects);

        if (ports.size() < objects.size())
        {
            // remove existing ports
            for (size_t i = ports.size(); i < objects.size(); ++i)
            {
                ScicosID p = objects[i];

                ScicosID signal;
                controller.getObjectProperty(p, PORT, CONNECTED_SIGNALS, signal);
                if (signal != ScicosID())
                {
                    model::Link* link = controller.getBaseObject<model::Link>(signal);
                    ScicosID opposite;
                    controller.getObjectProperty(link, DESTINATION_PORT, opposite);
                    if (opposite == p)
                    {
                        controller.setObjectProperty(link, DESTINATION_PORT, ScicosID());
                    }
                    controller.getObjectProperty(link, SOURCE_PORT, opposite);
                    if (opposite == p)
                    {
                        controller.setObjectProperty(link, SOURCE_PORT, ScicosID());
                    }
                }
                controller.deleteObject(p);
            }
            objects.resize(ports.size());
        }
        else
        {
            // add missing ports
            for (size_t i = objects.size(); i < ports.size(); ++i)
            {
                model::Port* p = controller.createBaseObject<model::Port>(PORT);

                controller.setObjectProperty(p, SOURCE_BLOCK, adaptor.getAdaptee()->id());
                controller.setObjectProperty(p, PORT_KIND, port_from_property(port_kind));

                objects.push_back(p->id());
            }
        }
        controller.setObjectProperty(adaptor.getAdaptee(), port_kind, objects);
    }
    return true;
}

struct pin
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return cached_ports_get(partial_pin, adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return cached_ports_set(partial_pin, adaptor, INPUTS, controller, v);
    }
};

struct pout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return cached_ports_get(partial_pout, adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return cached_ports_set(partial_pout, adaptor, OUTPUTS, controller, v);
    }
};

struct pein
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return cached_ports_get(partial_pein, adaptor, EVENT_INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return cached_ports_set(partial_pein, adaptor, EVENT_INPUTS, controller, v);
    }
};

struct peout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return cached_ports_get(partial_peout, adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return cached_ports_set(partial_peout, adaptor, EVENT_OUTPUTS, controller, v);
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
        adaptor.setGrIContent(v);
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
        model::Block* adaptee = adaptor.getAdaptee();

        std::string style;
        controller.getObjectProperty(adaptee, STYLE, style);

        return new types::String(style.c_str());
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();
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

            controller.setObjectProperty(adaptee, STYLE, style);
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
            controller.setObjectProperty(adaptee, STYLE, style);
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
        property<GraphicsAdapter>::reserve_properties(16);
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
        property<GraphicsAdapter>::shrink_to_fit();
    }
}

GraphicsAdapter::GraphicsAdapter() :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(),
    gr_i_content(reference_value(types::Double::Empty()))
{
    initialize_fields();
}

GraphicsAdapter::GraphicsAdapter(const Controller& c, model::Block* adaptee) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    gr_i_content(reference_value(types::Double::Empty()))
{
    initialize_fields();

    Controller controller;
    cached_ports_init(partial_pin, adaptee, INPUTS, controller);
    cached_ports_init(partial_pout, adaptee, OUTPUTS, controller);
    cached_ports_init(partial_pein, adaptee, EVENT_INPUTS, controller);
    cached_ports_init(partial_peout, adaptee, EVENT_OUTPUTS, controller);
}

GraphicsAdapter::~GraphicsAdapter()
{
    gr_i_content->DecreaseRef();
    gr_i_content->killMe();

    if (getAdaptee() != nullptr && getAdaptee()->refCount() == 0)
    {
        partial_pin.erase(getAdaptee()->id());
        partial_pout.erase(getAdaptee()->id());
        partial_pein.erase(getAdaptee()->id());
        partial_peout.erase(getAdaptee()->id());
    }
}

std::wstring GraphicsAdapter::getTypeStr() const
{
    return getSharedTypeStr();
}

std::wstring GraphicsAdapter::getShortTypeStr() const
{
    return getSharedTypeStr();
}

types::InternalType* GraphicsAdapter::getGrIContent() const
{
    return gr_i_content;
}

void GraphicsAdapter::setGrIContent(types::InternalType* v)
{
    types::InternalType* temp = gr_i_content;

    v->IncreaseRef();
    gr_i_content = v;

    temp->DecreaseRef();
    temp->killMe();
}

static void relink_cached(Controller& controller, model::BaseObject* adaptee, const std::vector<ScicosID>& children, std::map<ScicosID, std::vector<int> >& cache, object_properties_t p)
{
    auto it = cache.find(adaptee->id());
    if (it == cache.end())
    {
        // unable to relink as there is no information to do so
        return;
    }
    std::vector<int>& cached_information = it->second;

    std::vector<ScicosID> ports;
    controller.getObjectProperty(adaptee, p, ports);

    if (cached_information.size() != ports.size())
    {
        // defensive programming: unable to relink as something goes wrong on the adapters
        return;
    }

    bool isConnected = true;
    for (size_t i = 0; i < cached_information.size(); ++i)
    {
        ScicosID connectedSignal;
        controller.getObjectProperty(ports[i], PORT, CONNECTED_SIGNALS, connectedSignal);

        if (connectedSignal != ScicosID())
        {
            cached_information[i] = (int)std::distance(children.begin(), std::find(children.begin(), children.end(), connectedSignal));
        }
        else
        {
            isConnected = false;
        }
    }

    if (isConnected)
    {
        cache.erase(it);
    }
}

void GraphicsAdapter::relink(Controller& controller, model::Block* adaptee, const std::vector<ScicosID>& children)
{
    relink_cached(controller, adaptee, children, partial_pin, INPUTS);
    relink_cached(controller, adaptee, children, partial_pout, OUTPUTS);
    relink_cached(controller, adaptee, children, partial_pein, EVENT_INPUTS);
    relink_cached(controller, adaptee, children, partial_peout, EVENT_OUTPUTS);
}

void copyOnClone(model::BaseObject* original, model::BaseObject* cloned, std::map<ScicosID, std::vector<int> >& cache)
{

    auto it = cache.find(original->id());
    if (it != cache.end())
        cache.insert({cloned->id(), it->second});
}

void GraphicsAdapter::add_partial_links_information(Controller& controller, model::BaseObject* original, model::BaseObject* cloned)
{
    // precondition
    if (cloned == nullptr)
    {
        return;
    }

    if (original->kind() == BLOCK)
    {
        // add the from / to information if applicable
        copyOnClone(original, cloned, partial_pin);
        copyOnClone(original, cloned, partial_pout);
        copyOnClone(original, cloned, partial_pein);
        copyOnClone(original, cloned, partial_peout);
    }

    switch (original->kind())
    {
        // handle recursion
        case DIAGRAM:
        case BLOCK:
        {
            std::vector<ScicosID> originalChildren;
            controller.getObjectProperty(original, CHILDREN, originalChildren);
            std::vector<ScicosID> clonedChildren;
            controller.getObjectProperty(cloned, CHILDREN, clonedChildren);

            for (size_t i = 0; i < originalChildren.size(); ++i)
            {
                // a clone preserve position thus null ID, ignore them on
                // this loop
                if (originalChildren[i] != ScicosID())
                {
                    add_partial_links_information(controller, controller.getBaseObject(originalChildren[i]), controller.getBaseObject(clonedChildren[i]));
                }
            }
            break;
        }

        default:
            break;
    }
}

void GraphicsAdapter::remove_partial_links_information(model::Block* o)
{
    partial_pin.erase(o->id());
    partial_pout.erase(o->id());
    partial_pein.erase(o->id());
    partial_peout.erase(o->id());
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
