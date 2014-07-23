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

#include "int.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "GraphicsAdapter.hxx"

extern "C" {
#include "sci_malloc.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

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

        data[0] = angle[0];
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

static types::Double* get_port(const GraphicsAdapter& adaptor, object_properties_t p, const Controller& controller)
{
    model::Block* adaptee = adaptor.getAdaptee();

    // retrieve the identifiers
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, ids);

    // allocate some space for the return value
    double* ports;
    types::Double* o = new types::Double(ids.size(), 1, &ports);

    // translate identifiers to return values
    for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++ports)
    {
        std::vector<ScicosID> connectedSignals;

        controller.getObjectProperty(*it, PORT, CONNECTED_SIGNALS, connectedSignals);

        // always return the first connected signals, at scilab level a port can only be connected to one signal at a time.
        if (connectedSignals.empty())
        {
            // if the port is not connected returns 0
            *ports = 0;
        }
        else
        {
            // if the port is connected, return the index of the link in the parent diagram
            ScicosID link = connectedSignals[0];

            ScicosID parentDiagram = 0;
            controller.getObjectProperty(link, LINK, PARENT_DIAGRAM, parentDiagram);

            std::vector<ScicosID> children;
            controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);

            std::vector<ScicosID>::iterator it = std::find(children.begin(), children.end(), link);
            if (it != children.end())
            {
                int index = std::distance(children.begin(), it);
                *ports = index;
            }
            else
            {
                *ports = 0;
            }
        }

        ports++;
    }

    return o;
}

static bool set_port(const GraphicsAdapter& adaptor, types::InternalType* v, object_properties_t p, Controller& controller)
{
    model::Block* adaptee = adaptor.getAdaptee();

    if (v->getType() != types::InternalType::ScilabDouble)
    {
        return false;
    }
    types::Double* value = v->getAs<types::Double>();

    ScicosID parentDiagram;
    controller.getObjectProperty(adaptee->id(), BLOCK, PARENT_DIAGRAM, parentDiagram);

    std::vector<ScicosID> children;
    if (parentDiagram != 0)
    {
        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }

    std::vector<int> newPorts = std::vector<int>(value->getSize());
    double* d = value->getReal();
    for (std::vector<int>::iterator it = newPorts.begin(); it != newPorts.end(); ++it, ++d)
    {
        if (0 > *d && *d >= children.size())
        {
            return false;
        }

        *it = (int) * d;
    }
    std::vector<ScicosID> deletedObjects;

    // retrieve old data
    std::vector<ScicosID> oldPorts;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, oldPorts);
    std::vector<ScicosID> previousPorts = oldPorts;

    // updated ports
    while (!oldPorts.empty() || !newPorts.empty())
    {
        ScicosID oldPort = oldPorts.back();
        oldPorts.pop_back();
        int newPort = newPorts.back();
        newPorts.pop_back();

        ScicosID oldSignal;
        controller.getObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, oldSignal);
        ScicosID newSignal = children[newPort];

        if (oldSignal != newSignal)
        {
            // disconnect the old link
            ScicosID oldSignalSrc;
            controller.getObjectProperty(oldSignal, LINK, SOURCE_PORT, oldSignalSrc);
            ScicosID oldSignalDst;
            controller.getObjectProperty(oldSignal, LINK, DESTINATION_PORT, oldSignalDst);

            ScicosID unconnected = 0;
            if (oldSignalSrc == oldPort)
            {
                controller.setObjectProperty(oldSignalDst, PORT, CONNECTED_SIGNALS, unconnected);
            }
            else // oldSignalDst == oldPort
            {
                controller.setObjectProperty(oldSignalSrc, PORT, CONNECTED_SIGNALS, unconnected);
            }
            // Link de-association is not performed as the link will be removed

            // connect the new link
            controller.setObjectProperty(newSignal, LINK, SOURCE_PORT, 0);
            controller.setObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, newSignal);

            children.erase(std::find(children.begin(), children.end(), oldSignal));
            deletedObjects.push_back(oldSignal);
        }
    }

    // removed ports
    if (!oldPorts.empty())
    {
        previousPorts.erase(previousPorts.begin() + oldPorts.size(), previousPorts.end());

        while (!oldPorts.empty())
        {
            ScicosID oldPort = oldPorts.back();
            oldPorts.pop_back();

            ScicosID signal;
            controller.getObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, signal);
            if (signal != 0)
            {
                // the link is connected, disconnect the other side
                ScicosID oldSignalSrc;
                controller.getObjectProperty(signal, LINK, SOURCE_PORT, oldSignalSrc);
                ScicosID oldSignalDst;
                controller.getObjectProperty(signal, LINK, DESTINATION_PORT, oldSignalDst);

                ScicosID unconnected = 0;
                if (oldSignalSrc == oldPort)
                {
                    controller.setObjectProperty(oldSignalDst, PORT, CONNECTED_SIGNALS, unconnected);
                }
                else     // oldSignalDst == oldPort
                {
                    controller.setObjectProperty(oldSignalSrc, PORT, CONNECTED_SIGNALS, unconnected);
                }

                children.erase(std::find(children.begin(), children.end(), signal));
                deletedObjects.push_back(signal);
            }

            deletedObjects.push_back(oldPort);
        }

        controller.setObjectProperty(adaptee->id(), BLOCK, p, previousPorts);
    }

    // added ports
    if (!newPorts.empty())
    {
        while (!newPorts.empty())
        {
            int newPort = newPorts.back();
            oldPorts.pop_back();

            ScicosID id = controller.createObject(PORT);
            controller.setObjectProperty(id, PORT, SOURCE_BLOCK, adaptee->id());
            // set the connected signal if applicable
            if (newPort != 0)
            {
                ScicosID signal = children[newPort];
                controller.setObjectProperty(id, PORT, CONNECTED_SIGNALS, signal);
            }

            previousPorts.push_back(id);
        }

        controller.setObjectProperty(adaptee->id(), BLOCK, p, previousPorts);
    }

    // remove objects from the model after de-association
    if (parentDiagram != 0)
    {
        controller.setObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }
    for (std::vector<ScicosID>::iterator it = deletedObjects.begin(); it != deletedObjects.end(); ++it)
    {
        controller.deleteObject(*it);
    }

    return true;
}

struct pin
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_port(adaptor, INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_port(adaptor, v, INPUTS, controller);
    }
};

struct pout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_port(adaptor, OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_port(adaptor, v, OUTPUTS, controller);
    }
};

struct pein
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_port(adaptor, EVENT_INPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_port(adaptor, v, EVENT_INPUTS, controller);
    }
};

struct peout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        return get_port(adaptor, EVENT_OUTPUTS, controller);
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return set_port(adaptor, v, EVENT_OUTPUTS, controller);
    }
};

struct in_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: implement
        return false;
    }
};

struct out_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: implement
        return false;
    }
};

struct in_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: implement
        return false;
    }
};

struct out_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: implement
        return false;
    }
};

struct in_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: implement
        return false;
    }
};

struct out_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: implement
        return false;
    }
};

struct style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor, const Controller& controller)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: implement
        return false;
    }
};

template<> property<GraphicsAdapter>::props_t property<GraphicsAdapter>::fields = property<GraphicsAdapter>::props_t();

GraphicsAdapter::GraphicsAdapter(const GraphicsAdapter& o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(o) { }

GraphicsAdapter::GraphicsAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(o)
{
    if (property<GraphicsAdapter>::properties_has_not_been_set())
    {
        property<GraphicsAdapter>::fields.reserve(15);
        property<GraphicsAdapter>::add_property(L"orig", &orig::get, &orig::set);
        property<GraphicsAdapter>::add_property(L"sz", &sz::get, &sz::set);
        property<GraphicsAdapter>::add_property(L"flip", &flip::get, &flip::set);
        property<GraphicsAdapter>::add_property(L"theta", &theta::get, &theta::set);
        property<GraphicsAdapter>::add_property(L"exprs", &exprs::get, &exprs::set);
        property<GraphicsAdapter>::add_property(L"pin", &pin::get, &pin::set);
        property<GraphicsAdapter>::add_property(L"pout", &pout::get, &pout::set);
        property<GraphicsAdapter>::add_property(L"pein", &pein::get, &pein::set);
        property<GraphicsAdapter>::add_property(L"peout", &peout::get, &peout::set);
        property<GraphicsAdapter>::add_property(L"in_implicit", &in_implicit::get, &in_implicit::set);
        property<GraphicsAdapter>::add_property(L"out_implicit", &out_implicit::get, &out_implicit::set);
        property<GraphicsAdapter>::add_property(L"in_style", &in_style::get, &in_style::set);
        property<GraphicsAdapter>::add_property(L"in_label", &in_label::get, &in_label::set);
        property<GraphicsAdapter>::add_property(L"out_label", &out_label::get, &out_label::set);
        property<GraphicsAdapter>::add_property(L"style", &style::get, &style::set);
    }
}

GraphicsAdapter::~GraphicsAdapter()
{
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

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
