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

#include "internal.hxx"
#include "list.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "ScsAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

} /* namespace */

template<> property<ScsAdapter>::props_t property<ScsAdapter>::fields = property<ScsAdapter>::props_t();

ScsAdapter::ScsAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<ScsAdapter, org_scilab_modules_scicos::model::Diagram>(c, adaptee)
{
    if (property<ScsAdapter>::properties_have_not_been_set())
    {
        // FIXME: add some properties
    }
}

ScsAdapter::ScsAdapter(const ScsAdapter& adapter) :
    BaseAdapter<ScsAdapter, org_scilab_modules_scicos::model::Diagram>(adapter)
{
}

ScsAdapter::~ScsAdapter()
{
}

std::wstring ScsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring ScsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
