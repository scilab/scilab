/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

namespace org_scilab_modules_scicos
{

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
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
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
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

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            case SIM_FUNCTION_NAME:
                o->getSimFunctionName(v);
                return true;
            case STYLE:
                o->getStyle(v);
                return true;
            case LABEL:
                o->getLabel(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            case STYLE:
                o->getStyle(v);
                return true;
            case LABEL:
                o->getLabel(v);
                return true;
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            case PARENT_DIAGRAM:
                v = o->getParentDiagram();
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
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            case PARENT_DIAGRAM:
                v = o->getParentDiagram();
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));

    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            case PARENT_DIAGRAM:
                v = o->getParentDiagram();
                return true;
            case SOURCE_PORT:
                v = o->getSourcePort();
                return true;
            case DESTINATION_PORT:
                v = o->getDestinationPort();
                return true;
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            case CONNECTED_SIGNALS:
                v = o->getConnectedSignals().front();
                return true;
            default:
                break;
        }
    }
    else
    {
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<double>& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
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
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            case GEOMETRY:
                o->getGeometry(v);
                return true;
            case ANGLE:
                o->getAngle(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
    }
    else
    {
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<int>& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<std::string>& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            case EXPRS:
                o->getExprs(v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<ScicosID>& v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
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
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            case CHILDREN:
                v = o->getChildren();
                return true;
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
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
