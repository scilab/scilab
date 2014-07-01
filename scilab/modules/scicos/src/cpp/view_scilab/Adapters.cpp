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

#include <algorithm>

#include "Adapters.hxx"

#include "BlockAdapter.hxx"
#include "CprAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "ModelAdapter.hxx"
#include "StateAdapter.hxx"
#include "TextAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

Adapters Adapters::m_instance = Adapters();

Adapters& Adapters::instance()
{
    return m_instance;
}

Adapters::Adapters()
{
    /*
     * Create the supported data types list
     */
    adapters = adapters_t();
    // we assume that the INVALID_ADAPTER is set at end of the enum
    adapters.reserve(INVALID_ADAPTER);

    adapters.push_back(adapter_t(view_scilab::BlockAdapter::getSharedTypeStr(), BLOCK_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::CprAdapter::getSharedTypeStr(), CPR_ADAPTER));
    adapters.push_back(adapter_t(L"Deleted", DELETED_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::DiagramAdapter::getSharedTypeStr(), DIAGRAM_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::GraphicsAdapter::getSharedTypeStr(), GRAPHIC_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::LinkAdapter::getSharedTypeStr(), LINK_ADAPTER));
    adapters.push_back(adapter_t(view_scilab::ModelAdapter::getSharedTypeStr(), MODEL_ADAPTER));
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

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
