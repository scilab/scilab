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
#include <memory>

#include "internal.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "model/Block.hxx"
#include "BlockAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "ModelAdapter.hxx"
#include "TextAdapter.hxx"

extern "C" {
#include "wchar.h"

#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::wstring Deleted (L"Deleted");

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        GraphicsAdapter localAdaptor = GraphicsAdapter(adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::MList(), controller);
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        GraphicsAdapter localAdaptor = GraphicsAdapter(adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        ModelAdapter localAdaptor = ModelAdapter(adaptor.getAdaptee());
        localAdaptor.setDiagram(adaptor.getDiagram());

        return localAdaptor.getAsTList(new types::MList(), controller)->getAs<types::MList>();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ModelAdapter localAdaptor = ModelAdapter(adaptor.getAdaptee());
        if (!localAdaptor.setAsTList(v, controller))
        {
            return false;
        }

        adaptor.setDiagram(localAdaptor.getDiagram());

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
        if (current->getRows() != 1 || current->getCols() != 1)
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

BlockAdapter::BlockAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Block> adaptee) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adaptee),
    diagramAdapter(nullptr),
    doc_content(new types::List())
{
    if (property<BlockAdapter>::properties_have_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }
}

BlockAdapter::BlockAdapter(const BlockAdapter& adapter) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adapter),
    diagramAdapter(),
    doc_content(adapter.getDocContent())
{
    Controller controller;
    std::vector<ScicosID> diagramChild;
    controller.getObjectProperty(getAdaptee()->id(), BLOCK, CHILDREN, diagramChild);

    if (!diagramChild.empty())
    {
        std::shared_ptr<org_scilab_modules_scicos::model::Diagram> diagram = std::static_pointer_cast<org_scilab_modules_scicos::model::Diagram>(controller.getObject(diagramChild[0]));
        diagramAdapter = new DiagramAdapter(diagram);

        // Extract the information of the old cloned diagram
        DiagramAdapter* oldDiagram = adapter.getDiagram();
        types::List* oldList_objects = oldDiagram->getListObjects()->getAs<types::List>();
        std::vector<link_t> from_vec = oldDiagram->getFrom();
        std::vector<link_t> to_vec = oldDiagram->getTo();

        std::vector<ScicosID> diagramChildren;
        controller.getObjectProperty(diagram->id(), DIAGRAM, CHILDREN, diagramChildren);

        // FIXME: factor the following code with DiagramAdapter copy constructor?
        std::vector<LinkAdapter*> linkListView; // Store the new LinkAdapters to make the linking at model-level after the loop
        types::List* List_objects = new types::List();
        for (int i = 0; i < static_cast<int>(diagramChildren.size()); ++i)
        {
            if (diagramChildren[i] == 0)
            {
                types::MList* deletedObject = new types::MList();
                types::String* header = new types::String(Deleted.data());
                deletedObject->append(header);
                List_objects->append(deletedObject);
                continue;
            }

            std::shared_ptr<org_scilab_modules_scicos::model::BaseObject> item = controller.getObject(diagramChildren[i]);
            switch (item->kind())
            {
                case ANNOTATION:
                {
                    std::shared_ptr<org_scilab_modules_scicos::model::Annotation> annotation = std::static_pointer_cast<org_scilab_modules_scicos::model::Annotation>(item);
                    TextAdapter* localAdaptor = new TextAdapter(annotation);

                    List_objects->append(localAdaptor);
                    continue;
                }
                case BLOCK:
                {
                    std::shared_ptr<org_scilab_modules_scicos::model::Block> block = std::static_pointer_cast<org_scilab_modules_scicos::model::Block>(item);
                    BlockAdapter* localAdaptor = new BlockAdapter(block);

                    // If the diagram's block was a SuperBlock, make its new adapter point to its old diagram
                    if (i < oldList_objects->getSize())
                    {
                        BlockAdapter* oldBlock = oldList_objects->get(i)->getAs<BlockAdapter>();
                        DiagramAdapter* oldBlockDiagram = oldBlock->getDiagram();
                        if (oldBlockDiagram != nullptr)
                        {
                            oldBlockDiagram->IncreaseRef();
                        }
                        localAdaptor->setDiagram(oldBlockDiagram);
                    }

                    List_objects->append(localAdaptor);
                    continue;
                }
                case LINK:
                {
                    std::shared_ptr<org_scilab_modules_scicos::model::Link> link = std::static_pointer_cast<org_scilab_modules_scicos::model::Link>(item);
                    LinkAdapter* localAdaptor = new LinkAdapter(link);

                    // Do the model linking in the next loop, in case the Link points to a Block that has not been added yet
                    linkListView.push_back(localAdaptor);

                    List_objects->append(localAdaptor);
                    continue;
                }
                default:
                {
                }
            }
        }

        // Do the linking at model-level, from the old 'from_vec' and 'to_vec'
        for (int i = 0; i < static_cast<int>(linkListView.size()); ++i)
        {
            // Trigger 'from' and 'to' properties
            linkListView[i]->setFromInModel(from_vec[i], controller);
            linkListView[i]->setToInModel(to_vec[i], controller);
        }

        diagramAdapter->setListObjects(List_objects);
        diagramAdapter->setFrom(from_vec);
        diagramAdapter->setTo(to_vec);
    }
}

BlockAdapter::~BlockAdapter()
{
    if (diagramAdapter != nullptr)
    {
        // Update the model if the adaptee is being deleted
        if (getAdaptee().use_count() == 3)
        {
            Controller controller;
            controller.setObjectProperty(getAdaptee()->id(), BLOCK, CHILDREN, std::vector<ScicosID>());
        }

        diagramAdapter->DecreaseRef();
        diagramAdapter->killMe();
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

DiagramAdapter* BlockAdapter::getDiagram() const
{
    if (diagramAdapter != nullptr)
    {
        diagramAdapter->IncreaseRef();
    }
    return diagramAdapter;
}

void BlockAdapter::setDiagram(DiagramAdapter* v)
{
    if (v != nullptr)
    {
        // The old 'diagramAdapter' needs to be freed after setting it to 'v'
        DiagramAdapter* temp = diagramAdapter;

        diagramAdapter = v;

        if (temp != nullptr)
        {
            temp->DecreaseRef();
            temp->killMe();
        }
    }
}

types::InternalType* BlockAdapter::getDocContent() const
{
    doc_content->IncreaseRef();
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    doc_content->DecreaseRef();
    doc_content->killMe();

    v->IncreaseRef();
    doc_content = v;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
