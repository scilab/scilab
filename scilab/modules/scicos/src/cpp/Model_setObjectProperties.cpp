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
#include <cstring> // for memcpy

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

/* helper function to encode simple string EXPRS */
static std::vector<double> encode_string_vector(const std::vector<std::string>& v)
{
    std::vector<double> ret;

    // header
    ret.push_back(sci_strings);

    // serialize as a Scilab vector
    ret.push_back(2); // MxN
    ret.push_back(v.size()); // M
    ret.push_back(1); // N

    // reserve some space to store the length of each string (including the null terminating character)
    ret.resize(ret.size() + v.size());

    // store the index and the null terminated UTF-8 strings
    size_t stringOffset = 0;
    for (size_t i = 0; i < v.size(); ++i)
    {
        const std::string& str = v[i];
        // length as a 64bit index (as we store on a double vector)
        size_t len = ((str.size() + 1) * sizeof(char) + sizeof(double) - 1) / sizeof(double);

        // insert the offset
        auto it = ret.begin() + 4 + i;
        stringOffset += len;
        *it = stringOffset;

        // reserve some space for the string
        size_t size = ret.size();
        ret.resize(size + len);

        // copy the UTF-8 encoded values
        std::memcpy(ret.data() + size, str.data(), str.size());
    }

    return ret;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

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
                return o->setFiring(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

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
                return o->setSimFunctionApi(v);
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
                return o->setDebugLevel(v);
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
                return o->setColor(v);
            case KIND:
                return o->setKind(v);
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
                return o->setKind(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

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
                return o->setImplicit(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(baseObject);
        switch (p)
        {
            case PARENT_DIAGRAM:
                return o->setParentDiagram(v);
            case PARENT_BLOCK:
                return o->setParentBlock(v);
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
                return o->setParentDiagram(v);
            case PARENT_BLOCK:
                return o->setParentBlock(v);
            case PORT_REFERENCE:
                return o->setPortReference(v);
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
        model::Link* o = static_cast<model::Link*>(baseObject);
        switch (p)
        {
            case PARENT_DIAGRAM:
                return o->setParentDiagram(v);
            case PARENT_BLOCK:
                return o->setParentBlock(v);
            case SOURCE_PORT:
                return o->setSourcePort(v);
            case DESTINATION_PORT:
                return o->setDestinationPort(v);
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
                return o->setSourceBlock(v);
            case CONNECTED_SIGNALS:
                return o->setConnectedSignals(std::vector<ScicosID> (1, v));
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(baseObject);
        switch (p)
        {
            case DESCRIPTION:
                return o->setDescription(v);
            case FONT:
                return o->setFont(v);
            case FONT_SIZE:
                return o->setFontSize(v);
            case STYLE:
                return o->setStyle(v);
            case UID:
                return o->setUID(v);
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
                return o->setInterfaceFunction(v);
            case SIM_FUNCTION_NAME:
                return o->setSimFunctionName(v);
            case SIM_BLOCKTYPE:
                return o->setSimBlocktype(v);
            case STYLE:
                return o->setStyle(v);
            case DESCRIPTION:
                return o->setDescription(v);
            case LABEL:
                return o->setLabel(v);
            case UID:
                return o->setUID(v);
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
                return o->setTitle(v);
            case PATH:
                return o->setPath(v);
            case VERSION_NUMBER:
                return o->setVersionNumber(v);
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
                return o->setStyle(v);
            case LABEL:
                return o->setLabel(v);
            case UID:
                return o->setUID(v);
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
                return o->setStyle(v);
            case LABEL:
                return o->setLabel(v);
            case UID:
                return o->setUID(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<double>& v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(baseObject);
        switch (p)
        {
            case GEOMETRY:
                return o->setGeometry(v);
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
                return o->setGeometry(v);
            case EXPRS:
                return o->setExprs(v);
            case STATE:
                return o->setState(v);
            case DSTATE:
                return o->setDState(v);
            case ODSTATE:
                return o->setODState(v);
            case RPAR:
                return o->setRpar(v);
            case OPAR:
                return o->setOpar(v);
            case EQUATIONS:
                return o->setEquations(v);
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
                return o->setProperties(v);
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
                return o->setControlPoints(v);
            case THICK:
                return o->setThick(v);
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
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<int>& v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

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
                return o->setSimDepUT(v);
            case NZCROSS:
                return o->setNZcross(v);
            case NMODE:
                return o->setNMode(v);
            case IPAR:
                return o->setIpar(v);
            case COLOR:
                return o->setChildrenColor(v);
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
                return o->setColor(v);
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
                return o->setDataType(this, v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<bool>& /*v*/)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

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
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<std::string>& v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

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
                return o->setContext(v);
            case EXPRS:
                return o->setExprs(encode_string_vector(v));
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
                return o->setContext(v);
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
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<ScicosID>& v)
{
    model::BaseObject* baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

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
                return o->setIn(v);
            case OUTPUTS:
                return o->setOut(v);
            case EVENT_INPUTS:
                return o->setEin(v);
            case EVENT_OUTPUTS:
                return o->setEout(v);
            case CHILDREN:
                return o->setChildren(v);
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
                return o->setChildren(v);
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
                return o->setConnectedSignals(v);
            default:
                break;
        }
    }
    return FAIL;
}

} /* namespace org_scilab_modules_scicos */


