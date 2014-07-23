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

#include "internal.hxx"
#include "list.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "model/Block.hxx"
#include "BlockAdapter.hxx"
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

struct graphics
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        return new GraphicsAdapter(adaptor.getAdaptee());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() == types::InternalType::ScilabUserType
                && v->getShortTypeStr() == GraphicsAdapter::getSharedTypeStr())
        {
            GraphicsAdapter* graphics = v->getAs<GraphicsAdapter>();
            adaptor.setAdaptee(graphics->getAdaptee());
            return true;
        }
        return false;
    }
};

struct model
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        return new ModelAdapter(adaptor.getAdaptee());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() == types::InternalType::ScilabUserType
                && v->getShortTypeStr() == ModelAdapter::getSharedTypeStr())
        {
            ModelAdapter* model = v->getAs<ModelAdapter>();
            adaptor.setAdaptee(model->getAdaptee());
            return true;
        }
        return false;
    }
};

struct gui
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        std::string interface;
        org_scilab_modules_scicos::model::Block* adaptee = adaptor.getAdaptee();
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), INTERFACE_FUNCTION, interface);

        return new types::String(interface.data());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
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

        org_scilab_modules_scicos::model::Block* adaptee = adaptor.getAdaptee();
        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), INTERFACE_FUNCTION, stName);
        return true;
    }
};

struct doc
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        return adaptor.getDocContent();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
    {
        adaptor.setDocContent(v->clone());
        return true;
    }
};

template<> property<BlockAdapter>::props_t property<BlockAdapter>::fields = property<BlockAdapter>::props_t();

BlockAdapter::BlockAdapter(const BlockAdapter& o) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(o), doc_content(o.doc_content->clone()) {}

BlockAdapter::BlockAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(o)
{
    if (property<BlockAdapter>::properties_has_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }

    doc_content = new types::List();
}

BlockAdapter::~BlockAdapter()
{
    delete doc_content;
}

bool BlockAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"BlockAdapter.hxx: Dunno what to display there";
    return true;
}

std::wstring BlockAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring BlockAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* BlockAdapter::getDocContent() const
{
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    delete doc_content;
    doc_content = v->clone();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
