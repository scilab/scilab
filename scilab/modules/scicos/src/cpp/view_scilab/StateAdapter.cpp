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
static void initialize_fields()
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

StateAdapter::StateAdapter() :
    BaseAdapter<StateAdapter, org_scilab_modules_scicos::model::Diagram>()
{
    initialize_fields();
}

StateAdapter::StateAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<StateAdapter, org_scilab_modules_scicos::model::Diagram>(c, adaptee)
{
    initialize_fields();
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
