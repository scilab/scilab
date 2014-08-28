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
#include <vector>
#include <algorithm>

#include "double.hxx"
#include "string.hxx"
#include "types.hxx"

#include "ParamsAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct wpar
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        return 0;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // The model does not stock wpar.
        return true;
    }
};

struct title
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::string title;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), TITLE, title);
        std::string path;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PATH, path);

        types::String* o = new types::String(2, 1);
        o->set(0, title.data());
        o->set(1, path.data());

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        std::string path;
        std::string title;
        types::String* current = v->getAs<types::String>();
        if (current->getSize() == 1)
        {
            // Pass an empty path
        }
        else if (current->getSize() == 2)
        {
            char* Path = wide_string_to_UTF8(current->get(1));
            title = std::string(Path);
            FREE(Path);
        }
        else
        {
            return false;
        }

        char* Title = wide_string_to_UTF8(current->get(0));
        title = std::string(Title);
        FREE(Title);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), TITLE, title);
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), PATH, path);
        return true;
    }
};

struct tol
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        double* data;
        types::Double* o = new types::Double(7, 1, &data);

        std::vector<double> tol;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);

        std::copy(tol.begin() + 1, tol.end(), data);

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 7)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<double> tol;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);

        std::copy(current->getReal(), current->getReal() + current->getSize(), tol.begin() + 1);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);
        return true;
    }
};

struct tf
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<double> tf;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tf);

        return new types::Double(tf[0]);
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 1)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<double> tol;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);

        tol[0] = current->get(0);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);
        return true;
    }
};

struct context
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<std::string> context;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTEXT, context);

        types::String* o = new types::String(context.size(), 1);
        for (size_t i = 0; i < context.size(); ++i)
        {
            o->set(i, context[i].data());
        }

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getCols() != 0 && current->getCols() != 1)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::vector<std::string> context (current->getSize());
            for (size_t i = 0; i < context.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(current->get(i));
                context[i] = std::string(c_str);
                FREE(c_str);
            }

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), CONTEXT, context);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getRows() != 0 || current->getCols() != 0)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::vector<std::string> context;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), CONTEXT, context);
            return true;
        }
        return false;
    }
};

} /* namespace */

template<> property<ParamsAdapter>::props_t property<ParamsAdapter>::fields = property<ParamsAdapter>::props_t();

ParamsAdapter::ParamsAdapter(const ParamsAdapter& o) :
    BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>(o) {}

ParamsAdapter::ParamsAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>(o)
{
    if (property<ParamsAdapter>::properties_have_not_been_set())
    {
        property<ParamsAdapter>::fields.reserve(10);
        property<ParamsAdapter>::add_property(L"wpar", &wpar::get, &wpar::set);
        property<ParamsAdapter>::add_property(L"title", &title::get, &title::set);
        property<ParamsAdapter>::add_property(L"tol", &tol::get, &tol::set);
        property<ParamsAdapter>::add_property(L"tf", &tf::get, &tf::set);
        property<ParamsAdapter>::add_property(L"context", &context::get, &context::set);
        property<ParamsAdapter>::add_property(L"void1", &wpar::get, &wpar::set);
        property<ParamsAdapter>::add_property(L"options", &wpar::get, &wpar::set);
        property<ParamsAdapter>::add_property(L"void2", &wpar::get, &wpar::set);
        property<ParamsAdapter>::add_property(L"void3", &wpar::get, &wpar::set);
        property<ParamsAdapter>::add_property(L"doc", &wpar::get, &wpar::set);
    }
}

ParamsAdapter::~ParamsAdapter()
{
}

std::wstring ParamsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring ParamsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
