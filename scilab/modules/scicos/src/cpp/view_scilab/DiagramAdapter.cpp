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
#include <sstream>

#include "internal.hxx"
#include "double.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "DiagramAdapter.hxx"
#include "ParamsAdapter.hxx"
#include "TextAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
#include "model/BaseObject.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::wstring AnnotationStr (L"Text");
const std::wstring BlockStr (L"Block");
const std::wstring LinkStr (L"Link");

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
        // FIXME: get all children of the Diagram and return them as a list
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        types::List* o = new types::List();

        for (size_t i = 0; i < children.size(); ++i)
        {
            model::BaseObject* item = Controller().getObject(children[i]);
            switch (item->kind())
            {
                case ANNOTATION:
                {
                    model::Annotation* annotation = static_cast<model::Annotation*>(item);
                    TextAdapter localAdaptor = TextAdapter(annotation);
                    o->set(i, localAdaptor.getAsTList(new types::MList(), controller));
                    break;
                }
                case BLOCK:
                {
                    model::Block* block = static_cast<model::Block*>(item);
                    BlockAdapter localAdaptor = BlockAdapter(block);
                    o->set(i, localAdaptor.getAsTList(new types::MList(), controller));
                    break;
                }
                case LINK:
                {
                    model::Link* link = static_cast<model::Link*>(item);
                    LinkAdapter localAdaptor = LinkAdapter(link);
                    o->set(i, localAdaptor.getAsTList(new types::MList(), controller));
                    break;
                }
                default:
                    return 0;
            }
        }
        return o;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // FIXME implement, decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        types::List* list = v->getAs<types::List>();
        std::vector<ScicosID> diagramChildren (list->getSize());
        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() != types::InternalType::ScilabMList)
            {
                return false;
            }
            types::MList* modelElement = list->get(i)->getAs<types::MList>();

            std::wstring modelElementType = modelElement->getTypeStr();
            ScicosID id;

            if (modelElementType == AnnotationStr)
            {
                id = controller.createObject(ANNOTATION);

                TextAdapter localAdaptor = TextAdapter(static_cast<model::Annotation*>(controller.getObject(id)));
                if (!localAdaptor.setAsTList(modelElement, controller))
                {
                    return false;
                }
            }
            else if (modelElementType == BlockStr)
            {
                id = controller.createObject(BLOCK);

                BlockAdapter localAdaptor = BlockAdapter(static_cast<model::Block*>(controller.getObject(id)));
                if (!localAdaptor.setAsTList(modelElement, controller))
                {
                    return false;
                }
            }
            else if (modelElementType == LinkStr)
            {
                id = controller.createObject(LINK);

                LinkAdapter localAdaptor = LinkAdapter(static_cast<model::Link*>(controller.getObject(id)));
                if (!localAdaptor.setAsTList(modelElement, controller))
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

            diagramChildren[i] = id;
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, diagramChildren);
        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::string version;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);

        return new types::String(version.data());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string version (c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::string version;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
            return true;
        }

        return false;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getContribContent();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) v;
        (void) controller;

        adaptor.setContribContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(const DiagramAdapter& o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o)
{
    contrib_content = new types::List();
}

DiagramAdapter::DiagramAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }

    contrib_content = new types::List();
}

DiagramAdapter::~DiagramAdapter()
{
    delete contrib_content;
}

std::wstring DiagramAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContribContent() const
{
    return contrib_content;
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    delete contrib_content;
    contrib_content = v->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
