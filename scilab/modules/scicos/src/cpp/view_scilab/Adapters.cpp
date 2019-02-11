/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
 *  Copyright (C) 2017-2018 - ESI Group - Clement DAVID
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

#include "../../../includes/view_scilab/Adapters.hxx"

#include <string>
#include <algorithm>

#include "View.hxx"
#include "BlockAdapter.hxx"
#include "CprAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "ModelAdapter.hxx"
#include "ParamsAdapter.hxx"
#include "ScsAdapter.hxx"
#include "StateAdapter.hxx"
#include "TextAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

Adapters Adapters::m_instance;

Adapters& Adapters::instance()
{
    return m_instance;
}

Adapters::Adapters() : adapters()
{
    /*
     * Create the supported data types list
     */
    // we assume that the INVALID_ADAPTER is set at end of the enum
    adapters.reserve(INVALID_ADAPTER);

    adapters.push_back(adapter_t(view_scilab::BlockAdapter::getSharedTypeStr(), BLOCK_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::CprAdapter::getSharedTypeStr(), CPR_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::DiagramAdapter::getSharedTypeStr(), DIAGRAM_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::GraphicsAdapter::getSharedTypeStr(), GRAPHIC_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::LinkAdapter::getSharedTypeStr(), LINK_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::ModelAdapter::getSharedTypeStr(), MODEL_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::ParamsAdapter::getSharedTypeStr(), PARAMS_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::ScsAdapter::getSharedTypeStr(), SCS_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::StateAdapter::getSharedTypeStr(), STATE_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::TextAdapter::getSharedTypeStr(), TEXT_ADAPTER));

    std::sort(adapters.begin(), adapters.end());
}

Adapters::~Adapters()
{
    adapters.clear();
}

Adapters::adapters_index_t Adapters::lookup_by_typename(const std::wstring& name)
{
    adapters_t::iterator it = std::lower_bound(adapters.begin(), adapters.end(), name);
    if (it != adapters.end() && !(name < it->name))
    {
        return it->kind;
    }
    return INVALID_ADAPTER;
}

std::wstring Adapters::get_typename(Adapters::adapters_index_t kind)
{
    for (auto it : adapters)
    {
        if (it.kind == kind)
        {
            return it.name;
        }
    }

    return L"";
}

model::BaseObject* Adapters::descriptor(types::InternalType* v)
{
    const std::wstring& name = v->getShortTypeStr();
    adapters_t::iterator it = std::lower_bound(adapters.begin(), adapters.end(), name);
    if (v->isUserType() && it != adapters.end() && !(name < it->name))
    {
        return descriptor(it->kind, v);
    }
    return nullptr;
}

model::BaseObject* Adapters::descriptor(adapters_index_t index, types::InternalType* v)
{
    switch (index)
    {
        case BLOCK_ADAPTER:
            return v->getAs<view_scilab::BlockAdapter>()->getAdaptee();
        case CPR_ADAPTER:
            return v->getAs<view_scilab::CprAdapter>()->getAdaptee();
        case DIAGRAM_ADAPTER:
            return v->getAs<view_scilab::DiagramAdapter>()->getAdaptee();
        case GRAPHIC_ADAPTER:
            return v->getAs<view_scilab::GraphicsAdapter>()->getAdaptee();
        case LINK_ADAPTER:
            return v->getAs<view_scilab::LinkAdapter>()->getAdaptee();
        case MODEL_ADAPTER:
            return v->getAs<view_scilab::ModelAdapter>()->getAdaptee();
        case PARAMS_ADAPTER:
            return v->getAs<view_scilab::ParamsAdapter>()->getAdaptee();
        case SCS_ADAPTER:
            return v->getAs<view_scilab::ScsAdapter>()->getAdaptee();
        case STATE_ADAPTER:
            return v->getAs<view_scilab::StateAdapter>()->getAdaptee();
        case TEXT_ADAPTER:
            return v->getAs<view_scilab::TextAdapter>()->getAdaptee();
        default:
            return nullptr;
    }
}

types::InternalType* Adapters::allocate_view(ScicosID id, kind_t kind)
{
    Controller controller;

    return allocate_view(controller, controller.getBaseObject(id));
}

types::InternalType* Adapters::allocate_view(Controller& controller, model::BaseObject* o)
{
    switch (o->kind())
    {
        case BLOCK:
	        return new view_scilab::BlockAdapter(controller, static_cast<model::Block*>(o));
        case DIAGRAM:
	        return new view_scilab::DiagramAdapter(controller, static_cast<model::Diagram*>(o));
        case ANNOTATION:
	        return new view_scilab::TextAdapter(controller, static_cast<model::Annotation*>(o));
        case LINK:
            return new view_scilab::LinkAdapter(controller, static_cast<model::Link*>(o));
        default:
            return nullptr;
    }
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
