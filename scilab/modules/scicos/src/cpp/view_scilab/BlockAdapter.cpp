/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
 *  Copyright (C) 2017 - ESI Group - Clement DAVID
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

#include "internal.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "types.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "model/BaseObject.hxx"
#include "model/Block.hxx"
#include "BlockAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "ModelAdapter.hxx"
#include "TextAdapter.hxx"

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

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        GraphicsAdapter localAdaptor(controller, controller.referenceBaseObject(adaptor.getAdaptee()));
        return localAdaptor.getAsTList(new types::MList(), controller);
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        GraphicsAdapter localAdaptor(controller, controller.referenceBaseObject(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        ModelAdapter localAdaptor(controller, controller.referenceBaseObject(adaptor.getAdaptee()));
        return localAdaptor.getAsTList(new types::MList(), controller);
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ModelAdapter localAdaptor(controller, controller.referenceBaseObject(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct gui
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        std::string Interface;
        controller.getObjectProperty(adaptor.getAdaptee(), INTERFACE_FUNCTION, Interface);

        return new types::String(Interface.data());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (!current->isScalar())
        {
            return false;
        }

        wchar_t* w_name = current->get(0);
        char* name = wide_string_to_UTF8(w_name);
        std::string stName(name);
        FREE(name);

        controller.setObjectProperty(adaptor.getAdaptee(), INTERFACE_FUNCTION, stName);
        return true;
    }
};

struct doc
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getDocContent();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setDocContent(v->clone());
        return true;
    }
};

link_indices_t getPortEnd(const Controller& controller, org_scilab_modules_scicos::model::Block* adaptee, portKind port)
{
    ScicosID parent;
    kind_t parentKind = BLOCK;
    controller.getObjectProperty(adaptee, PARENT_BLOCK, parent);
    if (parent == ScicosID())
    {
        parentKind = DIAGRAM;
        controller.getObjectProperty(adaptee, PARENT_DIAGRAM, parent);
    }

    // early return if this block is out of a hierarchy
    if (parent == ScicosID())
    {
        return link_indices_t();
    }

    org_scilab_modules_scicos::model::BaseObject* parentObject = controller.getBaseObject(parent);

    std::vector<ScicosID> children;
    controller.getObjectProperty(parentObject, CHILDREN, children);

    std::vector<ScicosID> ports;
    controller.getObjectProperty(parentObject, property_from_port(port), children);

    // store the index of the connected signal, 0 if absent
    link_indices_t portIndices(ports.size());
    for (size_t i = 0; i < ports.size(); ++i)
    {
        ScicosID signal;
        controller.getObjectProperty(ports[i], PORT, CONNECTED_SIGNALS, signal);

        if (signal != ScicosID())
        {
            auto it = std::find(children.begin(), children.end(), signal);
            if (it != children.end())
            {
                portIndices[i] = (int)std::distance(children.begin(), it);
            }
        }
    }

    return portIndices;
};

} /* namespace */

template<> property<BlockAdapter>::props_t property<BlockAdapter>::fields = property<BlockAdapter>::props_t();

BlockAdapter::BlockAdapter(const Controller& c, org_scilab_modules_scicos::model::Block* adaptee) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    doc_content(default_value<types::List>())
{
    if (property<BlockAdapter>::properties_have_not_been_set())
    {
        property<BlockAdapter>::reserve_properties(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
        property<BlockAdapter>::shrink_to_fit();
    }
}

BlockAdapter::BlockAdapter(const BlockAdapter& adapter) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adapter),
    doc_content(reference_value(adapter.doc_content))
{
}

BlockAdapter::~BlockAdapter()
{
    doc_content->DecreaseRef();
    doc_content->killMe();
}

std::wstring BlockAdapter::getTypeStr() const
{
    return getSharedTypeStr();
}

std::wstring BlockAdapter::getShortTypeStr() const
{
    return getSharedTypeStr();
}

types::InternalType* BlockAdapter::getDocContent() const
{
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    types::InternalType* temp = doc_content;

    // Do not check if v is nullptr on purpose ; it *should* not
    v->IncreaseRef();
    doc_content = v;

    temp->DecreaseRef();
    temp->killMe();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
