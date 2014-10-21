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
#include <memory>

#include "internal.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "model/Block.hxx"
#include "BlockAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "ModelAdapter.hxx"

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

std::wstring rpar(L"rpar");

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        GraphicsAdapter localAdaptor = GraphicsAdapter(adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::MList(), controller);
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        GraphicsAdapter localAdaptor = GraphicsAdapter(adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        ModelAdapter localAdaptor = ModelAdapter(adaptor.getAdaptee());
        types::MList* ret = localAdaptor.getAsTList(new types::MList(), controller)->getAs<types::MList>();

        // If the Block is a SuperBlock, set its 'rpar' property with the saved Diagram
        DiagramAdapter* rpar_content = adaptor.getRpar()->getAs<DiagramAdapter>();
        if (rpar_content != 0 && localAdaptor.getDiagram() == 0)
        {
            // Decrease rpar_content's ref count to counter the increases from the other Adapters
            rpar_content->DecreaseRef();
            ret->set(rpar, rpar_content);
        }

        return ret;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ModelAdapter localAdaptor = ModelAdapter(adaptor.getAdaptee());
        if (!localAdaptor.setAsTList(v, controller))
        {
            return false;
        }

        adaptor.setRpar(localAdaptor.getDiagram());
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
        if (current->getRows() != 1 || current->getCols() != 1)
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

BlockAdapter::BlockAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Block> adaptee) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adaptee),
    rpar_content(nullptr),
    doc_content(new types::List())
{
    if (property<BlockAdapter>::properties_have_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }
}

BlockAdapter::BlockAdapter(const BlockAdapter& adapter) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adapter),
    rpar_content(adapter.getRpar()),
    doc_content(adapter.getDocContent())
{
}

BlockAdapter::~BlockAdapter()
{
    if (rpar_content != nullptr)
    {
        rpar_content->DecreaseRef();
        rpar_content->killMe();
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

types::InternalType* BlockAdapter::getRpar() const
{
    if (rpar_content != nullptr)
    {
        rpar_content->IncreaseRef();
    }
    return rpar_content;
}

void BlockAdapter::setRpar(types::InternalType* v)
{
    if (v != nullptr)
    {
        // The old 'rpar_content' needs to be freed after setting it to 'v'
        types::InternalType* temp = rpar_content;

        v->IncreaseRef();
        rpar_content = v;

        if (temp != nullptr)
        {
            temp->DecreaseRef();
            temp->killMe();
        }
    }
}

types::InternalType* BlockAdapter::getDocContent() const
{
    doc_content->IncreaseRef();
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    doc_content->DecreaseRef();
    doc_content->killMe();

    v->IncreaseRef();
    doc_content = v;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
