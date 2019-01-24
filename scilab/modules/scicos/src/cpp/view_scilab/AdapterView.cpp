/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2017-2018 - ESI Group - Clement DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "../../../includes/view_scilab/AdapterView.hxx"

#include "View.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

static const std::string NAME = "AdapterView";

AdapterView::AdapterView()
{
    Controller::register_view(NAME, this);
}

AdapterView::~AdapterView()
{
    Controller::unregister_view(this);
}

void AdapterView::objectCreated(const ScicosID& uid, kind_t kind)
{
};
void AdapterView::objectReferenced(const ScicosID& uid, kind_t kind, unsigned refCount) {};
void AdapterView::objectUnreferenced(const ScicosID& uid, kind_t kind, unsigned refCount) {};
void AdapterView::objectDeleted(const ScicosID& uid, kind_t kind)
{
    switch (kind)
    {
        case BLOCK:
            GraphicsAdapter::remove_partial_links_information(uid);
            break;
        case LINK:
            LinkAdapter::remove_partial_links_information(uid);
            break;
        default:
            break;
    }
};
void AdapterView::objectCloned(const ScicosID& uid, const ScicosID& cloned, kind_t kind)
{
    Controller controller;

    switch (kind)
    {
        case BLOCK:
            GraphicsAdapter::add_partial_links_information(controller, uid, cloned);
            break;
        case LINK:
            LinkAdapter::add_partial_links_information(controller, uid, cloned);
            break;
        default:
            break;
    }
};

void AdapterView::propertyUpdated(const ScicosID& uid, kind_t kind, object_properties_t property, update_status_t status) {};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
