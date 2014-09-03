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
#include <sstream>

#include "internal.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "DiagramAdapter.hxx"
#include "ParamsAdapter.hxx"

extern "C" {
#include "version.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {

        return 0;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return false;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        std::stringstream str;
        str << SCI_VERSION_MAJOR << '.' << SCI_VERSION_MINOR << '.' << SCI_VERSION_MAINTENANCE;

        return new types::String(str.str().c_str());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: handle version upgrade of the whole model
        return true;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        return 0;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return false;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(const DiagramAdapter& o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o), contrib(o.contrib) {}

DiagramAdapter::DiagramAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o), contrib(0)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }
}

DiagramAdapter::~DiagramAdapter()
{
}

std::wstring DiagramAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContrib() const
{
    return contrib;
}

void DiagramAdapter::setContrib(types::InternalType* contrib)
{
    this->contrib = contrib;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
