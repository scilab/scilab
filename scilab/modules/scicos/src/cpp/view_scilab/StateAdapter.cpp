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
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "StateAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct dummy_property
{

    static types::InternalType* get(const StateAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        // Return a default empty matrix.
        return types::Double::Empty();
    }

    static bool set(StateAdapter& /*adaptor*/, types::InternalType* /*v*/, Controller& /*controller*/)
    {
        // Everything should be right as the properties mapped using this adapter do not perform anything
        return true;
    }
};

} /* namespace */

template<> property<StateAdapter>::props_t property<StateAdapter>::fields = property<StateAdapter>::props_t();

StateAdapter::StateAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Diagram> adaptee) :
    BaseAdapter<StateAdapter, org_scilab_modules_scicos::model::Diagram>(adaptee)
{
    if (property<StateAdapter>::properties_have_not_been_set())
    {
        property<StateAdapter>::fields.reserve(8);
        property<StateAdapter>::add_property(L"x", &dummy_property::get, &dummy_property::set);
        property<StateAdapter>::add_property(L"z", &dummy_property::get, &dummy_property::set);
        property<StateAdapter>::add_property(L"oz", &dummy_property::get, &dummy_property::set);
        property<StateAdapter>::add_property(L"iz", &dummy_property::get, &dummy_property::set);
        property<StateAdapter>::add_property(L"tevts", &dummy_property::get, &dummy_property::set);
        property<StateAdapter>::add_property(L"evtspt", &dummy_property::get, &dummy_property::set);
        property<StateAdapter>::add_property(L"pointi", &dummy_property::get, &dummy_property::set);
        property<StateAdapter>::add_property(L"outtb", &dummy_property::get, &dummy_property::set);
    }
}

StateAdapter::StateAdapter(const StateAdapter& adapter) :
    BaseAdapter<StateAdapter, org_scilab_modules_scicos::model::Diagram>(adapter)
{
}

StateAdapter::~StateAdapter()
{
}

std::wstring StateAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring StateAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
