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

#include <cwchar>

#include <string>
#include <vector>

#include "internal.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "types.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "model/Block.hxx"
#include "BlockAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "ModelAdapter.hxx"
#include "TextAdapter.hxx"

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

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        GraphicsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        types::InternalType* v = localAdaptor.getAsTList(new types::MList(), controller);
        return v;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        GraphicsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        // If we are in a Superblock (has children) then reconstruct a DiagramAdapter, referencing the children
        DiagramAdapter* subDiagram = nullptr;
        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptor.getAdaptee()->id(), BLOCK, CHILDREN, children);
        if (!children.empty())
        {
            if (adaptor.getListObjects()->getSize() > 0)
            {
                Controller neededController = const_cast<Controller&>(controller);
                ScicosID newDiag = neededController.createObject(DIAGRAM);
                subDiagram = new DiagramAdapter(controller, static_cast<org_scilab_modules_scicos::model::Diagram*>(controller.getObject(newDiag)));
                neededController.setObjectProperty(newDiag, DIAGRAM, CHILDREN, children);

                for (const ScicosID id : children)
                {
                    if (id == ScicosID())
                    {
                        // Deleted object
                    }
                    else
                    {
                        auto o = controller.getObject(id);
                        neededController.setObjectProperty(o->id(), o->kind(), PARENT_DIAGRAM, newDiag);
                        neededController.referenceObject(o->id());
                    }
                }
                subDiagram->setFrom(adaptor.getFrom());
                subDiagram->setTo(adaptor.getTo());
                subDiagram->setListObjects(adaptor.getListObjects());
                subDiagram->setContribContent(adaptor.getContribContent());

                std::vector<std::string> context;
                controller.getObjectProperty(adaptor.getAdaptee()->id(), BLOCK, DIAGRAM_CONTEXT, context);
                neededController.setObjectProperty(newDiag, DIAGRAM, DIAGRAM_CONTEXT, context);
            }
            else
            {
                // The children adapters list has not been set yet. Create it, update the adapter and return.
                types::List* listObjects = new types::List();
                std::vector<link_t> from;
                std::vector<link_t> to;
                for (const ScicosID id : children)
                {
                    if (id == ScicosID())
                    {
                        // Deleted object
                    }
                    else
                    {
                        auto o = controller.getObject(id);
                        controller.referenceObject(o);

                        switch (o->kind())
                        {
                            case ANNOTATION :
                                listObjects->append(new TextAdapter(controller, static_cast<org_scilab_modules_scicos::model::Annotation*>(o)));
                                break;
                            case BLOCK :
                            {
                                BlockAdapter* block = new BlockAdapter(controller, static_cast<org_scilab_modules_scicos::model::Block*>(o));
                                listObjects->append(block);
                                break;
                            }
                            default : // LINK
                                LinkAdapter* link = new LinkAdapter(controller, static_cast<org_scilab_modules_scicos::model::Link*>(o));
                                from.push_back(link->getFrom());
                                to.push_back(link->getTo());
                                listObjects->append(link);
                                break;
                        }
                    }
                }
                const_cast<BlockAdapter&>(adaptor).setFrom(from);
                const_cast<BlockAdapter&>(adaptor).setTo(to);
                const_cast<BlockAdapter&>(adaptor).setListObjects(listObjects);
                return nullptr;
            }
        }

        ModelAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()), subDiagram);
        types::InternalType* mlist = localAdaptor.getAsTList(new types::MList(), controller)->getAs<types::MList>();

        if (localAdaptor.getDiagram() != nullptr)
        {
            // To handle the copy constructor case calling model::set
            const_cast<BlockAdapter&>(adaptor).setFrom(localAdaptor.getDiagram()->getFrom());
            const_cast<BlockAdapter&>(adaptor).setTo(localAdaptor.getDiagram()->getTo());
            const_cast<BlockAdapter&>(adaptor).setListObjects(localAdaptor.getDiagram()->getListObjects());
            const_cast<BlockAdapter&>(adaptor).setContribContent(localAdaptor.getDiagram()->getContribContent());
        }

        return mlist;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ModelAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()), nullptr);
        if (!localAdaptor.setAsTList(v, controller))
        {
            return false;
        }

        if (localAdaptor.getDiagram() != nullptr)
        {
            adaptor.setFrom(localAdaptor.getDiagram()->getFrom());
            adaptor.setTo(localAdaptor.getDiagram()->getTo());
            adaptor.setListObjects(localAdaptor.getDiagram()->getListObjects());
            adaptor.setContribContent(localAdaptor.getDiagram()->getContribContent());
        }
        return true;
    }
};

struct gui
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        std::string Interface;
        ScicosID adaptee = adaptor.getAdaptee()->id();
        controller.getObjectProperty(adaptee, BLOCK, INTERFACE_FUNCTION, Interface);

        return new types::String(Interface.data());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (!current->isScalar())
        {
            return false;
        }

        wchar_t* w_name = current->get(0);
        char* name = wide_string_to_UTF8(w_name);
        std::string stName(name);
        FREE(name);

        ScicosID adaptee = adaptor.getAdaptee()->id();
        controller.setObjectProperty(adaptee, BLOCK, INTERFACE_FUNCTION, stName);
        return true;
    }
};

struct doc
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getDocContent();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setDocContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<BlockAdapter>::props_t property<BlockAdapter>::fields = property<BlockAdapter>::props_t();

BlockAdapter::BlockAdapter(const Controller& c, org_scilab_modules_scicos::model::Block* adaptee) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    doc_content(nullptr),
    from_vec(),
    to_vec(),
    list_objects(nullptr),
    contrib_content(nullptr)
{
    if (property<BlockAdapter>::properties_have_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }

    setListObjects(new types::List());
    setContribContent(new types::List());
    setDocContent(new types::List());

    // model::get will set the adapter's content (listObjects, from_vec & to_vec) if needed
    Controller controller;
    model::get(*this, controller);

}

BlockAdapter::BlockAdapter(const BlockAdapter& adapter) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adapter, false),
    doc_content(nullptr),
    from_vec(),
    to_vec(),
    list_objects(nullptr),
    contrib_content(nullptr)
{
    Controller controller;

    if (adapter.getListObjects()->getSize() > 0)
    {
        types::InternalType* model = model::get(adapter, controller);
        model::set(*this, model, controller);
        model->killMe();
    }
    else
    {
        setListObjects(new types::List());
        setContribContent(new types::List());
    }

    setDocContent(adapter.getDocContent());
}

BlockAdapter::~BlockAdapter()
{
    // CHILDREN will be unreferenced on Controller::deleteObject

    if (list_objects != nullptr)
    {
        list_objects->DecreaseRef();
        list_objects->killMe();
    }

    if (contrib_content != nullptr)
    {
        contrib_content->DecreaseRef();
        contrib_content->killMe();
    }

    doc_content->DecreaseRef();
    doc_content->killMe();
}

std::wstring BlockAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring BlockAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* BlockAdapter::getDocContent() const
{
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    types::InternalType* temp = doc_content;

    v->IncreaseRef();
    doc_content = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
}

std::vector<link_t> BlockAdapter::getFrom() const
{
    return from_vec;
}

void BlockAdapter::setFrom(const std::vector<link_t>& from)
{
    from_vec = from;
}

std::vector<link_t> BlockAdapter::getTo() const
{
    return to_vec;
}

void BlockAdapter::setTo(const std::vector<link_t>& to)
{
    to_vec = to;
}

types::List* BlockAdapter::getListObjects() const
{
    return list_objects;
}

void BlockAdapter::setListObjects(types::List* v)
{
    types::InternalType* temp = list_objects;

    // Do not check if v is nullptr on purpose ; it *should* not
    v->IncreaseRef();
    list_objects = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
}

types::InternalType* BlockAdapter::getContribContent() const
{
    return contrib_content;
}

void BlockAdapter::setContribContent(types::InternalType* v)
{
    types::InternalType* temp = contrib_content;

    // do not check if v is nullptr on purpose ; it *should* not
    v->IncreaseRef();
    contrib_content = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
