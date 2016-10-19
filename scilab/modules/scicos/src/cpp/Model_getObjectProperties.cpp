/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
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

#include "Model.hxx"
#include "utilities.hxx"

#include "model/BaseObject.hxx"
#include "model/Annotation.hxx"
#include "model/Diagram.hxx"
#include "model/Block.hxx"
#include "model/Link.hxx"
#include "model/Port.hxx"

extern "C" {
#include "sci_types.h"
}

namespace org_scilab_modules_scicos
{

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, double& v)  const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(baseObject);
        switch (p)
        {
            case FIRING:
                o->getFiring(v);
                return true;
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, int& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(baseObject);
        switch (p)
        {
            case SIM_FUNCTION_API:
                o->getSimFunctionApi(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(baseObject);
        switch (p)
        {
            case DEBUG_LEVEL:
                o->getDebugLevel(v);
                return true;
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(baseObject);
        switch (p)
        {
            case COLOR:
                o->getColor(v);
                return true;
            case KIND:
                o->getKind(v);
                return true;
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(baseObject);
        switch (p)
        {
            case PORT_KIND:
                o->getKind(v);
                return true;
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, bool& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(baseObject);
        switch (p)
        {
            case IMPLICIT:
                o->getImplicit(v);
                return true;
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, std::string& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(baseObject);
        switch (p)
        {
            case DESCRIPTION:
                o->getDescription(v);
                return true;
            case FONT:
                o->getFont(v);
                return true;
            case FONT_SIZE:
                o->getFontSize(v);
                return true;
            case STYLE:
                o->getStyle(v);
                return true;
            case UID:
                o->getUID(v);
                return true;
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(baseObject);
        switch (p)
        {
            case INTERFACE_FUNCTION:
                o->getInterfaceFunction(v);
                return true;
            case SIM_FUNCTION_NAME:
                o->getSimFunctionName(v);
                return true;
            case SIM_BLOCKTYPE:
                o->getSimBlocktype(v);
                return true;
            case STYLE:
                o->getStyle(v);
                return true;
            case DESCRIPTION:
                o->getDescription(v);
                return true;
            case LABEL:
                o->getLabel(v);
                return true;
            case UID:
                o->getUID(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(baseObject);
        switch (p)
        {
            case TITLE:
                o->getTitle(v);
                return true;
            case PATH:
                o->getPath(v);
                return true;
            case VERSION_NUMBER:
                o->getVersionNumber(v);
                return true;
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(baseObject);
        switch (p)
        {
            case STYLE:
                o->getStyle(v);
                return true;
            case LABEL:
                o->getLabel(v);
                return true;
            case UID:
                o->getUID(v);
                return true;
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(baseObject);
        switch (p)
        {
            case STYLE:
                o->getStyle(v);
                return true;
            case LABEL:
                o->getLabel(v);
                return true;
            case UID:
                o->getUID(v);
                return true;
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, ScicosID& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(baseObject);
        switch (p)
        {
            case PARENT_DIAGRAM:
                o->getParentDiagram(v);
                return true;
            case PARENT_BLOCK:
                o->getParentBlock(v);
                return true;
            case RELATED_TO:
                v = o->getRelatedTo();
                return true;
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(baseObject);
        switch (p)
        {
            case PARENT_DIAGRAM:
                o->getParentDiagram(v);
                return true;
            case PARENT_BLOCK:
                o->getParentBlock(v);
                return true;
            case PORT_REFERENCE:
                o->getPortReference(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {

    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(baseObject);
        switch (p)
        {
            case PARENT_DIAGRAM:
                o->getParentDiagram(v);
                return true;
            case PARENT_BLOCK:
                o->getParentBlock(v);
                return true;
            case SOURCE_PORT:
                o->getSourcePort(v);
                return true;
            case DESTINATION_PORT:
                o->getDestinationPort(v);
                return true;
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(baseObject);
        switch (p)
        {
            case SOURCE_BLOCK:
                o->getSourceBlock(v);
                return true;
            case CONNECTED_SIGNALS:
                v = o->getConnectedSignals().front();
                return true;
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<double>& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(baseObject);
        switch (p)
        {
            case GEOMETRY:
                o->getGeometry(v);
                return true;
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(baseObject);
        switch (p)
        {
            case GEOMETRY:
                o->getGeometry(v);
                return true;
            case EXPRS:
                o->getExprs(v);
                return true;
            case STATE:
                o->getState(v);
                return true;
            case DSTATE:
                o->getDState(v);
                return true;
            case ODSTATE:
                o->getODState(v);
                return true;
            case RPAR:
                o->getRpar(v);
                return true;
            case OPAR:
                o->getOpar(v);
                return true;
            case EQUATIONS:
                o->getEquations(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(baseObject);
        switch (p)
        {
            case PROPERTIES:
                o->getProperties(v);
                return true;
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(baseObject);
        switch (p)
        {
            case CONTROL_POINTS:
                o->getControlPoints(v);
                return true;
            case THICK:
                o->getThick(v);
                return true;
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<int>& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(baseObject);
        switch (p)
        {
            case SIM_DEP_UT:
                o->getSimDepUT(v);
                return true;
            case NZCROSS:
                o->getNZcross(v);
                return true;
            case NMODE:
                o->getNMode(v);
                return true;
            case IPAR:
                o->getIpar(v);
                return true;
            case COLOR:
                o->getChildrenColor(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(baseObject);
        switch (p)
        {
            case COLOR:
                o->getColor(v);
                return true;
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(baseObject);
        switch (p)
        {
            case DATATYPE:
                o->getDataType(v);
                return true;
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<bool>& /*v*/) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<std::string>& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(baseObject);
        switch (p)
        {
            case DIAGRAM_CONTEXT:
                o->getContext(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(baseObject);
        switch (p)
        {
            case DIAGRAM_CONTEXT:
                o->getContext(v);
                return true;
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<ScicosID>& v) const
{
    model::BaseObject* baseObject = object;
    if (baseObject == nullptr)
    {
        return false;
    }
    kind_t k = object->kind();

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(baseObject);
        switch (p)
        {
            case INPUTS:
                o->getIn(v);
                return true;
            case OUTPUTS:
                o->getOut(v);
                return true;
            case EVENT_INPUTS:
                o->getEin(v);
                return true;
            case EVENT_OUTPUTS:
                o->getEout(v);
                return true;
            case CHILDREN:
                o->getChildren(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(baseObject);
        switch (p)
        {
            case CHILDREN:
                o->getChildren(v);
                return true;
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(baseObject);
        switch (p)
        {
            case CONNECTED_SIGNALS:
                v = o->getConnectedSignals();
                return true;
            default:
                break;
        }
    }
    return false;
}

} /* namespace org_scilab_modules_scicos */
