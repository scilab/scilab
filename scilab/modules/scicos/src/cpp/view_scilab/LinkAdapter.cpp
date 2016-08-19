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

#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "internal.hxx"
#include "list.hxx"
#include "types.hxx"
#include "user.hxx"
#include "double.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "LinkAdapter.hxx"
#include "controller_helpers.hxx"
#include "model/Link.hxx"
#include "model/Port.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
#include "localization.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::string split ("split");
const std::string lsplit ("lsplit");
const std::string limpsplit ("limpsplit");

struct xx
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

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
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: Real matrix object.\n"), "xx");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

        int newXSize = current->getSize();
        int oldXSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints (controlPoints);

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

        controller.setObjectProperty(adaptee, LINK, CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct yy
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

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
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: Real matrix object.\n"), "yy");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

        int newYSize = current->getSize();
        int oldYSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints (controlPoints);

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

        controller.setObjectProperty(adaptee, LINK, CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct id
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string id;
        controller.getObjectProperty(adaptee, LINK, LABEL, id);

        types::String* o = new types::String(1, 1);
        o->set(0, id.data());

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

        ScicosID adaptee = adaptor.getAdaptee()->id();

        char* c_str = wide_string_to_UTF8(current->get(0));
        std::string id(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee, LINK, LABEL, id);
        return true;
    }
};

struct thick
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> thick;
        controller.getObjectProperty(adaptee, LINK, THICK, thick);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = thick[0];
        data[1] = thick[1];
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

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

        std::vector<double> thick (2);
        thick[0] = current->get(0);
        thick[1] = current->get(1);

        controller.setObjectProperty(adaptee, LINK, THICK, thick);
        return true;
    }
};

struct ct
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        int color;
        int kind;
        controller.getObjectProperty(adaptee, LINK, COLOR, color);
        controller.getObjectProperty(adaptee, LINK, KIND, kind);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = color;
        data[1] = kind;
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

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
        int kind  = static_cast<int>(current->get(1));

        controller.setObjectProperty(adaptee, LINK, COLOR, color);
        controller.setObjectProperty(adaptee, LINK, KIND, kind);
        return true;
    }
};

link_t getLinkEnd(const LinkAdapter& adaptor, const Controller& controller, const object_properties_t end)
{
    ScicosID adaptee = adaptor.getAdaptee()->id();

    link_t ret {0, 0, Start};

    ScicosID endID;
    controller.getObjectProperty(adaptee, LINK, end, endID);
    if (endID != ScicosID())
    {
        ScicosID sourceBlock;
        controller.getObjectProperty(endID, PORT, SOURCE_BLOCK, sourceBlock);

        // Looking for the block number among the block IDs
        ScicosID parent;
        kind_t parentKind = BLOCK;
        controller.getObjectProperty(adaptee, LINK, PARENT_BLOCK, parent);
        std::vector<ScicosID> children;
        // Added to a superblock
        if (parent == ScicosID())
        {
            // Added to a diagram
            controller.getObjectProperty(adaptee, LINK, PARENT_DIAGRAM, parent);
            parentKind = DIAGRAM;
            if (parent == ScicosID())
            {
                return ret;
            }
        }
        controller.getObjectProperty(parent, parentKind, CHILDREN, children);

        ret.block = static_cast<int>(std::distance(children.begin(), std::find(children.begin(), children.end(), sourceBlock)) + 1);

        // To find the port index from its 'endID' ID, search through all the block's ports lists
        std::vector<ScicosID> sourceBlockPorts;
        controller.getObjectProperty(sourceBlock, BLOCK, INPUTS, sourceBlockPorts);

        std::vector<ScicosID>::iterator found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
        if (found == sourceBlockPorts.end()) // Not found in the data inputs
        {
            sourceBlockPorts.clear();
            controller.getObjectProperty(sourceBlock, BLOCK, OUTPUTS, sourceBlockPorts);
            found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
            if (found == sourceBlockPorts.end()) // Not found in the data outputs
            {
                sourceBlockPorts.clear();
                controller.getObjectProperty(sourceBlock, BLOCK, EVENT_INPUTS, sourceBlockPorts);
                found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
                if (found == sourceBlockPorts.end()) // Not found in the event inputs
                {
                    sourceBlockPorts.clear();
                    controller.getObjectProperty(sourceBlock, BLOCK, EVENT_OUTPUTS, sourceBlockPorts);
                    found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
                    if (found == sourceBlockPorts.end()) // Not found in the event outputs
                    {
                        return ret;
                    }
                }
            }
        }
        ret.port = static_cast<int>(std::distance(sourceBlockPorts.begin(), found) + 1);

        int kind;
        controller.getObjectProperty(endID, PORT, PORT_KIND, kind);
        if (kind == PORT_IN || kind == PORT_EIN)
        {
            ret.kind = End;
        }
    }
    // Default case, the property was initialized at [].
    return ret;
}

/*
 * Connectivity is ensured if 'port' is of the desired type or if either of the concerned blocks is a split block,
 * because they are connectable to anything
 */
bool checkConnectivity(const int neededType, const ScicosID port, const ScicosID blk1, Controller& controller)
{
    int portKind;
    controller.getObjectProperty(port, PORT, PORT_KIND, portKind);

    if (portKind != neededType)
    {
        // Last chance if one of the connecting blocks is just a split block
        std::string name1;
        controller.getObjectProperty(blk1, BLOCK, SIM_FUNCTION_NAME, name1);
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

void setLinkEnd(const ScicosID id, Controller& controller, const object_properties_t end, const link_t& v)
{

    ScicosID from;
    controller.getObjectProperty(id, LINK, SOURCE_PORT, from);
    ScicosID to;
    controller.getObjectProperty(id, LINK, DESTINATION_PORT, to);
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
    ScicosID unconnected = ScicosID();

    if (v.block == 0 || v.port == 0)
    {
        // We want to set an empty link
        if (concernedPort == ScicosID())
        {
            // In this case, the link was already empty, do a dummy call to display the console status.
            controller.setObjectProperty(id, LINK, end, concernedPort);
        }
        else
        {
            // Untie the old link on the concerned end and set its port as unconnected
            controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
            controller.setObjectProperty(id, LINK, end, unconnected);
        }
        return;
    }

    ScicosID parentDiagram;
    controller.getObjectProperty(id, LINK, PARENT_DIAGRAM, parentDiagram);
    std::vector<ScicosID> children;
    if (parentDiagram != ScicosID())
    {
        // Adding to a diagram
        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }
    else
    {
        ScicosID parentBlock;
        controller.getObjectProperty(id, LINK, PARENT_BLOCK, parentBlock);
        if (parentBlock != ScicosID())
        {
            // Adding to a superblock
            controller.getObjectProperty(parentBlock, BLOCK, CHILDREN, children);
        }
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
    if (controller.getObject(blkID)->kind() != BLOCK)
    {
        return;
    }

    // v.port may be decremented locally to square with the port indexes
    int portIndex = v.port;

    std::vector<ScicosID> sourceBlockPorts;
    bool newPortIsImplicit = false;
    enum portKind newPortKind = PORT_UNDEF;
    int linkType;
    controller.getObjectProperty(id, LINK, KIND, linkType);
    if (linkType == model::activation)
    {
        std::vector<ScicosID> evtin;
        std::vector<ScicosID> evtout;
        controller.getObjectProperty(blkID, BLOCK, EVENT_INPUTS, evtin);
        controller.getObjectProperty(blkID, BLOCK, EVENT_OUTPUTS, evtout);

        if (v.kind == Start)
        {
            if (otherPort != ScicosID())
            {
                if (!checkConnectivity(PORT_EIN, otherPort, blkID, controller))
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
                if (!checkConnectivity(PORT_EOUT, otherPort, blkID, controller))
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
        controller.getObjectProperty(blkID, BLOCK, INPUTS, in);
        controller.getObjectProperty(blkID, BLOCK, OUTPUTS, out);

        if (linkType == model::regular)
        {
            if (v.kind == Start)
            {
                if (otherPort != ScicosID())
                {
                    if (!checkConnectivity(PORT_IN, otherPort, blkID, controller))
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
                    if (!checkConnectivity(PORT_OUT, otherPort, blkID, controller))
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

    // Disconnect the old port if it was connected. After that, concernedPort will be reused to designate the new port
    if (concernedPort != ScicosID())
    {
        controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
    }

    int nBlockPorts = static_cast<int>(sourceBlockPorts.size());
    if (nBlockPorts >= portIndex)
    {
        concernedPort = sourceBlockPorts[portIndex - 1];
    }
    else
    {
        while (nBlockPorts < portIndex) // Create as many ports as necessary
        {
            concernedPort = controller.createObject(PORT);
            controller.setObjectProperty(concernedPort, PORT, IMPLICIT, newPortIsImplicit);
            controller.setObjectProperty(concernedPort, PORT, PORT_KIND, static_cast<int>(newPortKind));
            controller.setObjectProperty(concernedPort, PORT, SOURCE_BLOCK, blkID);
            controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
            // Set the default dataType so it is saved in the model
            std::vector<int> dataType;
            controller.getObjectProperty(concernedPort, PORT, DATATYPE, dataType);
            controller.setObjectProperty(concernedPort, PORT, DATATYPE, dataType);

            std::vector<ScicosID> concernedPorts;
            if (linkType == model::activation)
            {
                if (v.kind == Start)
                {
                    controller.getObjectProperty(blkID, BLOCK, EVENT_OUTPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, EVENT_OUTPUTS, concernedPorts);
                }
                else
                {
                    controller.getObjectProperty(blkID, BLOCK, EVENT_INPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, EVENT_INPUTS, concernedPorts);
                }
            }
            else // model::regular || model::implicit
            {
                if (v.kind == Start)
                {
                    controller.getObjectProperty(blkID, BLOCK, OUTPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, OUTPUTS, concernedPorts);
                }
                else
                {
                    controller.getObjectProperty(blkID, BLOCK, INPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, INPUTS, concernedPorts);
                }
            }

            nBlockPorts++;
        }
    }
    ScicosID oldLink;
    controller.getObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, oldLink);
    if (oldLink != ScicosID())
    {
        // Disconnect the old link
        controller.setObjectProperty(oldLink, LINK, end, unconnected);
        controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
    }

    // Connect the new source and destination ports together
    controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, id);
    controller.setObjectProperty(id, LINK, end, concernedPort);
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

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& /*controller*/)
    {
        link_t from_content = adaptor.getFrom();

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

        link_t from_content {0, 0, Start};
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

        adaptor.setFromInModel(from_content, controller);
        return true;
    }
};

struct to
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& /*controller*/)
    {
        link_t to_content = adaptor.getTo();

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

        link_t to_content {0, 0, End};
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

        adaptor.setToInModel(to_content, controller);
        return true;
    }
};

} /* namespace */

template<> property<LinkAdapter>::props_t property<LinkAdapter>::fields = property<LinkAdapter>::props_t();

LinkAdapter::LinkAdapter(const Controller& c, org_scilab_modules_scicos::model::Link* adaptee) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(c, adaptee),
    m_from(),
    m_to()
{
    if (property<LinkAdapter>::properties_have_not_been_set())
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

    // If the Link has been added to a diagram, the following lines will dig up its information at model-level
    Controller controller;
    m_from = getLinkEnd(*this, controller, SOURCE_PORT);
    m_to   = getLinkEnd(*this, controller, DESTINATION_PORT);
}

LinkAdapter::LinkAdapter(const LinkAdapter& adapter) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(adapter),
    m_from(adapter.getFrom()),
    m_to(adapter.getTo())
{
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

link_t LinkAdapter::getFrom() const
{
    return m_from;
}

void LinkAdapter::setFrom(const link_t& v)
{
    m_from = v;
}

void LinkAdapter::setFromInModel(const link_t& v, Controller& controller)
{
    m_from = v;

    ScicosID parentDiagram;
    controller.getObjectProperty(getAdaptee()->id(), LINK, PARENT_DIAGRAM, parentDiagram);
    ScicosID parentBlock;
    controller.getObjectProperty(getAdaptee()->id(), LINK, PARENT_BLOCK, parentBlock);

    if (parentDiagram != ScicosID() || parentBlock != ScicosID())
    {
        // If the Link has been added to a diagram, do the linking at model-level
        // If the provided values are wrong, the model is not updated but the info is stored in the Adapter for future attempts
        setLinkEnd(getAdaptee()->id(), controller, SOURCE_PORT, v);
    }
}

link_t LinkAdapter::getTo() const
{
    return m_to;
}

void LinkAdapter::setTo(const link_t& v)
{
    m_to = v;
}

void LinkAdapter::setToInModel(const link_t& v, Controller& controller)
{
    m_to = v;

    ScicosID parentDiagram;
    controller.getObjectProperty(getAdaptee()->id(), LINK, PARENT_DIAGRAM, parentDiagram);
    ScicosID parentBlock;
    controller.getObjectProperty(getAdaptee()->id(), LINK, PARENT_BLOCK, parentBlock);

    if (parentDiagram != ScicosID() || parentBlock != ScicosID())
    {
        // If the Link has been added to a diagram, do the linking at model-level
        // If the provided values are wrong, the model is not updated but the info is stored in the Adapter for future attempts
        setLinkEnd(getAdaptee()->id(), controller, DESTINATION_PORT, v);
    }
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
