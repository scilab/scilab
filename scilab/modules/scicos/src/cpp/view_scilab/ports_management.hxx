/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef PORTS_MANAGEMENT_HXX_
#define PORTS_MANAGEMENT_HXX_

#include <string>
#include <vector>
#include <algorithm>

#include "internal.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "model/Port.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/*
 * Return a Scilab encoded value for a property.
 */
template<typename Adaptor, object_properties_t p>
types::InternalType* get_ports_property(const Adaptor& adaptor, object_properties_t port_kind, const Controller& controller)
{
    model::Block* adaptee = adaptor.getAdaptee();

    // Retrieve the identifiers
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), port_kind, ids);

    // Translate identifiers: shared variables
    int i = 0;
    size_t datatypeIndex = -1;
    // Translate identifiers to return values
    switch (p)
    {
        case STYLE:
        case LABEL:
        {
            types::String* o = new types::String((int)ids.size(), 1);
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                std::string s;
                controller.getObjectProperty(*it, PORT, p, s);
                o->set(i, s.data());
            }
            return o;
        }
        case DATATYPE_TYPE:
            // The type defaults to [1] if no port has been defined
            if (ids.empty())
            {
                return new types::Double(1);
            }
            datatypeIndex++;
            // no break
        case DATATYPE_COLS:
            datatypeIndex++;
            // no break
        case DATATYPE_ROWS:
        {
            datatypeIndex++;
            double* data;
            types::Double* o = new types::Double((int)ids.size(), 1, &data);
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                std::vector<int> v;
                controller.getObjectProperty(*it, PORT, DATATYPE, v);
                data[i] = v[datatypeIndex];
            }
            return o;
        }
        case FIRING:
        {
            double* data;
            types::Double* o = new types::Double((int)ids.size(), 1, &data);
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                controller.getObjectProperty(*it, PORT, p, data[i]);
            }
            return o;
        }
        case IMPLICIT:
        {
            types::String* o = new types::String((int)ids.size(), 1);
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                bool v;
                controller.getObjectProperty(*it, PORT, p, v);
                o->set(i, (v == false) ? L"E" : L"I");
            }
            return o;
        }
        case CONNECTED_SIGNALS:
        {
            double* v;
            types::Double* o = new types::Double((int)ids.size(), 1, &v);

            ScicosID diagram;
            controller.getObjectProperty(adaptee->id(), adaptee->kind(), PARENT_DIAGRAM, diagram);

            std::vector<ScicosID> children;
            if (diagram != 0)
            {
                controller.getObjectProperty(diagram, DIAGRAM, CHILDREN, children);
            }

            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                ScicosID id;
                controller.getObjectProperty(*it, PORT, p, id);

                std::vector<ScicosID>::iterator found = std::find(children.begin(), children.end(), id);

                if (found != children.end())
                {
                    v[i] = (double)std::distance(children.begin(), found) + 1;
                }
                else
                {
                    v[i] = 0;
                }
            }
            return o;
        }
        default:
            return 0;
    }
}

/*
 * Set a Scilab encoded values as a property.
 *
 * \note this method will ignore or return false if one of the ports does not exist, depending on the property setted.
 */
template<typename Adaptor, object_properties_t p>
bool set_ports_property(const Adaptor& adaptor, object_properties_t port_kind, Controller& controller, types::InternalType* v)
{
    model::Block* adaptee = adaptor.getAdaptee();

    // Retrieve the ports identifiers
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), port_kind, ids);

    if (v->getType() == types::InternalType::ScilabString)
    {
        types::String* current = v->getAs<types::String>();

        // Translate identifiers: shared variables
        int i = 0;
        // Translate identifiers from values
        switch (p)
        {
            case STYLE:
            case LABEL:
            {
                for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
                {
                    char* c_str = NULL;
                    if (i >= current->getSize())
                    {
                        // If the input isn't large enough, fill each port with empty strings
                        c_str = wide_string_to_UTF8(L"");
                    }
                    else
                    {
                        c_str = wide_string_to_UTF8(current->get(i));
                    }
                    controller.setObjectProperty(*it, PORT, p, std::string(c_str));
                    FREE(c_str);
                }
                return true;
            }
            case IMPLICIT:
            {
                if (current->getSize() != ids.size())
                {
                    return false;
                }

                std::wstring E = L"E";
                std::wstring I = L"I";
                for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
                {
                    if (current->get(i) == I)
                    {
                        controller.setObjectProperty(*it, PORT, p, true);
                    }
                    else if (current->get(i) == E)
                    {
                        controller.setObjectProperty(*it, PORT, p, false);
                    }
                    else
                    {
                        return false;
                    }
                }
                return true;
            }
            default:
                return false;
        }
    }
    else if (v->getType() == types::InternalType::ScilabDouble)
    {
        types::Double* current = v->getAs<types::Double>();

        // Translate identifiers: shared variables
        int i = 0;
        size_t datatypeIndex = -1;
        // Translate identifiers from values
        switch (p)
        {
            case FIRING:
                if (current->isEmpty())
                {
                    return true;
                }

                if (current->getSize() < ids.size())
                {
                    return false;
                }

                for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
                {
                    double firing = current->get(i);

                    controller.setObjectProperty(*it, PORT, p, firing);
                }
                return true;
            case STYLE:
            case LABEL:
                // Do nothing, because if the sizes match, then there are already zero concerned ports, so no ports to update
                return true;

            case DATATYPE_TYPE:
                datatypeIndex++;
                // no break
            case DATATYPE_COLS:
                datatypeIndex++;
                // no break
            case DATATYPE_ROWS:
            {
                datatypeIndex++;

                // ignore the set without error
                if (current->getSize() != ids.size())
                {
                    return true;
                }

                for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
                {
                    std::vector<int> v;
                    controller.getObjectProperty(*it, PORT, DATATYPE, v);

                    double data = current->get(i);
                    if (std::floor(data) != data)
                    {
                        return false;
                    }

                    v[datatypeIndex] = static_cast<int>(data);
                    controller.setObjectProperty(*it, PORT, DATATYPE, v);
                }
                return true;
            }

            case IMPLICIT:
                // Do nothing, because if the sizes match, then there are already zero concerned ports, so no ports to update
                return true;
        }

    }
    return false;
}

/**
 * Fill \a newPorts with \a d values checking content if possible.
 *
 * \param newPorts new ports children's index or value to be filled
 * \param children all object in the current layer (diagram or superblock)
 * \param d the C-array values to set
 * \return true on success, false otherwise
 */
template<typename Adaptor, object_properties_t p>
inline bool fillNewPorts(std::vector<int>& newPorts, const std::vector<ScicosID>& children, double* d)
{
    for (std::vector<int>::iterator it = newPorts.begin(); it != newPorts.end(); ++it, ++d)
    {

        if (p == CONNECTED_SIGNALS)   // the associated link must exist
        {
            if (0 > *d && *d >= children.size())
            {
                return false;
            }
        } // no check is performed for other properties as newPorts will contains value not index

        *it = static_cast<int>(*d);
    }
    return true;
}

/**
 * Set the port value
 *
 * \param oldPort the old port object ID
 * \param newPort new port children's index or value
 * \param controller current transaction instance
 * \param children all object in the current layer (diagram or superblock)
 * \param deletedObjects trash used to delete objects
 */
template<typename Adaptor, object_properties_t p>
inline void updateNewPort(ScicosID oldPort, int newPort, Controller& controller,
                          std::vector<ScicosID>& children, std::vector<ScicosID>& deletedObjects)
{
    if (p == CONNECTED_SIGNALS)
    {
        // update signal and manage deconnection, using newPort as a children index
        ScicosID oldSignal;
        controller.getObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, oldSignal);

        ScicosID newSignal;
        if (children.size() > 0)
        {
            newSignal = children[newPort];
        }
        else
        {
            newSignal = 0;
        }

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
            else   // oldSignalDst == oldPort
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
    else
    {
        // update the p property, using newPort as a value
        int datatypeIndex = -1;
        switch (p)
        {
            case DATATYPE_TYPE:
                datatypeIndex++;
                // no break
            case DATATYPE_COLS:
                datatypeIndex++;
                // no break
            case DATATYPE_ROWS:
            {
                datatypeIndex++;
                std::vector<int> datatype;
                controller.getObjectProperty(oldPort, PORT, DATATYPE, datatype);
                datatype[datatypeIndex] = newPort;
                controller.setObjectProperty(oldPort, PORT, DATATYPE, datatype);
                return;
            }
            default:
                controller.setObjectProperty(oldPort, PORT, p, newPort);
        }
    }
}

/**
 * Add a new port
 *
 * \param newPortID the old port object ID
 * \param newPort new port children's index or value
 * \param children all object in the current layer (diagram or superblock)
 * \param controller current transaction instance
 * \return true on success, false otherwise
 */
template<typename Adaptor, object_properties_t p>
inline bool addNewPort(ScicosID newPortID, int newPort, const std::vector<ScicosID>& children,	Controller& controller)
{
    bool status = true;
    if (p == CONNECTED_SIGNALS)
    {
        // set the connected signal if applicable, using newPort as a children index
        if (children.size() > 0)
        {
            ScicosID signal = children[newPort];
            status = controller.setObjectProperty(newPortID, PORT, CONNECTED_SIGNALS, signal) != FAIL;
        }
    }
    else
    {
        // set the requested property, using newPort as a value
        int datatypeIndex = -1;
        switch (p)
        {
            case DATATYPE_TYPE:
                datatypeIndex++;
                // no break
            case DATATYPE_COLS:
                datatypeIndex++;
                // no break
            case DATATYPE_ROWS:
            {
                datatypeIndex++;
                std::vector<int> datatype;
                controller.getObjectProperty(newPortID, PORT, DATATYPE, datatype);
                datatype[datatypeIndex] = newPort;
                return controller.setObjectProperty(newPortID, PORT, DATATYPE, datatype) != FAIL;
            }
            default:
                return controller.setObjectProperty(newPortID, PORT, p, newPort) != FAIL;
        }
    }

    return status;
}

/**
 * Update the ports with a specific property.
 *
 * Create ports if needed, remove ports if needed and set a default property on each port.
 */
template<typename Adaptor, object_properties_t p>
bool update_ports_property(const Adaptor& adaptor, object_properties_t port_kind,  Controller& controller, types::InternalType* v)
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

    std::vector<int> newPorts (value->getSize());

    // retrieve old data
    std::vector<ScicosID> oldPorts;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), port_kind, oldPorts);
    std::vector<ScicosID> previousPorts = oldPorts;

    double* d = value->getReal();
    if (!fillNewPorts<Adaptor, p>(newPorts, children, d))
    {
        return false;
    }

    std::vector<ScicosID> deletedObjects;

    // updated ports
    while (!oldPorts.empty() && !newPorts.empty())
    {
        ScicosID oldPort = oldPorts.back();
        oldPorts.pop_back();
        int newPort = newPorts.back();
        newPorts.pop_back();

        updateNewPort<Adaptor, p>(oldPort, newPort, controller, children, deletedObjects);
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

        controller.setObjectProperty(adaptee->id(), BLOCK, port_kind, previousPorts);
    }

    // added ports
    if (!newPorts.empty())
    {
        while (!newPorts.empty())
        {
            int newPort = newPorts.back();
            newPorts.pop_back();

            ScicosID id = controller.createObject(PORT);
            controller.setObjectProperty(id, PORT, SOURCE_BLOCK, adaptee->id());
            switch (port_kind)
            {
                case INPUTS:
                    controller.setObjectProperty(id, PORT, PORT_KIND, model::IN);
                    break;
                case OUTPUTS:
                    controller.setObjectProperty(id, PORT, PORT_KIND, model::OUT);
                    break;
                case EVENT_INPUTS:
                    controller.setObjectProperty(id, PORT, PORT_KIND, model::EIN);
                    break;
                case EVENT_OUTPUTS:
                    controller.setObjectProperty(id, PORT, PORT_KIND, model::EOUT);
                    break;
                default:
                    return false;
            }
            addNewPort<Adaptor, p>(id, newPort, children, controller);
            previousPorts.push_back(id);
        }

        controller.setObjectProperty(adaptee->id(), BLOCK, port_kind, previousPorts);
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


} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* PORTS_MANAGEMENT_HXX_ */
