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
#include <vector>
#include <algorithm>

#include "double.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "list.hxx"
#include "tlist.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "controller_helpers.hxx"
#include "ParamsAdapter.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
#include "localization.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::wstring scsopt(L"scsopt");
const std::wstring ThreeD(L"3D");
const std::wstring Background(L"Background");
const std::wstring Link(L"Link");
const std::wstring ID(L"ID");
const std::wstring Cmap(L"Cmap");

struct dummy_property
{

    static types::InternalType* get(const ParamsAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        // Return a default empty matrix.
        return types::Double::Empty();
    }

    static bool set(ParamsAdapter& /*adaptor*/, types::InternalType* /*v*/, Controller& /*controller*/)
    {
        // everything should be right as the properties mapped using this adapter do not perform anything
        return true;
    }
};

struct wpar
{

    static types::InternalType* get(const ParamsAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        types::Double* ret = new types::Double(1, 6);
        ret->set(0, 600);
        ret->set(1, 450);
        ret->set(2, 0);
        ret->set(3, 0);
        ret->set(4, 600);
        ret->set(5, 450);
        return ret;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // The model does not store 'wpar'.
        return dummy_property::set(adaptor, v, controller);
    }
};

struct title
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string title;
        controller.getObjectProperty(adaptee, DIAGRAM, TITLE, title);
        std::string path;
        controller.getObjectProperty(adaptee, DIAGRAM, PATH, path);

        types::String* o = new types::String(2, 1);
        o->set(0, title.data());
        o->set(1, path.data());

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: String expected.\n"), "params", "title");
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();

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
            path = std::string(Path);
            FREE(Path);
        }
        else
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: String expected.\n"), "params", "title");
            return false;
        }

        char* Title = wide_string_to_UTF8(current->get(0));
        title = std::string(Title);
        FREE(Title);

        controller.setObjectProperty(adaptee, DIAGRAM, TITLE, title);
        controller.setObjectProperty(adaptee, DIAGRAM, PATH, path);
        return true;
    }
};

struct tol
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        double* data;
        types::Double* o = new types::Double(1, 7, &data);

        std::vector<double> tol;
        controller.getObjectProperty(adaptee, DIAGRAM, PROPERTIES, tol);
#ifdef _MSC_VER
        std::copy(tol.begin() + 1, tol.end(), stdext::checked_array_iterator<double*>( data, 7 ));
#else
        std::copy(tol.begin() + 1, tol.end(), data);
#endif

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real matrix expected.\n"), "params", "tol");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 6 && current->getSize() != 7)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: %d-by-%d expected.\n"), "params", "tol", 7, 1);
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> tol;
        controller.getObjectProperty(adaptee, DIAGRAM, PROPERTIES, tol);

        std::copy(current->getReal(), current->getReal() + current->getSize(), tol.begin() + 1);

        // In case the last parameter is missing
        if (current->getSize() == 6)
        {
            tol[7] = 0;
        }

        controller.setObjectProperty(adaptee, DIAGRAM, PROPERTIES, tol);
        return true;
    }
};

struct tf
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> tf;
        controller.getObjectProperty(adaptee, DIAGRAM, PROPERTIES, tf);

        return new types::Double(tf[0]);
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real expected.\n"), "params", "tf");
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 1)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: Real expected.\n"), "params", "tf");
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> tol;
        controller.getObjectProperty(adaptee, DIAGRAM, PROPERTIES, tol);

        tol[0] = current->get(0);

        controller.setObjectProperty(adaptee, DIAGRAM, PROPERTIES, tol);
        return true;
    }
};

struct context
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<std::string> context;
        controller.getObjectProperty(adaptee, DIAGRAM, DIAGRAM_CONTEXT, context);

        if (context.size() == 0)
        {
            // An empty context returns an empty matrix
            return types::Double::Empty();
        }

        types::String* o = new types::String((int)context.size(), 1);
        for (int i = 0; i < (int)context.size(); ++i)
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
            // Only allow vectors and empty matrices
            if (!current->isVector() && current->getSize() != 0)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s.%s: m-by-1 expected.\n"), "params", "context");
                return false;
            }

            ScicosID adaptee = adaptor.getAdaptee()->id();

            std::vector<std::string> context (current->getSize());
            for (int i = 0; i < (int)context.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(current->get(i));
                context[i] = std::string(c_str);
                FREE(c_str);
            }

            controller.setObjectProperty(adaptee, DIAGRAM, DIAGRAM_CONTEXT, context);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (!current->isEmpty())
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s : string expected.\n"), "params", "context");
                return false;
            }

            ScicosID adaptee = adaptor.getAdaptee()->id();

            std::vector<std::string> context;
            controller.setObjectProperty(adaptee, DIAGRAM, DIAGRAM_CONTEXT, context);
            return true;
        }

        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s : string expected.\n"), "params", "context");
        return false;
    }
};

struct options
{

    static types::InternalType* get(const ParamsAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        // Return a default 'scsopt'-typed tlist.
        types::String* header = new types::String(1, 6);
        header->set(0, scsopt.c_str());
        header->set(1, ThreeD.c_str());
        header->set(2, Background.c_str());
        header->set(3, Link.c_str());
        header->set(4, ID.c_str());
        header->set(5, Cmap.c_str());

        types::TList* Scsopt = new types::TList();
        Scsopt->append(header);

        types::List* ThreeDField = new types::List();
        types::Bool* ThreeD1 = new types::Bool(1);
        ThreeDField->append(ThreeD1);
        types::Double* ThreeD2 = new types::Double(33);
        ThreeDField->append(ThreeD2);
        Scsopt->append(ThreeDField);
        ThreeDField->killMe();

        types::Double* BackgroundField = new types::Double(1, 2);
        BackgroundField->set(0, 8);
        BackgroundField->set(1, 1);
        Scsopt->append(BackgroundField);

        types::Double* LinkField = new types::Double(1, 2);
        LinkField->set(0, 1);
        LinkField->set(1, 5);
        Scsopt->append(LinkField);

        types::List* IDField = new types::List();
        types::Double* ID1 = new types::Double(1, 4);
        ID1->set(0, 4);
        ID1->set(1, 1);
        ID1->set(2, 10);
        ID1->set(3, 1);
        IDField->append(ID1);
        types::Double* ID2 = new types::Double(1, 4);
        ID2->set(0, 4);
        ID2->set(1, 1);
        ID2->set(2, 2);
        ID2->set(3, 1);
        IDField->append(ID2);
        Scsopt->append(IDField);
        IDField->killMe();

        types::Double* CmapField = new types::Double(1, 3);
        CmapField->set(0, 0.8);
        CmapField->set(1, 0.8);
        CmapField->set(2, 0.8);
        Scsopt->append(CmapField);

        return Scsopt;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // The model does not store 'options'.
        return dummy_property::set(adaptor, v, controller);
    }
};

struct doc
{
    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getDocContent();
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setDocContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<ParamsAdapter>::props_t property<ParamsAdapter>::fields = property<ParamsAdapter>::props_t();
static void initialize_fields()
{
    if (property<ParamsAdapter>::properties_have_not_been_set())
    {
        property<ParamsAdapter>::fields.reserve(10);
        property<ParamsAdapter>::add_property(L"wpar", &wpar::get, &wpar::set);
        property<ParamsAdapter>::add_property(L"title", &title::get, &title::set);
        property<ParamsAdapter>::add_property(L"tol", &tol::get, &tol::set);
        property<ParamsAdapter>::add_property(L"tf", &tf::get, &tf::set);
        property<ParamsAdapter>::add_property(L"context", &context::get, &context::set);
        property<ParamsAdapter>::add_property(L"void1", &dummy_property::get, &dummy_property::set);
        property<ParamsAdapter>::add_property(L"options", &options::get, &options::set);
        property<ParamsAdapter>::add_property(L"void2", &dummy_property::get, &dummy_property::set);
        property<ParamsAdapter>::add_property(L"void3", &dummy_property::get, &dummy_property::set);
        property<ParamsAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }
}

ParamsAdapter::ParamsAdapter() :
    BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>(),
    doc_content(new types::List())
{
    initialize_fields();
}

ParamsAdapter::ParamsAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>(c, adaptee),
    doc_content(new types::List())
{
    initialize_fields();
}

ParamsAdapter::~ParamsAdapter()
{
    doc_content->DecreaseRef();
    doc_content->killMe();
}

std::wstring ParamsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring ParamsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* ParamsAdapter::getDocContent() const
{
    doc_content->IncreaseRef();
    return doc_content;
}

void ParamsAdapter::setDocContent(types::InternalType* v)
{
    doc_content->DecreaseRef();
    doc_content->killMe();

    v->IncreaseRef();
    doc_content = v;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
