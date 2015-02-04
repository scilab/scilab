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

#include <cwchar>

#include <string>
#include <vector>

#include "internal.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
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

const std::wstring Deleted (L"Deleted");

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        GraphicsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        types::InternalType* v = localAdaptor.getAsTList(new types::MList(), controller);
        return v;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        GraphicsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        ModelAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()), adaptor.getDiagram());
        types::InternalType* tlist = localAdaptor.getAsTList(new types::MList(), controller)->getAs<types::MList>();

        const_cast<BlockAdapter&>(adaptor).setDiagram(localAdaptor.getDiagram());
        return tlist;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ModelAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()), adaptor.getDiagram());
        if (!localAdaptor.setAsTList(v, controller))
        {
            return false;
        }

        adaptor.setDiagram(localAdaptor.getDiagram());
        return true;
    }
};

struct gui
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        std::string Interface;
        ScicosID adaptee = adaptor.getAdaptee()->id();
        controller.getObjectProperty(adaptee, BLOCK, INTERFACE_FUNCTION, Interface);

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

        ScicosID adaptee = adaptor.getAdaptee()->id();
        controller.setObjectProperty(adaptee, BLOCK, INTERFACE_FUNCTION, stName);
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

} /* namespace */

template<> property<BlockAdapter>::props_t property<BlockAdapter>::fields = property<BlockAdapter>::props_t();

BlockAdapter::BlockAdapter(const Controller& c, org_scilab_modules_scicos::model::Block* adaptee) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    diagramAdapter(nullptr),
    doc_content(nullptr)
{
    if (property<BlockAdapter>::properties_have_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }

    setDocContent(new types::List());
}

BlockAdapter::BlockAdapter(const BlockAdapter& adapter) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adapter, false),
    diagramAdapter(nullptr),
    doc_content(nullptr)
{
    Controller controller;

    if (adapter.getDiagram() != nullptr)
    {
    	types::InternalType* model = model::get(adapter, controller);
        model::set(*this, model, controller);
        model->killMe();
    }

    setDocContent(adapter.getDocContent());
}

BlockAdapter::~BlockAdapter()
{
    Controller controller;

    /*
     * The block own a reference to their children
     */
    std::vector<ScicosID> v;
    controller.getObjectProperty(getAdaptee()->id(), BLOCK, CHILDREN, v);
    for (const ScicosID& id : v)
    {
        controller.deleteObject(id);
    }

    if (diagramAdapter != nullptr)
    {
        diagramAdapter->DecreaseRef();
        diagramAdapter->killMe();
    }

    doc_content->DecreaseRef();
    doc_content->killMe();
}

std::wstring BlockAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring BlockAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

DiagramAdapter* BlockAdapter::getDiagram() const
{
    return diagramAdapter;
}

void BlockAdapter::setDiagram(DiagramAdapter* v)
{
    // The old 'diagramAdapter' needs to be freed after setting it to 'v'
    DiagramAdapter* temp = diagramAdapter;

    if (v != nullptr)
    {
        v->IncreaseRef();
        diagramAdapter = v;
    }

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
}

types::InternalType* BlockAdapter::getDocContent() const
{
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    types::InternalType* temp = doc_content;

    v->IncreaseRef();
    doc_content = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
