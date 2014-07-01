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
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "LinkAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

template<> property<LinkAdapter>::props_t property<LinkAdapter>::fields = property<LinkAdapter>::props_t();

LinkAdapter::LinkAdapter(const LinkAdapter& o) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(o) {}

LinkAdapter::LinkAdapter(org_scilab_modules_scicos::model::Link* o) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(o)
{
    if (property<LinkAdapter>::properties_has_not_been_set())
    {
        // FIXME: add some properties
    }
}

LinkAdapter::~LinkAdapter()
{
}

bool LinkAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"LinkAdapter.hxx: Dunno what to display there";
    return true;
}

std::wstring LinkAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring LinkAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
