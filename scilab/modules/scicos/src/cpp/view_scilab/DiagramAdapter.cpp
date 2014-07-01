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
#include "DiagramAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(const DiagramAdapter& o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o) {}

DiagramAdapter::DiagramAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o)
{
    if (property<DiagramAdapter>::properties_has_not_been_set())
    {
        // FIXME: add some properties
    }
}

DiagramAdapter::~DiagramAdapter()
{
}

bool DiagramAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"DiagramAdapter.hxx: Dunno what to display there";
    return true;
}

std::wstring DiagramAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
