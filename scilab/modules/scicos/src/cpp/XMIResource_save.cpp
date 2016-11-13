/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016-2016 - Scilab Enterprises - Clement DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "XMIResource.hxx"
#include "base64.hxx"

#include <string>
#include <sstream>
#include <vector>
#include <cstdio>

extern "C" {
#include "sci_types.h"

#include <libxml/xmlwriter.h>
}

namespace org_scilab_modules_scicos
{

int XMIResource::save(const char* uri)
{
    int status;

    xmlTextWriterPtr writer = xmlNewTextWriterFilename(uri, 0);
    if (writer == NULL)
    {
        return -1;
    }

    status = xmlTextWriterSetIndent(writer, 1);
    if (status == -1)
    {
        xmlFreeTextWriter(writer);
        return status;
    }

    status = xmlTextWriterStartDocument(writer, "1.0", "UTF-8", NULL);
    if (status == -1)
    {
        xmlFreeTextWriter(writer);
        return status;
    }

    status = writeDiagram(writer);
    if (status == -1)
    {
        xmlFreeTextWriter(writer);
        return status;
    }

    status = xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
    return status;
}

static bool is_empty_matrix(const std::vector<double>& v)
{
    // v == {1, 2, 0, 0, 0}
    return v.size() == 5 && v[0] == sci_matrix && v[1] == 2. && v[2] == 0. && v[3] == 0. && v[4] == 0.;
}

static bool is_empty_list(const std::vector<double>& v)
{
    // v == {15, 0}
    return v.size() == 2 && v[0] == sci_list && v[1] == 0.;
}

static bool is_string_vector(const std::vector<double>& v)
{
    return v.size() > 2 && v[0] == sci_strings && v[1] != 0;
}

static std::string to_string(int v)
{
    return std::to_string(v);
}

static std::string to_string(double v)
{
    if (std::trunc(v) == v)
    {
        return to_string((int) v);
    }

    std::string str(15, '\0');
	// std::snprintf(const_cast<char*>(str.data()), str.size(), "%.6E", v);
    std::sprintf(const_cast<char*>(str.data()), "%.6E", v);
    return str;
}

static int writeBase64(xmlTextWriterPtr writer, const char* name, const std::vector<double>& v)
{
    int status;

    // convert values as big endian (network endianess)
    // convert the big endian value to Base64
    std::string content = base64::encode(v);

    // write the XML data
    status = xmlTextWriterStartElement(writer, BAD_CAST(name));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("base64"), BAD_CAST(content.data()));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterEndElement(writer);
    return status;
}


int XMIResource::writePoint(xmlTextWriterPtr writer, double x, double y)
{
    int status;


    status = xmlTextWriterStartElement(writer, BAD_CAST("controlPoint"));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("x"), BAD_CAST(to_string(x).c_str()));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("y"), BAD_CAST(to_string(y).c_str()));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

int XMIResource::writeGeometry(xmlTextWriterPtr writer, ScicosID id, kind_t kind)
{
    int status;

    status = xmlTextWriterStartElement(writer, BAD_CAST("geometry"));
    if (status == -1)
    {
        return status;
    }

    std::vector<double> doubleArrayValue;
    controller.getObjectProperty(id, kind, GEOMETRY, doubleArrayValue);
    unsigned int i = 0;
    if (doubleArrayValue.size() > i && doubleArrayValue[i])
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("x"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    i++;
    if (doubleArrayValue.size() > i && doubleArrayValue[i])
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("y"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    i++;
    if (doubleArrayValue.size() > i && doubleArrayValue[i])
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("width"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    i++;
    if (doubleArrayValue.size() > i && doubleArrayValue[i])
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("height"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

int XMIResource::writeAbstractLayer(xmlTextWriterPtr writer, ScicosID id, kind_t kind)
{
    int status = 1;

    std::vector<std::string> context;
    controller.getObjectProperty(id, kind, DIAGRAM_CONTEXT, context);
    for (const std::string& c : context)
    {
        status = xmlTextWriterStartElement(writer, BAD_CAST("context"));
        if (status == -1)
        {
            return status;
        }

        if (!c.empty())
        {
            status = xmlTextWriterWriteCDATA(writer, BAD_CAST(c.c_str()));
            if (status == -1)
            {
                return status;
            }
        }

        status = xmlTextWriterEndElement(writer);
        if (status == -1)
        {
            return status;
        }
    }

    std::vector<ScicosID> children;
    controller.getObjectProperty(id, kind, CHILDREN, children);
    for (ScicosID child : children)
    {
        kind_t kind = controller.getKind(child);
        switch (kind)
        {
            case BLOCK:
                status = writeBlock(writer, child);
                break;
            case LINK:
                status = writeLink(writer, child);
                break;
            case ANNOTATION:
                status = writeAnnotation(writer, child);
                break;
            default:
                status =  -1;
                break;
        }

        if (status == -1)
        {
            return status;
        }
    }

    return status;
}

int XMIResource::writeAbstractBaseObject(xmlTextWriterPtr writer, ScicosID id, kind_t kind)
{
    int status;

    std::string uid;
    controller.getObjectProperty(id, kind, UID, uid);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("uid"), BAD_CAST(uid.c_str()));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("parentDiagram"), BAD_CAST("/"));
    if (status == -1)
    {
        return status;
    }

    /* parent / child relation is not serialized as this relation is the XML tree */
    return status;
}

int XMIResource::writeDiagram(xmlTextWriterPtr writer)
{
    int status;

    status = xmlTextWriterStartElementNS(writer, BAD_CAST("xcos"), BAD_CAST("Diagram"), BAD_CAST("org.scilab.modules.xcos"));
    if (status == -1)
    {
        return status;
    }

    /*
     * Write default xmlns
     */
    status = xmlTextWriterWriteAttributeNS(writer, BAD_CAST("xmi"), BAD_CAST("version"), BAD_CAST("http://www.omg.org/XMI"), BAD_CAST("2.0"));
    if (status == -1)
    {
        return status;
    }
    status = xmlTextWriterWriteAttributeNS(writer, BAD_CAST("xsi"), BAD_CAST("schemaLocation"), BAD_CAST("http://www.w3.org/2001/XMLSchema-instance"), BAD_CAST("org.scilab.modules.xcos Xcos.xcore#/EPackage"));
    if (status == -1)
    {
        return status;
    }

    /*
     * Diagram values
     */
    std::string strValue;
    controller.getObjectProperty(root, DIAGRAM, TITLE, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("title"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(root, DIAGRAM, PATH, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("path"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    int intValue;
    controller.getObjectProperty(root, DIAGRAM, DEBUG_LEVEL, intValue);
    strValue = to_string(intValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("debugLevel"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(root, DIAGRAM, VERSION_NUMBER, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("version"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    status = writeAbstractLayer(writer, root, DIAGRAM);
    if (status == -1)
    {
        return status;
    }

    status = writeSimulationConfig(writer, root);
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

int XMIResource::writeSimulationConfig(xmlTextWriterPtr writer, ScicosID id)
{
    int status;

    status = xmlTextWriterStartElement(writer, BAD_CAST("properties"));
    if (status == -1)
    {
        return status;
    }

    std::vector<double> doubleArrayValue;
    controller.getObjectProperty(id, DIAGRAM, PROPERTIES, doubleArrayValue);
    unsigned int i = 0;
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("finalTime"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    i++;
    if (i >= doubleArrayValue.size())
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("absoluteTime"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    i++;
    if (i >= doubleArrayValue.size())
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("relativeTolerance"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    i++;
    if (i >= doubleArrayValue.size())
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("absoluteTolerance"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    i++;
    if (i >= doubleArrayValue.size())
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("deltaT"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    i++;
    if (i >= doubleArrayValue.size())
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("realtimeScale"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    i++;
    if (i >= doubleArrayValue.size())
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("solver"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    i++;
    if (i >= doubleArrayValue.size())
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("deltaH"), BAD_CAST(to_string(doubleArrayValue[i]).c_str()));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

int XMIResource::writeBlock(xmlTextWriterPtr writer, ScicosID id)
{
    int status;

    status = xmlTextWriterStartElement(writer, BAD_CAST("child"));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("xsi:type"), BAD_CAST("xcos:Block"));
    if (status == -1)
    {
        return status;
    }

    status = writeAbstractBaseObject(writer, id, BLOCK);
    if (status == -1)
    {
        return status;
    }

    std::string strValue;
    controller.getObjectProperty(id, BLOCK, DESCRIPTION, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("description"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, BLOCK, LABEL, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("label"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, BLOCK, STYLE, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("style"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, BLOCK, INTERFACE_FUNCTION, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("interfaceFunction"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, BLOCK, SIM_FUNCTION_NAME, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("functionName"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    int intValue;
    controller.getObjectProperty(id, BLOCK, SIM_FUNCTION_API, intValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("functionAPI"), BAD_CAST(to_string(intValue).c_str()));
    if (status == -1)
    {
        return status;
    }

    std::vector<int> intArrayValue;
    controller.getObjectProperty(id, BLOCK, SIM_DEP_UT, intArrayValue);
    unsigned int i = 0;
    if (intArrayValue.size() > i && intArrayValue[i])
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("dependsOnU"), BAD_CAST(to_string(intArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }
    i++;
    if (intArrayValue.size() > i && intArrayValue[i])
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("dependsOnT"), BAD_CAST(to_string(intArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    strValue.clear();
    controller.getObjectProperty(id, BLOCK, SIM_BLOCKTYPE, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("blocktype"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    status = writeAbstractLayer(writer, id, BLOCK);
    if (status == -1)
    {
        return status;
    }

    status = writeGeometry(writer, id, BLOCK);
    if (status == -1)
    {
        return status;
    }

    std::vector<double> doubleArrayValue;
    controller.getObjectProperty(id, BLOCK, EXPRS, doubleArrayValue);
    if (is_empty_matrix(doubleArrayValue))
    {
        // we do not serialize the default value
    }
    else if (is_string_vector(doubleArrayValue))
    {
        // if this is a string the expression is used
        std::vector<std::string> values;
        controller.getObjectProperty(id, BLOCK, EXPRS, values);

        for (const std::string& s : values)
        {
            status = xmlTextWriterStartElement(writer, BAD_CAST("expression"));
            if (status == -1)
            {
                return status;
            }
            status = xmlTextWriterWriteCDATA(writer, BAD_CAST(s.c_str()));
            if (status == -1)
            {
                return status;
            }

            status = xmlTextWriterEndElement(writer);
            if (status == -1)
            {
                return status;
            }
        }
    }
    else
    {
        // encode the value as base64 binary
        status = writeBase64(writer, "exprs", doubleArrayValue);
        if (status == -1)
        {
            return status;
        }
    }

    intArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, NZCROSS, intArrayValue);
    for (int i : intArrayValue)
    {
        status = xmlTextWriterWriteElement(writer, BAD_CAST("nzcross"), BAD_CAST(to_string(i).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    intArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, NMODE, intArrayValue);
    for (int i : intArrayValue)
    {
        status = xmlTextWriterWriteElement(writer, BAD_CAST("nmode"), BAD_CAST(to_string(i).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    doubleArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, EQUATIONS, doubleArrayValue);
    if (!doubleArrayValue.empty() && !is_empty_list(doubleArrayValue))
    {
        status = writeBase64(writer, "equations", doubleArrayValue);
        if (status == -1)
        {
            return status;
        }
    }

    std::vector<ScicosID> scicosIDArrayValue;
    controller.getObjectProperty(id, BLOCK, INPUTS, scicosIDArrayValue);
    for (ScicosID p : scicosIDArrayValue)
    {
        status = writePort(writer, INPUTS, p);
        if (status == -1)
        {
            return status;
        }
    }

    scicosIDArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, OUTPUTS, scicosIDArrayValue);
    for (ScicosID p : scicosIDArrayValue)
    {
        status = writePort(writer, OUTPUTS, p);
        if (status == -1)
        {
            return status;
        }
    }

    controller.getObjectProperty(id, BLOCK, EVENT_INPUTS, scicosIDArrayValue);
    for (ScicosID p : scicosIDArrayValue)
    {
        status = writePort(writer, EVENT_INPUTS, p);
        if (status == -1)
        {
            return status;
        }
    }

    controller.getObjectProperty(id, BLOCK, EVENT_OUTPUTS, scicosIDArrayValue);
    for (ScicosID p : scicosIDArrayValue)
    {
        status = writePort(writer, EVENT_OUTPUTS, p);
        if (status == -1)
        {
            return status;
        }
    }

    doubleArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, RPAR, doubleArrayValue);
    for (double d : doubleArrayValue)
    {
        status = xmlTextWriterWriteElement(writer, BAD_CAST("rpar"), BAD_CAST(to_string(d).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    intArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, IPAR, intArrayValue);
    for (int i : intArrayValue)
    {
        status = xmlTextWriterWriteElement(writer, BAD_CAST("ipar"), BAD_CAST(to_string(i).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    doubleArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, OPAR, doubleArrayValue);
    if (!is_empty_list(doubleArrayValue))
    {
        status = writeBase64(writer, "opar", doubleArrayValue);
        if (status == -1)
        {
            return status;
        }
    }

    doubleArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, STATE, doubleArrayValue);
    for (double d : doubleArrayValue)
    {
        status = xmlTextWriterWriteElement(writer, BAD_CAST("state"), BAD_CAST(to_string(d).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    doubleArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, DSTATE, doubleArrayValue);
    for (double d : doubleArrayValue)
    {
        status = xmlTextWriterWriteElement(writer, BAD_CAST("dstate"), BAD_CAST(to_string(d).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    doubleArrayValue.clear();
    controller.getObjectProperty(id, BLOCK, ODSTATE, doubleArrayValue);
    if (!is_empty_list(doubleArrayValue))
    {
        status = writeBase64(writer, "odstate", doubleArrayValue);
        if (status == -1)
        {
            return status;
        }
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

int XMIResource::writePort(xmlTextWriterPtr writer, enum object_properties_t container, ScicosID id)
{
    int status;

    std::string element;
    switch (container)
    {
        case INPUTS:
            element = "in";
            break;
        case OUTPUTS:
            element = "out";
            break;
        case EVENT_INPUTS:
            element = "ein";
            break;
        case EVENT_OUTPUTS:
            element = "eout";
            break;
        default:
            return -1;
    }

    status = xmlTextWriterStartElement(writer, BAD_CAST(element.c_str()));
    if (status == -1)
    {
        return status;
    }

    std::string strValue;
    controller.getObjectProperty(id, PORT, UID, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("uid"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    ScicosID idValue;
    controller.getObjectProperty(id, PORT, SOURCE_BLOCK, idValue);
    controller.getObjectProperty(idValue, BLOCK, UID, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("sourceBlock"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    const std::vector<std::string> elementName = {"portUndefined", "in", "out", "ein", "eout"};
    int portKind;
    controller.getObjectProperty(id, PORT, PORT_KIND, portKind);
    if (portKind < 0 && elementName.size() <= (unsigned int) portKind)
    {
        return -1;
    }
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("kind"), BAD_CAST(elementName[portKind].c_str()));
    if (status == -1)
    {
        return status;
    }

    controller.getObjectProperty(id, PORT, CONNECTED_SIGNALS, idValue);
    if (idValue != 0)
    {
        strValue.clear();
        controller.getObjectProperty(idValue, LINK, UID, strValue);

        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("connectedSignal"), BAD_CAST(strValue.c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    strValue.clear();
    controller.getObjectProperty(id, PORT, STYLE, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("style"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, PORT, LABEL, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("label"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    std::vector<double> doubleArrayValue;
    controller.getObjectProperty(id, BLOCK, DATATYPE, doubleArrayValue);
    for (double d : doubleArrayValue)
    {
        status = xmlTextWriterWriteElement(writer, BAD_CAST("datatype"), BAD_CAST(to_string(d).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

int XMIResource::writeLink(xmlTextWriterPtr writer, ScicosID id)
{
    int status;

    status = xmlTextWriterStartElement(writer, BAD_CAST("child"));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("xsi:type"), BAD_CAST("xcos:Link"));
    if (status == -1)
    {
        return status;
    }

    status = writeAbstractBaseObject(writer, id, LINK);
    if (status == -1)
    {
        return status;
    }

    ScicosID idValue;
    std::string strValue;
    controller.getObjectProperty(id, LINK, SOURCE_PORT, idValue);
    if (idValue != 0)
    {
        strValue.clear();
        controller.getObjectProperty(idValue, PORT, UID, strValue);

        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("sourcePort"), BAD_CAST(strValue.c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    controller.getObjectProperty(id, LINK, DESTINATION_PORT, idValue);
    if (idValue != 0)
    {
        strValue.clear();
        controller.getObjectProperty(idValue, PORT, UID, strValue);

        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("destinationPort"), BAD_CAST(strValue.c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    strValue.clear();
    controller.getObjectProperty(id, LINK, STYLE, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("style"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, LINK, LABEL, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("label"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    int intValue;
    controller.getObjectProperty(id, LINK, COLOR, intValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("color"), BAD_CAST(to_string(intValue).c_str()));
    if (status == -1)
    {
        return status;
    }

    std::vector<int> intArrayValue;
    controller.getObjectProperty(id, LINK, THICK, intArrayValue);
    unsigned int i = 0;
    if (i < intArrayValue.size())
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("lineWidth"), BAD_CAST(to_string(intArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }
    i++;
    if (i < intArrayValue.size())
    {
        status = xmlTextWriterWriteAttribute(writer, BAD_CAST("lineHeight"), BAD_CAST(to_string(intArrayValue[i]).c_str()));
        if (status == -1)
        {
            return status;
        }
    }

    status = writeGeometry(writer, id, LINK);
    if (status == -1)
    {
        return status;
    }

    std::vector<double> dblArrayValue;
    controller.getObjectProperty(id, LINK, CONTROL_POINTS, dblArrayValue);
    for (unsigned int i = 0; i < dblArrayValue.size(); i += 2)
    {
        status = writePoint(writer, dblArrayValue[i], dblArrayValue[i + 1]);
        if (status == -1)
        {
            return status;
        }
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

int XMIResource::writeAnnotation(xmlTextWriterPtr writer, ScicosID id)
{
    int status;

    status = xmlTextWriterStartElement(writer, BAD_CAST("child"));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("xsi:type"), BAD_CAST("xcos:Annotation"));
    if (status == -1)
    {
        return status;
    }

    status = writeAbstractBaseObject(writer, id, ANNOTATION);
    if (status == -1)
    {
        return status;
    }

    std::string strValue;
    controller.getObjectProperty(id, ANNOTATION, DESCRIPTION, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("description"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, ANNOTATION, FONT, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("font"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, ANNOTATION, FONT_SIZE, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("fontSize"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    strValue.clear();
    controller.getObjectProperty(id, ANNOTATION, STYLE, strValue);
    status = xmlTextWriterWriteAttribute(writer, BAD_CAST("style"), BAD_CAST(strValue.c_str()));
    if (status == -1)
    {
        return status;
    }

    status = xmlTextWriterEndElement(writer);
    if (status == -1)
    {
        return status;
    }

    return status;
}

} /* namespace org_scilab_modules_xcos */
