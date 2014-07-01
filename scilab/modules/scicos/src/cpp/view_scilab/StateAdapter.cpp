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
#include "StateAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

template<> property<StateAdapter>::props_t property<StateAdapter>::fields = property<StateAdapter>::props_t();

StateAdapter::StateAdapter(const StateAdapter& o) :
    BaseAdapter<StateAdapter, org_scilab_modules_scicos::model::Diagram>(o) {}

StateAdapter::StateAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<StateAdapter, org_scilab_modules_scicos::model::Diagram>(o)
{
    if (property<StateAdapter>::properties_has_not_been_set())
    {
        // FIXME: add some properties
    }
}

StateAdapter::~StateAdapter()
{
}

bool StateAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"StateAdapter.hxx: Dunno what to display there";
    return true;
}

std::wstring StateAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring StateAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
