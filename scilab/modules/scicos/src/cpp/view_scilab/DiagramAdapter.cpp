/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
 *  Copyright (C) 2017-2018 - ESI Group - Clement DAVID
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

#include <bitset>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>

#include "types.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "controller_helpers.hxx"

#include "view_scilab/Adapters.hxx"
#include "view_scilab/AdapterView.hxx"
#include "DiagramAdapter.hxx"
#include "ParamsAdapter.hxx"
#include "BlockAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/BaseObject.hxx"
#include "model/Diagram.hxx"
#include "model/Block.hxx"

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

const std::wstring Deleted (L"Deleted");
const std::wstring Text (L"Text");
const std::wstring Block (L"Block");
const std::wstring Link (L"Link");

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor(controller, controller.referenceBaseObject<model::BaseObject>(adaptor.getAdaptee()));
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor(controller, controller.referenceBaseObject<model::BaseObject>(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{
    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::BaseObject* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee, CHILDREN, children);
        types::List* ret = new types::List();
        // TODO: ret.reserve(children.size());
        for (int i = 0; i < static_cast<int>(children.size()); ++i)
        {
            if (children[i] == ScicosID())
            {
                // Deleted adapter
                types::MList* deleted = new types::MList();
                deleted->set(0, new types::String(Deleted.data()));
                ret->append(deleted);
                continue;
            }

            model::BaseObject* o = controller.getBaseObject(children[i]);
            switch (o->kind())
            {
                case ANNOTATION :
                    ret->append(new TextAdapter(controller, controller.referenceBaseObject<model::Annotation>(o)));
                    break;
                case BLOCK :
                    ret->append(new BlockAdapter(controller, controller.referenceBaseObject<model::Block>(o)));
                    break;
                case LINK:
                {
                    ret->append(new LinkAdapter(controller, controller.referenceBaseObject<model::Link>(o)));
                    break;
                }
                default:
                {
                    types::MList* deleted = new types::MList();
                    deleted->set(0, new types::String(Deleted.data()));
                    ret->append(deleted);
                    break;
                }
            }
        }

        return ret;
    }

    template <kind_t kind, typename Adapter, typename Adaptee>
    static Adapter* allocAndSet(types::MList* modelElement, Controller& controller)
    {
        ScicosID id = controller.createObject(kind);
        Adaptee* localAdaptee = controller.getBaseObject<Adaptee>(id);

        Adapter* localAdaptor = new Adapter(controller, localAdaptee);
        localAdaptor->setAsTList(modelElement, controller);

        return localAdaptor;
    };

    /**
     * Allocate a model element from its mlist() representation
     *
     * \param modelElement the mlist representation
     * \param controller the shared controller
     * \param o the model object
     * \param a any adapter compatible with \o
     */
    static void allocateAsMList(types::MList* modelElement, Controller& controller, model::BaseObject*& o, types::UserType*& a)
    {
        types::String* header = modelElement->getFieldNames();

        if (header->get(0) == Deleted)
        {
            a = nullptr;
            o = nullptr;
        }
        else if (header->get(0) == Text)
        {
            TextAdapter* adapter = allocAndSet<ANNOTATION, TextAdapter, model::Annotation>(modelElement, controller);
            a = adapter;
            o = adapter->getAdaptee();
        }
        else if (header->get(0) == Block)
        {
            BlockAdapter* adapter = allocAndSet<BLOCK, BlockAdapter, model::Block>(modelElement, controller);
            a = adapter;
            o = adapter->getAdaptee();
        }
        else if (header->get(0) == Link)
        {
            LinkAdapter* adapter = allocAndSet<LINK, LinkAdapter, model::Link>(modelElement, controller);
            a = adapter;
            o = adapter->getAdaptee();
        }
        else
        {
            a = nullptr;
            o = nullptr;
        }
    }

    // Helper to manage updates
    //  * set an adapter at a position
    //  * clear an adapter (eg. remove object) if the value is nullptr
    struct update_t
    {
        update_t(int index, model::BaseObject* adaptee, types::InternalType* adapter) :
            index(index), adaptee(adaptee), adapter(adapter) {}

        int index;
        model::BaseObject* adaptee;
        types::InternalType* adapter;
    };

    // Reference all the inserted children before processing them.
    // This struct (cstr and dstr) will avoid early deletion
    struct ChildrenToUpdateOwner
    {
        ChildrenToUpdateOwner(Controller& controller, const std::vector<update_t>& childrenToUpdate) :
            controller(controller),
            childrenToUpdate(childrenToUpdate)
        {
            std::for_each(childrenToUpdate.begin(), childrenToUpdate.end(), [&controller] (const update_t & u)
            {
                if (u.adapter != nullptr)
                {
                    u.adapter->IncreaseRef();
                }
                else if (u.adaptee != nullptr)
                {
                    controller.referenceBaseObject(u.adaptee);
                }
            });
        }

        ~ChildrenToUpdateOwner()
        {
            std::for_each(childrenToUpdate.begin(), childrenToUpdate.end(), [this] (const update_t & u)
            {
                if (u.adapter != nullptr)
                {
                    u.adapter->DecreaseRef();
                    u.adapter->killMe();
                }
                else if (u.adaptee != nullptr)
                {
                    controller.deleteBaseObject(u.adaptee);
                }
            });
        }

        Controller& controller;
        const std::vector<update_t>& childrenToUpdate;
    };

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // Decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s: list expected.\n"), "objs");
            return false;
        }

        model::BaseObject* adaptee = adaptor.getAdaptee();
        types::List* argumentList = v->getAs<types::List>();
        AdapterView update_partial_information;

        // retrieve the current children to update
        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee, CHILDREN, children);

        // A boolean to know if we are removing an object
        bool deletion = children.size() > static_cast<size_t>(argumentList->getSize());
        // There is going to be as many children as the input list suggests, but don't lose information on old children just yet
        if (!deletion)
        {
            children.resize(argumentList->getSize());
        }

        /*
         * Fill a buffer of things to update
         */

        // work buffer :
        std::vector<update_t> childrenToUpdate;
        Controller::cloned_t mapped;

        // fill the work buffers accordingly to the arguments
        for (int i = 0; i < argumentList->getSize(); ++i)
        {
            types::InternalType* pIT = argumentList->get(i);
            switch (pIT->getType())
            {
                case types::InternalType::ScilabUserType:
                {
                    model::BaseObject* o = Adapters::instance().descriptor(pIT);
                    if (o == nullptr)
                    {
                        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for element %d of field %s: unknown Scicos object.\n"), i + 1, "objs");
                        return false;
                    }

                    // filter out non modified children
                    if (o->id() == children[i])
                    {
                        break;
                    }

                    // clone if the adapter is used somewhere else (eg. not owned by the list)
                    // TODO add a insertionCall depth counter to use there, this will avoid extra copy on o.model.rpar.objs(1) modification
                    if (!deletion && (pIT->getRef() > 1 || o->refCount() > 0))
                    {
                        model::BaseObject *cloned = controller.cloneBaseObject(mapped, o, true, true);
                        types::InternalType *clonedAdapter = Adapters::instance().allocate_view(controller, cloned);

                        childrenToUpdate.emplace_back(i, cloned, clonedAdapter);
                        break;
                    }

                    // o have been edited in place, refresh partial information
                    childrenToUpdate.emplace_back(i, o, pIT);
                    break;
                }
                case types::InternalType::ScilabMList:
                    model::BaseObject* o;
                    types::UserType* a;
                    allocateAsMList(pIT->getAs<types::MList>(), controller, o, a);
                    childrenToUpdate.emplace_back(i, o, a);
                    break;
                case types::InternalType::ScilabList:
                    // clear on list()
                    if (pIT->getAs<types::List>()->getSize() != 0)
                    {
                        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for element %d of field %s: unknown Scicos object.\n"), i + 1, "objs");
                        return false;
                    }
                    childrenToUpdate.emplace_back(i, nullptr, nullptr);
                    break;
                default:
                    get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for element %d of field %s: unknown Scicos object.\n"), i + 1, "objs");
                    return false;
            }
        }

        // clear trailing objects
        for (int i = argumentList->getSize() ; i < static_cast<int>(children.size()); ++i)
        {
            childrenToUpdate.emplace_back(i, nullptr, nullptr);
        }

        /*
         * Update parent / children
         */
        ChildrenToUpdateOwner tempOwning(controller, childrenToUpdate);

        // Process the children / parent relationship
        ScicosID parentDiagram;
        controller.getObjectProperty(adaptee, PARENT_DIAGRAM, parentDiagram);
        int offset = 0;
        for (const auto & update : childrenToUpdate)
        {
            // reference / derefence the content
            if (update.adapter == nullptr)
            {
                ScicosID c = children[update.index];
                if (c == ScicosID())
                {
                    continue;
                }

                auto o = controller.getBaseObject(c);
                controller.deleteBaseObject(o);
            }
            else
            {
                if (deletion && children[update.index] == ScicosID())
                {
                    // This object is the one being deleted in the diagram:
                    //  - we are in effective delete mode
                    //  - the old object is a "Deleted" mlist (deletion in two  steps)
                    //  - the new object is not a "Deleted" mlist (just replacing  the old one)
                    // Then 'offset' will skip the mlist so all the old children are deleted
                    ++offset;
                }

                ScicosID deleted = children[update.index + offset];
                if (deleted)
                {
                    LinkAdapter::store_partial_links_information(controller, update.adaptee, update.index + offset, children);
                    GraphicsAdapter::store_partial_links_information(controller, update.adaptee, update.index + offset, children);
                }
            }

            // manage insertion and field update
            if (update.adapter == nullptr)
            {
                children[update.index] = ScicosID();
            }
            else
            {
                controller.referenceBaseObject(update.adaptee);
                if (children[update.index] != ScicosID())
                {
                    controller.deleteObject(children[update.index]);
                }
                children[update.index] = update.adaptee->id();

                if (adaptee->kind() == BLOCK)
                {
                    controller.setObjectProperty(update.adaptee, PARENT_DIAGRAM, parentDiagram);
                    controller.setObjectProperty(update.adaptee, PARENT_BLOCK, adaptee->id());
                }
                else
                {
                    controller.setObjectProperty(update.adaptee, PARENT_DIAGRAM, adaptee->id());
                    controller.setObjectProperty(update.adaptee, PARENT_BLOCK, ScicosID());
                }
            }
        }

        // We don't need old children information anymore: reduce children size
        if (deletion)
        {
            children.resize(argumentList->getSize());
        }

        /*
         * Update partial linking information
         */
        for (auto it = childrenToUpdate.begin(); it != childrenToUpdate.end(); ++it)
        {
            if (it->adaptee != nullptr && it->adaptee->kind() == BLOCK)
            {
                model::Block* adaptee = static_cast<model::Block*>(it->adaptee);
                GraphicsAdapter::relink(controller, adaptee, children);
                LinkAdapter::reverse_relink(controller, adaptee, it->index, children);
            }
        }
        for (auto it = childrenToUpdate.begin(); it != childrenToUpdate.end(); ++it)
        {
            if (it->adaptee != nullptr && it->adaptee->kind() == LINK)
            {
                model::Link* adaptee = static_cast<model::Link*>(it->adaptee);
                LinkAdapter::relink(controller, adaptee, children);
                GraphicsAdapter::reverse_relink(controller, adaptee, it->index, children);
            }
        }

        // set the children after update
        controller.setObjectProperty(adaptee, CHILDREN, children);

        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        std::string version;
        if (adaptor.getAdaptee()->kind() == BLOCK)
        {
            model::Block* adaptee = static_cast<model::Block*>(adaptor.getAdaptee());

            ScicosID parentDiagram;
            controller.getObjectProperty(adaptee, PARENT_DIAGRAM, parentDiagram);
            controller.getObjectProperty(parentDiagram, DIAGRAM, VERSION_NUMBER, version);
        }
        else
        {
            model::Diagram *adaptee = static_cast<model::Diagram *>(adaptor.getAdaptee());
            controller.getObjectProperty(adaptee, VERSION_NUMBER, version);
        }

        return new types::String(version.data());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong dimension for field %s: %d-by-%d expected.\n"), "version", 1, 1);
                return false;
            }

            if (adaptor.getAdaptee()->kind() != DIAGRAM)
            {
                // version field is not present on the model for non-diagram ; let's pass it !
                return true;
            }
            model::Diagram* adaptee = static_cast<model::Diagram*>(adaptor.getAdaptee());

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string version (c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee, VERSION_NUMBER, version);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("Wrong size for field %s: at least %d-by-%d expected.\n"), "version", 1, 1);
                return false;
            }

            if (adaptor.getAdaptee()->kind() != DIAGRAM)
            {
                get_or_allocate_logger()->log(LOG_ERROR, _("\"%s\" is a read-only field.\n"), "version");
                return false;
            }
            model::Diagram* adaptee = static_cast<model::Diagram*>(adaptor.getAdaptee());

            std::string version;
            controller.setObjectProperty(adaptee, VERSION_NUMBER, version);
            return true;
        }

        get_or_allocate_logger()->log(LOG_ERROR, _("Wrong type for field %s.%s: Real matrix expected.\n"), "graphics", "orig");
        return false;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getContribContent();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setContribContent(v);
        return true;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(const Controller& c, org_scilab_modules_scicos::model::BaseObject* adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::BaseObject>(c, adaptee),
    contrib_content(default_value<types::List>())
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::reserve_properties(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
        property<DiagramAdapter>::shrink_to_fit();
    }
}

DiagramAdapter::DiagramAdapter(const DiagramAdapter& adapter) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::BaseObject>(adapter),
    contrib_content(reference_value(adapter.contrib_content))
{
}

DiagramAdapter::~DiagramAdapter()
{
    contrib_content->DecreaseRef();
    contrib_content->killMe();
}

std::wstring DiagramAdapter::getTypeStr() const
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr() const
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContribContent() const
{
    return contrib_content;
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    // v and contrib_content should not be nullptr

    types::InternalType* temp = contrib_content;

    v->IncreaseRef();
    contrib_content = v;

    temp->DecreaseRef();
    temp->killMe();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
