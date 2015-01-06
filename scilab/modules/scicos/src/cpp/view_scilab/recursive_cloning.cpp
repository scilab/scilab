/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <vector>
#include <memory>

#include "types.hxx"
#include "list.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "recursive_cloning.hxx"
#include "BlockAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/*
 * Recursively create a List of Adapters for all objects contained in 'diagramChildren'.
 * At each call, do the linking at model-level
 */
types::List* deepCreateAdapters(const std::vector<ScicosID>& diagramChildren, types::List* oldList_objects, const std::vector<link_t>& from, const std::vector<link_t>& to, Controller controller)
{
    std::vector<LinkAdapter*> linkListView; // Store the new LinkAdapters to make the linking at model-level after the loop
    types::List* List_objects = new types::List();

    for (int i = 0; i < static_cast<int>(diagramChildren.size()); ++i)
    {
        if (diagramChildren[i] == 0)
        {
            types::MList* deletedObject = new types::MList();
            types::String* header = new types::String("Deleted");
            deletedObject->append(header);
            List_objects->append(deletedObject);
            continue;
        }

        std::shared_ptr<model::BaseObject> item = controller.getObject(diagramChildren[i]);
        switch (item->kind())
        {
            case ANNOTATION:
            {
                std::shared_ptr<model::Annotation> annotation = std::static_pointer_cast<model::Annotation>(item);
                TextAdapter* localAdaptor = new TextAdapter(annotation);

                List_objects->append(localAdaptor);
                continue;
            }
            case BLOCK:
            {
                std::shared_ptr<model::Block> block = std::static_pointer_cast<model::Block>(item);
                BlockAdapter* localAdaptor = new BlockAdapter(block);

                std::vector<ScicosID> childDiagram;
                controller.getObjectProperty(block->id(), BLOCK, CHILDREN, childDiagram);

                if (!childDiagram.empty())
                {
                    std::shared_ptr<org_scilab_modules_scicos::model::Diagram> diagram = std::static_pointer_cast<org_scilab_modules_scicos::model::Diagram>(controller.getObject(childDiagram[0]));
                    DiagramAdapter* diagramAdapter = new DiagramAdapter(diagram);

                    // Extract the information of the old cloned diagram
                    BlockAdapter* oldBlock = oldList_objects->get(i)->getAs<BlockAdapter>();
                    DiagramAdapter* oldBlockDiagram = oldBlock->getDiagram();

                    std::vector<ScicosID> subDiagramChildren;
                    controller.getObjectProperty(diagram->id(), DIAGRAM, CHILDREN, subDiagramChildren);

                    types::List* oldDiagramList_objects = oldBlockDiagram->getListObjects()->getAs<types::List>();

                    std::vector<link_t> from_vec = oldBlockDiagram->getFrom();
                    std::vector<link_t> to_vec = oldBlockDiagram->getTo();

                    types::List* new_list_objects = deepCreateAdapters(subDiagramChildren, oldDiagramList_objects, from_vec, to_vec, controller);

                    diagramAdapter->setListObjects(new_list_objects);
                    diagramAdapter->setFrom(from_vec);
                    diagramAdapter->setTo(to_vec);

                    localAdaptor->setDiagram(diagramAdapter);
                }

                List_objects->append(localAdaptor);
                continue;
            }
            case LINK:
            {
                std::shared_ptr<model::Link> link = std::static_pointer_cast<model::Link>(item);
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
        linkListView[i]->setFromInModel(from[i], controller);
        linkListView[i]->setToInModel(to[i], controller);
    }

    return List_objects;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
