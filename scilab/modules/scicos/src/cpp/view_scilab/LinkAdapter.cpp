/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2017-2018 - ESI Group - Clement DAVID
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

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "double.hxx"
#include "internal.hxx"
#include "list.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "LinkAdapter.hxx"
#include "adapters_utilities.hxx"
#include "controller_helpers.hxx"
#include "model/Block.hxx"
#include "model/Link.hxx"
#include "model/Port.hxx"
#include "utilities.hxx"

extern "C" {
#include "charEncoding.h"
#include "localization.h"
#include "sci_malloc.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::string split("split");
const std::string lsplit("lsplit");
const std::string limpsplit("limpsplit");

// shared information for relinking across adapters hierarchy
partials_links_t partial_links;

struct xx
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, CONTROL_POINTS, controlPoints);

        double* data;
        int size = (int)controlPoints.size() / 2;
        types::Double* o = new types::Double(size, 1, &data);

        for (int i = 0; i < size; ++i)
        {
            data[i] = controlPoints[2 * i];
        }
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: Real matrix object.\n"), "xx");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, CONTROL_POINTS, controlPoints);

        int newXSize = current->getSize();
        int oldXSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints(controlPoints);

        if (newXSize == oldXSize)
        {
            for (int i = 0; i < newXSize; ++i)
            {
                newControlPoints[2 * i] = current->getReal()[i];
            }
        }
        else
        {
            newControlPoints.resize(2 * current->getSize(), 0);

            for (int i = 0; i < newXSize; ++i)
            {
                newControlPoints[2 * i] = current->getReal()[i];
            }
        }

        controller.setObjectProperty(adaptee, CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct yy
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, CONTROL_POINTS, controlPoints);

        double* data;
        int size = (int)controlPoints.size() / 2;
        types::Double* o = new types::Double(size, 1, &data);

        for (int i = 0; i < size; ++i)
        {
            data[i] = controlPoints[2 * i + 1];
        }
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: Real matrix object.\n"), "yy");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, CONTROL_POINTS, controlPoints);

        int newYSize = current->getSize();
        int oldYSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints(controlPoints);

        if (newYSize == oldYSize)
        {
            for (int i = 0; i < newYSize; ++i)
            {
                newControlPoints[2 * i + 1] = current->getReal()[i];
            }
        }
        else
        {
            newControlPoints.resize(2 * current->getSize());

            for (int i = 0; i < newYSize; ++i)
            {
                newControlPoints[2 * i + 1] = current->getReal()[i];
            }
        }

        controller.setObjectProperty(adaptee, CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct id
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::string id;
        controller.getObjectProperty(adaptee, DESCRIPTION, id);

        types::String* o = new types::String(id.data());
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: String matrix expected.\n"), "id");
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s: %d-by-%d expected.\n"), "id", 1, 1);
            return false;
        }

        model::Link* adaptee = adaptor.getAdaptee();

        char* c_str = wide_string_to_UTF8(current->get(0));
        std::string description(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee, DESCRIPTION, description);
        return true;
    }
};

struct thick
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> thick;
        controller.getObjectProperty(adaptee, THICK, thick);

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
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: Real matrix expected.\n"), "thick");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 2)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s: %d-by-%d expected.\n"), "thick", 2, 1);
            return false;
        }

        std::vector<double> thick(2);
        thick[0] = current->get(0);
        thick[1] = current->get(1);

        controller.setObjectProperty(adaptee, THICK, thick);
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
        controller.getObjectProperty(adaptee, COLOR, color);
        controller.getObjectProperty(adaptee, KIND, kind);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = color;
        data[1] = kind;
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
        if (current->getSize() != 2)
        {
            return false;
        }
        if (floor(current->get(0)) != current->get(0) || floor(current->get(1)) != current->get(1))
        {
            return false;
        }

        int color = static_cast<int>(current->get(0));
        int kind = static_cast<int>(current->get(1));

        controller.setObjectProperty(adaptee, COLOR, color);
        controller.setObjectProperty(adaptee, KIND, kind);
        return true;
    }
};

static link_t getLinkEnd(model::Link* adaptee, const Controller& controller,
                         const object_properties_t end)
{
    link_t ret{0, 0, Start};
    if (end == DESTINATION_PORT)
    {
        ret.kind = End;
    }

    ScicosID endID;
    controller.getObjectProperty(adaptee, end, endID);
    if (endID != ScicosID())
    {
        ScicosID sourceBlock;
        controller.getObjectProperty(endID, PORT, SOURCE_BLOCK, sourceBlock);
        model::Block* sourceBlockObject =
            controller.getBaseObject<model::Block>(sourceBlock);

        // Looking for the block number among the block IDs
        ScicosID parent;
        kind_t parentKind = BLOCK;
        controller.getObjectProperty(adaptee, PARENT_BLOCK, parent);
        std::vector<ScicosID> children;
        // Added to a superblock
        if (parent == ScicosID())
        {
            // Added to a diagram
            controller.getObjectProperty(adaptee, PARENT_DIAGRAM, parent);
            parentKind = DIAGRAM;
            if (parent == ScicosID())
            {
                return ret;
            }
        }
        controller.getObjectProperty(parent, parentKind, CHILDREN, children);

        ret.block =
            static_cast<int>(std::distance(children.begin(),
                                           std::find(children.begin(),
                                                   children.end(), sourceBlock)) +
                             1);

        // To find the port index from its 'endID' ID, search through all the
        // block's ports lists
        std::vector<ScicosID> sourceBlockPorts;
        controller.getObjectProperty(sourceBlockObject, INPUTS, sourceBlockPorts);

        std::vector<ScicosID>::iterator found =
            std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
        if (found == sourceBlockPorts.end()) // Not found in the data inputs
        {
            sourceBlockPorts.clear();
            controller.getObjectProperty(sourceBlockObject, OUTPUTS,
                                         sourceBlockPorts);
            found =
                std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
            if (found == sourceBlockPorts.end()) // Not found in the data outputs
            {
                sourceBlockPorts.clear();
                controller.getObjectProperty(sourceBlockObject, EVENT_INPUTS,
                                             sourceBlockPorts);
                found =
                    std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
                if (found == sourceBlockPorts.end()) // Not found in the event inputs
                {
                    sourceBlockPorts.clear();
                    controller.getObjectProperty(sourceBlockObject, EVENT_OUTPUTS,
                                                 sourceBlockPorts);
                    found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(),
                                      endID);
                    if (found == sourceBlockPorts.end()) // Not found in the event outputs
                    {
                        return ret;
                    }
                }
            }
        }
        ret.port =
            static_cast<int>(std::distance(sourceBlockPorts.begin(), found) + 1);

        int kind;
        controller.getObjectProperty(endID, PORT, PORT_KIND, kind);
        if (kind == PORT_IN || kind == PORT_EIN)
        {
            ret.kind = End;
        }
        else
        {
            ret.kind = Start;
        }
    }
    // Default case, the property was initialized at [].
    return ret;
}

/*
 * Connectivity is ensured if 'port' is of the desired type or if either of the concerned blocks is a split block,
 * because they are connectable to anything
 */
bool checkConnectivity(const int neededType, const ScicosID port, model::Block* blk1, Controller& controller)
{
    int portKind;
    controller.getObjectProperty(port, PORT, PORT_KIND, portKind);

    if (portKind != neededType)
    {
        // Last chance if one of the connecting blocks is just a split block
        std::string name1;
        controller.getObjectProperty(blk1, SIM_FUNCTION_NAME, name1);
        if (name1 != split && name1 != lsplit && name1 != limpsplit)
        {
            ScicosID blk2;
            controller.getObjectProperty(port, PORT, SOURCE_BLOCK, blk2);
            std::string name2;
            controller.getObjectProperty(blk2, BLOCK, SIM_FUNCTION_NAME, name2);
            if (name2 != split && name2 != lsplit && name2 != limpsplit)
            {
                return false;
            }
        }
    }
    return true;
}

void setLinkEnd(model::Link* linkObject, Controller& controller, const object_properties_t end, const link_t& v, const std::vector<ScicosID>& children)
{
    ScicosID from;
    controller.getObjectProperty(linkObject, SOURCE_PORT, from);
    ScicosID to;
    controller.getObjectProperty(linkObject, DESTINATION_PORT, to);
    ScicosID concernedPort;
    ScicosID otherPort;
    switch (end)
    {
        case SOURCE_PORT:
            concernedPort = from;
            otherPort = to;
            break;
        case DESTINATION_PORT:
            concernedPort = to;
            otherPort = from;
            break;
        default:
            return;
    }

    if (v.block == 0 || v.port == 0)
    {
        // We want to set an empty link
        if (concernedPort == ScicosID())
        {
            // In this case, the link was already empty, do a dummy call to display the console status.
            controller.setObjectProperty(linkObject, end, concernedPort);
        }
        else
        {
            // Untie the old link on the concerned end and set its port as unconnected
            controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, ScicosID());
            controller.setObjectProperty(linkObject, end, ScicosID());
        }
        return;
    }

    // Connect the new one

    if (v.kind != Start && v.kind != End)
    {
        return;
    }
    // kind == 0: trying to set the start of the link (output port)
    // kind == 1: trying to set the end of the link (input port)

    if (v.block < 0 || v.block > static_cast<int>(children.size()))
    {
        return; // Trying to link to a non-existing block
    }
    ScicosID blkID = children[v.block - 1];

    if (blkID == ScicosID())
    {
        // Deleted Block
        return;
    }

    // Check that the ID designates a BLOCK (and not an ANNOTATION)
    model::Block* blkObject = controller.getBaseObject<model::Block>(blkID);
    if (blkObject->kind() != BLOCK)
    {
        return;
    }

    // v.port may be decremented locally to square with the port indexes
    int portIndex = v.port;

    std::vector<ScicosID> sourceBlockPorts;
    bool newPortIsImplicit = false;
    enum portKind newPortKind = PORT_UNDEF;
    int linkType;
    controller.getObjectProperty(linkObject, KIND, linkType);
    if (linkType == model::activation)
    {
        std::vector<ScicosID> evtin;
        std::vector<ScicosID> evtout;
        controller.getObjectProperty(blkObject, EVENT_INPUTS, evtin);
        controller.getObjectProperty(blkObject, EVENT_OUTPUTS, evtout);

        if (v.kind == Start)
        {
            if (otherPort != ScicosID())
            {
                if (!checkConnectivity(PORT_EIN, otherPort, blkObject, controller))
                {
                    return;
                }
            }
            newPortKind = PORT_EOUT;
            sourceBlockPorts = evtout;
        }
        else
        {
            if (otherPort != ScicosID())
            {
                if (!checkConnectivity(PORT_EOUT, otherPort, blkObject, controller))
                {
                    return;
                }
            }
            newPortKind = PORT_EIN;
            sourceBlockPorts = evtin;
        }
    }
    else if (linkType == model::regular || linkType == model::implicit)
    {
        std::vector<ScicosID> in;
        std::vector<ScicosID> out;
        controller.getObjectProperty(blkObject, INPUTS, in);
        controller.getObjectProperty(blkObject, OUTPUTS, out);

        if (linkType == model::regular)
        {
            if (v.kind == Start)
            {
                if (otherPort != ScicosID())
                {
                    if (!checkConnectivity(PORT_IN, otherPort, blkObject, controller))
                    {
                        return;
                    }
                }
                newPortKind = PORT_OUT;
                sourceBlockPorts = out;
            }
            else
            {
                if (otherPort != ScicosID())
                {
                    if (!checkConnectivity(PORT_OUT, otherPort, blkObject, controller))
                    {
                        return;
                    }
                }
                newPortKind = PORT_IN;
                sourceBlockPorts = in;
            }

            // Rule out the implicit ports
            for (int i = 0; i < static_cast<int>(sourceBlockPorts.size()); ++i)
            {
                bool isImplicit;
                controller.getObjectProperty(sourceBlockPorts[i], PORT, IMPLICIT, isImplicit);
                if (isImplicit == true)
                {
                    sourceBlockPorts.erase(sourceBlockPorts.begin() + i);
                    if (portIndex > i + 1)
                    {
                        portIndex--; // Keep portIndex consistent with the port indexes
                    }
                }
            }
        }
        else // model::implicit
        {
            newPortIsImplicit = true;
            if (v.kind == Start)
            {
                sourceBlockPorts = out;
            }
            else // End
            {
                sourceBlockPorts = in;
            }

            // Rule out the explicit ports
            for (size_t i = 0; i < sourceBlockPorts.size(); ++i)
            {
                bool isImplicit;
                controller.getObjectProperty(sourceBlockPorts[i], PORT, IMPLICIT, isImplicit);
                if (isImplicit == false)
                {
                    sourceBlockPorts.erase(sourceBlockPorts.begin() + i);
                    if (portIndex > static_cast<int>(i + 1))
                    {
                        portIndex--; // Keep portIndex consistent with the port indexes
                    }
                }
            }
        }
    }



    model::Port* concernedPortObject = nullptr;
    int nBlockPorts = static_cast<int>(sourceBlockPorts.size());
    if (nBlockPorts >= portIndex)
    {
        concernedPort = sourceBlockPorts[portIndex - 1];
        concernedPortObject = controller.getBaseObject<model::Port>(concernedPort);
    }
    else
    {
        // the interface does not match, we should not create anything else but rather alert the user that he is not matching the block interface.
        return;
    }

    // Disconnect the old port if it was connected. After that, concernedPort will be reused to designate the new port
    if (concernedPort != ScicosID())
    {
        controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, ScicosID());
    }

    ScicosID oldLink = ScicosID();
    controller.getObjectProperty(concernedPortObject, CONNECTED_SIGNALS, oldLink);
    if (oldLink != ScicosID())
    {
        // Disconnect the old link if it was indeed connected to the concerned port
        ScicosID oldPort;
        controller.getObjectProperty(oldLink, LINK, end, oldPort);
        if (concernedPort == oldPort)
        {
            controller.setObjectProperty(oldLink, LINK, end, ScicosID());
        }
    }

    // Connect the new source and destination ports together
    controller.setObjectProperty(concernedPortObject, CONNECTED_SIGNALS, linkObject->id());
    controller.setObjectProperty(linkObject, end, concernedPort);
}

// Check if the Link is valid
bool is_valid(types::Double* o)
{
    if (o->getSize() == 0)
    {
        return true;
    }

    if (o->getSize() == 2 || o->getSize() == 3)
    {
        if (floor(o->get(0)) != o->get(0) || floor(o->get(1)) != o->get(1))
        {
            return false; // Block and Port numbers must be integer values
        }
        if (o->get(1) < 0)
        {
            return false; // Port number must be positive
        }

        if (o->getSize() == 3)
        {
            if (floor(o->get(2)) != o->get(2))
            {
                return false; // Kind must be an integer value
            }
            if (o->get(2) < 0)
            {
                return false; // Kind must be positive
            }
        }

        return true;
    }

    return false;
}

struct from
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        link_t from_content;
        auto it = partial_links.find(adaptor.getAdaptee()->id());
        if (it == partial_links.end())
        {
            // if not found use the connected value
            from_content = getLinkEnd(adaptor.getAdaptee(), controller, SOURCE_PORT);
        }
        else
        {
            // if found, use the partial value
            from_content = it->second.from;
        }

        double* data;
        types::Double* o = new types::Double(1, 3, &data);

        data[0] = from_content.block;
        data[1] = from_content.port;
        data[2] = from_content.kind;
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        if (!is_valid(current))
        {
            return false;
        }

        link_t from_content{0, 0, Start};
        if (current->getSize() >= 2)
        {
            from_content.block = static_cast<int>(current->get(0));
            from_content.port = static_cast<int>(current->get(1));
            // By default, 'kind' designates an output (set to 0)

            if (current->getSize() == 3)
            {
                from_content.kind = (current->get(2) == 0.) ? Start : End;
            }
        }

        // store the new data on the adapter, the linking will be performed later on the diagram update
        auto it = partial_links.find(adaptor.getAdaptee()->id());
        if (it == partial_links.end())
        {
            partial_link_t l;
            l.from = from_content;
            l.to = getLinkEnd(adaptor.getAdaptee(), controller, DESTINATION_PORT);
            partial_links.insert({adaptor.getAdaptee()->id(), l});
        }
        else
        {
            it->second.from = from_content;
        }
        return true;
    }
};

struct to
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        link_t to_content;
        auto it = partial_links.find(adaptor.getAdaptee()->id());

        if (it == partial_links.end())
        {
            // if not found use the connected value
            to_content = getLinkEnd(adaptor.getAdaptee(), controller, DESTINATION_PORT);
        }
        else
        {
            // if found, use the partial value
            to_content = it->second.to;
        }

        double* data;
        types::Double* o = new types::Double(1, 3, &data);

        data[0] = to_content.block;
        data[1] = to_content.port;
        data[2] = to_content.kind;
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        if (current->getSize() != 0 && current->getSize() != 2 && current->getSize() != 3)
        {
            return false;
        }

        if (!is_valid(current))
        {
            return false;
        }

        link_t to_content{0, 0, End};
        if (current->getSize() >= 2)
        {
            to_content.block = static_cast<int>(current->get(0));
            to_content.port = static_cast<int>(current->get(1));
            // By default, 'kind' designates an input (set to 1)

            if (current->getSize() == 3)
            {
                to_content.kind = (current->get(2) == 0.) ? Start : End;
            }
        }

        // store the new data on the adapter, the linking will be performed later on the diagram update
        auto it = partial_links.find(adaptor.getAdaptee()->id());
        if (it == partial_links.end())
        {
            partial_link_t l;
            l.from = getLinkEnd(adaptor.getAdaptee(), controller, SOURCE_PORT);
            l.to = to_content;
            partial_links.insert({adaptor.getAdaptee()->id(), l});
        }
        else
        {
            it->second.to = to_content;
        }
        return true;
    }
};

} /* namespace */

#ifndef _MSC_VER
template<>
#endif
property<LinkAdapter>::props_t property<LinkAdapter>::fields = property<LinkAdapter>::props_t();

LinkAdapter::LinkAdapter(const Controller& c, org_scilab_modules_scicos::model::Link* adaptee) : BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(c, adaptee)
{
    if (property<LinkAdapter>::properties_have_not_been_set())
    {
        property<LinkAdapter>::reserve_properties(7);
        property<LinkAdapter>::add_property(L"xx", &xx::get, &xx::set);
        property<LinkAdapter>::add_property(L"yy", &yy::get, &yy::set);
        property<LinkAdapter>::add_property(L"id", &id::get, &id::set);
        property<LinkAdapter>::add_property(L"thick", &thick::get, &thick::set);
        property<LinkAdapter>::add_property(L"ct", &ct::get, &ct::set);
        property<LinkAdapter>::add_property(L"from", &from::get, &from::set);
        property<LinkAdapter>::add_property(L"to", &to::get, &to::set);
        property<LinkAdapter>::shrink_to_fit();
    }
}

LinkAdapter::LinkAdapter(const LinkAdapter& adapter) : BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(adapter)
{
}

LinkAdapter::~LinkAdapter()
{
}

std::wstring LinkAdapter::getTypeStr() const
{
    return getSharedTypeStr();
}
std::wstring LinkAdapter::getShortTypeStr() const
{
    return getSharedTypeStr();
}

void refresh_shared_values(Controller& controller, model::Link* adaptee, partials_links_t::iterator& it)
{
    ScicosID from;
    controller.getObjectProperty(adaptee, SOURCE_PORT, from);
    ScicosID to;
    controller.getObjectProperty(adaptee, DESTINATION_PORT, to);

    bool isConnected = from != ScicosID() && to != ScicosID();
    if (isConnected)
    {
        partial_links.erase(it);
    }
}

void LinkAdapter::relink(Controller& controller, model::Link* adaptee, const std::vector<ScicosID>& children)
{
    auto it = partial_links.find(adaptee->id());
    if (it == partial_links.end())
    {
        // unable to relink as there is no information to do so
        return;
    }
    const partial_link_t& l = it->second;

    setLinkEnd(adaptee, controller, SOURCE_PORT, l.from, children);
    setLinkEnd(adaptee, controller, DESTINATION_PORT, l.to, children);

    refresh_shared_values(controller, adaptee, it);
}

std::vector<model::Port*> getPorts(Controller& controller, model::Block* adaptee, object_properties_t port_kind)
{
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee, port_kind, ids);

    std::vector<model::Port*> ports;
    ports.reserve(ids.size());
    for (ScicosID id : ids)
    {
        ports.push_back(controller.getBaseObject<model::Port>(id));
    }

    return ports;
}

void LinkAdapter::reverse_relink(Controller& controller, model::Block* adaptee, int index, const std::vector<ScicosID>& children)
{
    if (adaptee->id() != children[index])
    {
        return;
    }

    for (object_properties_t p :
            {
                INPUTS, OUTPUTS, EVENT_INPUTS, EVENT_OUTPUTS
            })
    {
        std::vector<model::Port*> ports = getPorts(controller, adaptee, p);

        for (size_t i = 0; i < ports.size(); i++)
        {
            ScicosID signal = ScicosID();
            controller.getObjectProperty(ports[i], CONNECTED_SIGNALS, signal);
            model::Link* link = controller.getBaseObject<model::Link>(signal);

            auto opposite = partial_links.find(signal);
            if (opposite != partial_links.end())
            {
                auto& from = opposite->second.from;
                auto& to = opposite->second.to;

                if (from.block == index + 1 && from.port == i)
                {
                    controller.setObjectProperty(link, SOURCE_PORT, ports[i]->id());
                    from.kind = Start;
                }
                else if (to.block == index + 1 && to.port == i)
                {
                    controller.setObjectProperty(link, DESTINATION_PORT, ports[i]->id());
                    to.kind = End;
                }

                refresh_shared_values(controller, link, opposite);
            }
        }
    }
}

void LinkAdapter::add_partial_links_information(Controller& controller, ScicosID original, ScicosID cloned)
{
    auto it = partial_links.find(original);
    if (it != partial_links.end())
    {
        partial_links.insert({cloned, it->second});
    }
    else
    {
        model::Link* link = controller.getBaseObject<model::Link>(original);

        partial_link_t l;
        l.from = getLinkEnd(link, controller, SOURCE_PORT);
        l.to = getLinkEnd(link, controller, DESTINATION_PORT);
        partial_links.insert({cloned, l});
    }
}

void reverse_store(Controller& controller, model::Block* removed, object_properties_t port_kind)
{
    std::vector<model::Port*> ports = getPorts(controller, removed, port_kind);
    for (model::Port* p : ports)
    {
        ScicosID signal = ScicosID();
        controller.getObjectProperty(p, CONNECTED_SIGNALS, signal);
        if (signal == ScicosID())
        {
            continue;
        }
        model::Link* link = controller.getBaseObject<model::Link>(signal);

        auto it = partial_links.find(link->id());
        if (it == partial_links.end())
        {
            partial_link_t l;
            l.from = getLinkEnd(link, controller, SOURCE_PORT);
            l.to = getLinkEnd(link, controller, DESTINATION_PORT);
            partial_links.insert({link->id(), l});
        }
    }
}

// manage partial information before a model delete
void LinkAdapter::store_partial_links_information(Controller& controller, model::BaseObject* added, int index, const std::vector<ScicosID>& children)
{
    model::BaseObject* removed = controller.getBaseObject(children[index]);
    if (removed == nullptr || added == nullptr)
    {
        return;
    }

    if (removed->kind() == LINK && added->kind() == LINK)
    {
        model::Link* link = static_cast<model::Link*>(added);

        partial_link_t l;
        l.from = getLinkEnd(link, controller, SOURCE_PORT);
        l.to = getLinkEnd(link, controller, DESTINATION_PORT);
        partial_links.insert({link->id(), l});
    }
    else if (removed->kind() == BLOCK && added->kind() == BLOCK)
    {
        model::Block* block = static_cast<model::Block*>(removed);

        reverse_store(controller, block, INPUTS);
        reverse_store(controller, block, OUTPUTS);
        reverse_store(controller, block, EVENT_INPUTS);
        reverse_store(controller, block, EVENT_OUTPUTS);
    }
}

// delete all information related to the block
void LinkAdapter::remove_partial_links_information(ScicosID uid)
{
    partial_links.erase(uid);
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
