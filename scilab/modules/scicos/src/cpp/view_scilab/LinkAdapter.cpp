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

#include "internal.hxx"
#include "list.hxx"
#include "types.hxx"
#include "user.hxx"
#include "double.hxx"

#include "Controller.hxx"
#include "LinkAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct xx
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        double* data;
        types::Double* o = new types::Double(controlPoints.size() / 2, 1, &data);

        std::copy(controlPoints.begin(), controlPoints.begin() + controlPoints.size() / 2, data);
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        if (current->getSize() != static_cast<int>(controlPoints.size() / 2))
        {
            return false;
        }

        std::copy(current->getReal(), current->getReal() + current->getSize(), controlPoints.begin());
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        return true;
    }
};

struct yy
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        double* data;
        types::Double* o = new types::Double(controlPoints.size() / 2, 1, &data);

        std::copy(controlPoints.begin() + controlPoints.size() / 2, controlPoints.end(), data);
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        if (current->getSize() != static_cast<int>(controlPoints.size() / 2))
        {
            return false;
        }

        std::copy(current->getReal(), current->getReal() + current->getSize(), controlPoints.begin() + controlPoints.size() / 2);
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        return true;
    }
};

struct id
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::string id;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), LABEL, id);

        types::String* o = new types::String(1, 1);
        o->set(0, id.data());

        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
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

        model::Link* adaptee = adaptor.getAdaptee();

        std::string id;
        char* c_str = wide_string_to_UTF8(current->get(0));
        id = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), LABEL, id);
        return true;
    }
};

struct thick
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> thick;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), THICK, thick);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = thick[0];
        data[1] = thick[1];
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        std::vector<double> thick (2);
        thick[0] = current->get(0);
        thick[1] = current->get(1);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), THICK, thick);
        return true;
    }
};

struct ct
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        int color;
        int kind;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), COLOR, color);
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), KIND, kind);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = static_cast<double>(color);
        data[1] = static_cast<double>(kind);
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }
        if (floor(current->get(0)) != current->get(0) || floor(current->get(1)) != current->get(1))
        {
            return false;
        }

        int color = static_cast<int>(current->get(0));
        int kind  = static_cast<int>(current->get(1));

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), COLOR, color);
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), KIND, kind);
        return true;
    }
};

static types::Double* getLinkEnd(const LinkAdapter& adaptor, const Controller& controller, object_properties_t end)
{
    model::Link* adaptee = adaptor.getAdaptee();

    double* data;
    types::Double* o = new types::Double(1, 3, &data);
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;

    ScicosID endID;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), end, endID);
    if (endID != 0)
    {
        ScicosID sourceBlock;
        controller.getObjectProperty(endID, PORT, SOURCE_BLOCK, sourceBlock);
        int kind;
        controller.getObjectProperty(endID, PORT, PORT_KIND, kind);

        data[0] = static_cast<double>(sourceBlock);
        data[1] = static_cast<double>(endID);
        data[2] = static_cast<double>(kind);
    }
    // Default case, the property was initialized at [].
    return o;
}

static bool setLinkEnd(LinkAdapter& adaptor, Controller& controller, object_properties_t end, types::InternalType* v)
{
    model::Link* adaptee = adaptor.getAdaptee();

    if (v->getType() != types::InternalType::ScilabDouble)
    {
        return false;
    }

    types::Double* current = v->getAs<types::Double>();

    if ((current->getRows() != 1 || current->getCols() != 3) && current->getSize() != 0)
    {
        return false; // Must be [] or [x y z]
    }

    ScicosID from;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), SOURCE_PORT, from);
    ScicosID to;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), DESTINATION_PORT, to);
    ScicosID concernedPort;
    object_properties_t otherEnd;
    object_properties_t portType;
    switch (end)
    {
        case SOURCE_PORT:
            concernedPort = from;
            otherEnd = DESTINATION_PORT;
            portType = OUTPUTS;
            break;
        case DESTINATION_PORT:
            concernedPort = to;
            otherEnd = SOURCE_PORT;
            portType = INPUTS;
            break;
        default:
            return false;
    }
    ScicosID unconnected = 0;

    if (current->getSize() == 0 || (current->get(0) == 0 || current->get(1) == 0))
    {
        // We want to set an empty link
        if (concernedPort == 0)
        {
            // In this case, the link was already empty, do a dummy call to display the console status.
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), end, concernedPort);
        }
        else
        {
            // Untie the old link on both ends and set the 2 concerned ports as unconnected
            controller.setObjectProperty(from, PORT, CONNECTED_SIGNALS, unconnected);
            controller.setObjectProperty(to, PORT, CONNECTED_SIGNALS, unconnected);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SOURCE_PORT, unconnected);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), DESTINATION_PORT, unconnected);
        }
        return true;
    }

    if (current->get(2) != 0 && current->get(2) != 1)
    {
        return false; // "From" port must be output type or implicit.
    }

    if (floor(current->get(0)) != current->get(0) || floor(current->get(1)) != current->get(1))
    {
        return false; // Must be an integer value
    }

    // Disconnect the old port if it was connected
    if (from != 0)
    {
        controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
    }

    // Connect the new one
    int blk  = static_cast<int>(current->get(0));
    int port = static_cast<int>(current->get(1));
    int kind = static_cast<int>(current->get(2));
    std::vector<ScicosID> sourceBlockPorts;
    controller.getObjectProperty(blk, BLOCK, portType, sourceBlockPorts);
    int nBlockPorts = sourceBlockPorts.size();
    // Create as many ports as necessary
    while (nBlockPorts < port)
    {
        ScicosID createdPort = controller.createObject(PORT);
        controller.setObjectProperty(createdPort, PORT, SOURCE_BLOCK, blk);
        controller.setObjectProperty(createdPort, PORT, CONNECTED_SIGNALS, unconnected);
        nBlockPorts++;
    }
    controller.getObjectProperty(blk, BLOCK, portType, sourceBlockPorts);
    ScicosID newPort = sourceBlockPorts[port - 1];
    ScicosID oldLink;
    controller.getObjectProperty(newPort, PORT, CONNECTED_SIGNALS, oldLink);
    if (oldLink != 0)
    {
        // Disconnect the old other end port and delete the old link
        ScicosID oldPort;
        controller.getObjectProperty(oldLink, LINK, otherEnd, oldPort);
        controller.setObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, unconnected);
        controller.deleteObject(oldLink);
    }

    // Connect the new source and destination ports together
    controller.setObjectProperty(newPort, PORT, PORT_KIND, kind);
    controller.setObjectProperty(newPort, PORT, CONNECTED_SIGNALS, adaptee->id());
    controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, adaptee->id());
    controller.setObjectProperty(adaptee->id(), adaptee->kind(), end, newPort);
    return true;
}

struct from
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        return getLinkEnd(adaptor, controller, SOURCE_PORT);
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return setLinkEnd(adaptor, controller, SOURCE_PORT, v);
    }
};

struct to
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        return getLinkEnd(adaptor, controller, DESTINATION_PORT);
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return setLinkEnd(adaptor, controller, DESTINATION_PORT, v);
    }
};

} /* namespace */

template<> property<LinkAdapter>::props_t property<LinkAdapter>::fields = property<LinkAdapter>::props_t();

LinkAdapter::LinkAdapter(const LinkAdapter& o) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(o) {}

LinkAdapter::LinkAdapter(org_scilab_modules_scicos::model::Link* o) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(o)
{
    if (property<LinkAdapter>::properties_has_not_been_set())
    {
        property<LinkAdapter>::fields.reserve(7);
        property<LinkAdapter>::add_property(L"xx", &xx::get, &xx::set);
        property<LinkAdapter>::add_property(L"yy", &yy::get, &yy::set);
        property<LinkAdapter>::add_property(L"id", &id::get, &id::set);
        property<LinkAdapter>::add_property(L"thick", &thick::get, &thick::set);
        property<LinkAdapter>::add_property(L"ct", &ct::get, &ct::set);
        property<LinkAdapter>::add_property(L"from", &from::get, &from::set);
        property<LinkAdapter>::add_property(L"to", &to::get, &to::set);
    }
}

LinkAdapter::~LinkAdapter()
{
}

std::wstring LinkAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring LinkAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
