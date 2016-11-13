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

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib> // for atoi and atof
#include <cstring> // for strcmp and strchr

extern "C"
{
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlreader.h>

#include "sciprint.h"
}

namespace org_scilab_modules_scicos
{

/**
 * Display on the Scilab console
 */
void console_print(void *, const char *msg, ...) LIBXML_ATTR_FORMAT(2, 3);
void console_print(void *, const char *msg, ...)
{
    //print the message
    va_list ap;
    va_start(ap, msg);
    scivprint(msg, ap);
    va_end(ap);
}

/**
 * Helper class to set / reset the XML parser state
 */
struct LibXML2State
{
    LibXML2State()
    {
        xmlGenericErrorFunc f = &console_print;
        initGenericErrorDefaultFunc(&f);
    }
    ~LibXML2State()
    {
        initGenericErrorDefaultFunc(nullptr);
    }
};

int XMIResource::load(const char* uri)
{
    int ret;

    LibXML2State state;

    /*
     * Allocate the reader object, this API is used as it is simpler to use than SAX2 :
     *  * we have direct access to a node object
     *  * Strings are interned by libxml2
     *  * partial SAX2 callbacks are not supported by libxml2
     */
    xmlTextReaderPtr reader;
    /* resolve xinclude and intern strings */
    reader = xmlReaderForFile(uri, NULL, XML_PARSE_XINCLUDE | XML_PARSE_COMPACT);

    /*
     * Intern strings to speedup comparaison, this table can be generated using XPath on xcos.ecore .
     */
    constXcosNames[e_Annotation] = xmlTextReaderConstString(reader, BAD_CAST ("Annotation"));
    constXcosNames[e_BaseObject] = xmlTextReaderConstString(reader, BAD_CAST ("BaseObject"));
    constXcosNames[e_Block] = xmlTextReaderConstString(reader, BAD_CAST ("Block"));
    constXcosNames[e_CompiledRepresentation] = xmlTextReaderConstString(reader, BAD_CAST ("CompiledRepresentation"));
    constXcosNames[e_Diagram] = xmlTextReaderConstString(reader, BAD_CAST ("Diagram"));
    constXcosNames[e_Geometry] = xmlTextReaderConstString(reader, BAD_CAST ("Geometry"));
    constXcosNames[e_Layer] = xmlTextReaderConstString(reader, BAD_CAST ("Layer"));
    constXcosNames[e_Link] = xmlTextReaderConstString(reader, BAD_CAST ("Link"));
    constXcosNames[e_Point] = xmlTextReaderConstString(reader, BAD_CAST ("Point"));
    constXcosNames[e_Port] = xmlTextReaderConstString(reader, BAD_CAST ("Port"));
    constXcosNames[e_PortKind] = xmlTextReaderConstString(reader, BAD_CAST ("PortKind"));
    constXcosNames[e_SimulationConfig] = xmlTextReaderConstString(reader, BAD_CAST ("SimulationConfig"));
    constXcosNames[e_absoluteTolerance] = xmlTextReaderConstString(reader, BAD_CAST ("absoluteTolerance"));
    constXcosNames[e_base64] = xmlTextReaderConstString(reader, BAD_CAST ("base64"));
    constXcosNames[e_blocktype] = xmlTextReaderConstString(reader, BAD_CAST ("blocktype"));
    constXcosNames[e_child] = xmlTextReaderConstString(reader, BAD_CAST ("child"));
    constXcosNames[e_color] = xmlTextReaderConstString(reader, BAD_CAST ("color"));
    constXcosNames[e_connectedSignal] = xmlTextReaderConstString(reader, BAD_CAST ("connectedSignal"));
    constXcosNames[e_context] = xmlTextReaderConstString(reader, BAD_CAST ("context"));
    constXcosNames[e_controlPoint] = xmlTextReaderConstString(reader, BAD_CAST ("controlPoint"));
    constXcosNames[e_datatype] = xmlTextReaderConstString(reader, BAD_CAST ("datatype"));
    constXcosNames[e_debugLevel] = xmlTextReaderConstString(reader, BAD_CAST ("debugLevel"));
    constXcosNames[e_deltaH] = xmlTextReaderConstString(reader, BAD_CAST ("deltaH"));
    constXcosNames[e_deltaT] = xmlTextReaderConstString(reader, BAD_CAST ("deltaT"));
    constXcosNames[e_dependsOnT] = xmlTextReaderConstString(reader, BAD_CAST ("dependsOnT"));
    constXcosNames[e_dependsOnU] = xmlTextReaderConstString(reader, BAD_CAST ("dependsOnU"));
    constXcosNames[e_description] = xmlTextReaderConstString(reader, BAD_CAST ("description"));
    constXcosNames[e_destinationPort] = xmlTextReaderConstString(reader, BAD_CAST ("destinationPort"));
    constXcosNames[e_dstate] = xmlTextReaderConstString(reader, BAD_CAST ("dstate"));
    constXcosNames[e_ein] = xmlTextReaderConstString(reader, BAD_CAST ("ein"));
    constXcosNames[e_eout] = xmlTextReaderConstString(reader, BAD_CAST ("eout"));
    constXcosNames[e_equations] = xmlTextReaderConstString(reader, BAD_CAST ("equations"));
    constXcosNames[e_expression] = xmlTextReaderConstString(reader, BAD_CAST ("expression"));
    constXcosNames[e_exprs] = xmlTextReaderConstString(reader, BAD_CAST ("exprs"));
    constXcosNames[e_finalTime] = xmlTextReaderConstString(reader, BAD_CAST ("finalTime"));
    constXcosNames[e_firing] = xmlTextReaderConstString(reader, BAD_CAST ("firing"));
    constXcosNames[e_font] = xmlTextReaderConstString(reader, BAD_CAST ("font"));
    constXcosNames[e_fontSize] = xmlTextReaderConstString(reader, BAD_CAST ("fontSize"));
    constXcosNames[e_functionAPI] = xmlTextReaderConstString(reader, BAD_CAST ("functionAPI"));
    constXcosNames[e_functionName] = xmlTextReaderConstString(reader, BAD_CAST ("functionName"));
    constXcosNames[e_geometry] = xmlTextReaderConstString(reader, BAD_CAST ("geometry"));
    constXcosNames[e_height] = xmlTextReaderConstString(reader, BAD_CAST ("height"));
    constXcosNames[e_implicit] = xmlTextReaderConstString(reader, BAD_CAST ("implicit"));
    constXcosNames[e_in] = xmlTextReaderConstString(reader, BAD_CAST ("in"));
    constXcosNames[e_interfaceFunction] = xmlTextReaderConstString(reader, BAD_CAST ("interfaceFunction"));
    constXcosNames[e_ipar] = xmlTextReaderConstString(reader, BAD_CAST ("ipar"));
    constXcosNames[e_kind] = xmlTextReaderConstString(reader, BAD_CAST ("kind"));
    constXcosNames[e_label] = xmlTextReaderConstString(reader, BAD_CAST ("label"));
    constXcosNames[e_lineHeight] = xmlTextReaderConstString(reader, BAD_CAST ("lineHeight"));
    constXcosNames[e_lineWidth] = xmlTextReaderConstString(reader, BAD_CAST ("lineWidth"));
    constXcosNames[e_nmode] = xmlTextReaderConstString(reader, BAD_CAST ("nmode"));
    constXcosNames[e_nzcross] = xmlTextReaderConstString(reader, BAD_CAST ("nzcross"));
    constXcosNames[e_odstate] = xmlTextReaderConstString(reader, BAD_CAST ("odstate"));
    constXcosNames[e_opar] = xmlTextReaderConstString(reader, BAD_CAST ("opar"));
    constXcosNames[e_out] = xmlTextReaderConstString(reader, BAD_CAST ("out"));
    constXcosNames[e_parent] = xmlTextReaderConstString(reader, BAD_CAST ("parent"));
    constXcosNames[e_parentDiagram] = xmlTextReaderConstString(reader, BAD_CAST ("parentDiagram"));
    constXcosNames[e_path] = xmlTextReaderConstString(reader, BAD_CAST ("path"));
    constXcosNames[e_properties] = xmlTextReaderConstString(reader, BAD_CAST ("properties"));
    constXcosNames[e_realtimeScale] = xmlTextReaderConstString(reader, BAD_CAST ("realtimeScale"));
    constXcosNames[e_relativeTolerance] = xmlTextReaderConstString(reader, BAD_CAST ("relativeTolerance"));
    constXcosNames[e_rpar] = xmlTextReaderConstString(reader, BAD_CAST ("rpar"));
    constXcosNames[e_solver] = xmlTextReaderConstString(reader, BAD_CAST ("solver"));
    constXcosNames[e_sourceBlock] = xmlTextReaderConstString(reader, BAD_CAST ("sourceBlock"));
    constXcosNames[e_sourcePort] = xmlTextReaderConstString(reader, BAD_CAST ("sourcePort"));
    constXcosNames[e_state] = xmlTextReaderConstString(reader, BAD_CAST ("state"));
    constXcosNames[e_style] = xmlTextReaderConstString(reader, BAD_CAST ("style"));
    constXcosNames[e_timeTolerance] = xmlTextReaderConstString(reader, BAD_CAST ("timeTolerance"));
    constXcosNames[e_title] = xmlTextReaderConstString(reader, BAD_CAST ("title"));
    constXcosNames[e_type] = xmlTextReaderConstString(reader, BAD_CAST ("type"));
    constXcosNames[e_uid] = xmlTextReaderConstString(reader, BAD_CAST ("uid"));
    constXcosNames[e_version] = xmlTextReaderConstString(reader, BAD_CAST ("version"));
    constXcosNames[e_width] = xmlTextReaderConstString(reader, BAD_CAST ("width"));
    constXcosNames[e_x] = xmlTextReaderConstString(reader, BAD_CAST ("x"));
    constXcosNames[e_xcos] = xmlTextReaderConstString(reader, BAD_CAST ("xcos"));
    constXcosNames[e_y] = xmlTextReaderConstString(reader, BAD_CAST ("y"));

    xcosNamespaceUri = xmlTextReaderConstString(reader, BAD_CAST ("org.scilab.modules.xcos"));
    xsiNamespaceUri = xmlTextReaderConstString(reader, BAD_CAST ("http://www.w3.org/2001/XMLSchema-instance"));

    unresolved.clear();

    /*
     * Process the document
     */
    if (reader != NULL)
    {
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
        {
            ret = processNode(reader);
            if (ret == 1)
            {
                ret = xmlTextReaderRead(reader);
            }
        }
        /*
         * Once the document has been fully parsed check the validation results
         */
        if (xmlTextReaderIsValid(reader) < 0)
        {
            sciprint("Document %s does not validate\n", uri);
        }
        xmlFreeTextReader(reader);
        if (ret < 0)
        {
            sciprint("%s : failed to parse\n", uri);
            return ret;
        }
    }
    else
    {
        sciprint("Unable to open %s\n", uri);
        return -1;
    }

    /*
     * After loading the XML file, resolve all references
     */
    for (const unresolvedReference& ref : unresolved)
    {
        auto it = references.find(ref.m_uid);
        if (it != references.end())
        {
            controller.setObjectProperty(ref.m_id, ref.m_kind, ref.m_prop, it->second);
        }
        else
        {
            sciprint("Unable to resolve %s\n", ref.m_uid.c_str());
            return -1;
        }
    }

    return ret;
}

/*
 * Convert an XML UTF-8 string to a model string
 */
std::string to_string(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return "";
    }

    // the strings in the model are stored as UTF-8 as in libxml2
    return std::string((const char*) xmlStr);
}

/*
 * Convert an XML UTF-8 string to a model int
 */
int to_int(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return 0;
    }

    return std::atoi((const char*) xmlStr);
}

/*
 * Convert an XML UTF-8 string to a model double
 */
double to_double(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return 0.0;
    }

    return std::atof((const char*) xmlStr);
}

int XMIResource::loadDoubleArray(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o)
{
    std::vector<double> v;
    controller.getObjectProperty(o.id(), o.kind(), property, v);

    v.push_back(to_double(xmlTextReaderConstValue(reader)));

    controller.setObjectProperty(o.id(), o.kind(), property, v);
    return 1;
}

int XMIResource::loadIntArray(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o)
{
    std::vector<int> v;
    controller.getObjectProperty(o.id(), o.kind(), property, v);

    v.push_back(to_int(xmlTextReaderConstValue(reader)));

    controller.setObjectProperty(o.id(), o.kind(), property, v);
    return 1;
}

int XMIResource::loadStringArray(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o)
{
    std::vector<std::string> v;
    controller.getObjectProperty(o.id(), o.kind(), property, v);

    v.push_back(to_string(xmlTextReaderConstValue(reader)));

    controller.setObjectProperty(o.id(), o.kind(), property, v);
    return 1;
}

int XMIResource::loadBase64(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o)
{
    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));

        switch (current)
        {
            case e_base64:
            {
                const xmlChar* base64 = xmlTextReaderConstValue(reader);
                std::vector<double> v = base64::decode<std::vector<double> >(to_string(base64));
                controller.setObjectProperty(o.id(), o.kind(), property, v);
                break;
            }
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    return 1;
}

int XMIResource::loadPoint(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == LINK);

    std::vector<double> points;
    controller.getObjectProperty(o.id(), o.kind(), CONTROL_POINTS, points);

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_x:
                points.push_back(to_double(xmlTextReaderConstValue(reader)));
                break;
            case e_y:
                points.push_back(to_double(xmlTextReaderConstValue(reader)));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    controller.setObjectProperty(o.id(), o.kind(), CONTROL_POINTS, points);
    return 1;
}

int XMIResource::loadGeometry(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == BLOCK || o.kind() == ANNOTATION || o.kind() == LINK);

    std::vector<double> geom;
    controller.getObjectProperty(o.id(), o.kind(), GEOMETRY, geom);
    geom.resize(4);

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_x:
                geom[0] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_y:
                geom[1] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_width:
                geom[2] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_height:
                geom[3] = to_double(xmlTextReaderConstValue(reader));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    controller.setObjectProperty(o.id(), o.kind(), GEOMETRY, geom);
    return 1;
}

int XMIResource::loadAbstractBaseObject(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == BLOCK || o.kind() == ANNOTATION || o.kind() == LINK);

    // abstract Layer is not decoded there as it has no attribute

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_uid:
            {
                std::string uid = to_string(xmlTextReaderConstValue(reader));
                controller.setObjectProperty(o.id(), o.kind(), UID, uid);
                references.insert(std::make_pair(uid, o.id()));
                break;
            }
            case e_parentDiagram:
            {
                // not lookup needed ; only one diagram is serialized at a time
                controller.setObjectProperty(o.id(), o.kind(), PARENT_DIAGRAM, root);
                break;
            }
            case e_parent:
            {
                // not lookup needed thanks to the XML hierarchy
                const model::BaseObject& parent = *(processed.end() - 2);
                controller.setObjectProperty(o.id(), o.kind(), PARENT_BLOCK, parent.id());
                break;
            }
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    return 1;
}

int XMIResource::loadDiagram(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == DIAGRAM);

    // abstract Layer is not decoded there as it has no attribute

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_title:
                controller.setObjectProperty(o.id(), o.kind(), TITLE, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_path:
                controller.setObjectProperty(o.id(), o.kind(), PATH, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_debugLevel:
                controller.setObjectProperty(o.id(), o.kind(), DEBUG_LEVEL, to_int(xmlTextReaderConstValue(reader)));
                break;
            case e_version:
                controller.setObjectProperty(o.id(), o.kind(), VERSION_NUMBER, to_string(xmlTextReaderConstValue(reader)));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    return 1;
}

int XMIResource::loadSimulationConfig(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == DIAGRAM);

    std::vector<double> properties;
    controller.getObjectProperty(o.id(), o.kind(), PROPERTIES, properties);
    properties.resize(8);

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_finalTime:
                properties[0] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_absoluteTolerance:
                properties[1] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_relativeTolerance:
                properties[2] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_timeTolerance:
                properties[3] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_deltaT:
                properties[4] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_realtimeScale:
                properties[5] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_solver:
                properties[6] = to_double(xmlTextReaderConstValue(reader));
                break;
            case e_deltaH:
                properties[7] = to_double(xmlTextReaderConstValue(reader));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    controller.setObjectProperty(o.id(), o.kind(), PROPERTIES, properties);
    return 1;
}

int XMIResource::loadBlock(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == BLOCK);

    // load the base class
    int ret = loadAbstractBaseObject(reader, o);
    if (ret != 1)
    {
        return ret;
    }

    // Layer has no attribute so there is no need to decode it there
    // Geometry is handled as an element

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_description:
                controller.setObjectProperty(o.id(), o.kind(), DESCRIPTION, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_label:
                controller.setObjectProperty(o.id(), o.kind(), LABEL, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_style:
                controller.setObjectProperty(o.id(), o.kind(), STYLE, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_interfaceFunction:
                controller.setObjectProperty(o.id(), o.kind(), INTERFACE_FUNCTION, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_functionName:
                controller.setObjectProperty(o.id(), o.kind(), SIM_FUNCTION_NAME, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_functionAPI:
                controller.setObjectProperty(o.id(), o.kind(), SIM_FUNCTION_API, to_int(xmlTextReaderConstValue(reader)));
                break;
            case e_dependsOnT:
            {
                std::vector<int> dep_ut;
                controller.getObjectProperty(o.id(), o.kind(), SIM_DEP_UT, dep_ut);
                dep_ut.resize(2);

                dep_ut[1] = to_int(xmlTextReaderConstValue(reader));
                controller.setObjectProperty(o.id(), o.kind(), SIM_DEP_UT, dep_ut);
                break;
            }
            case e_dependsOnU:
            {
                std::vector<int> dep_ut;
                controller.getObjectProperty(o.id(), o.kind(), SIM_DEP_UT, dep_ut);
                dep_ut.resize(2);

                dep_ut[0] = to_int(xmlTextReaderConstValue(reader));
                controller.setObjectProperty(o.id(), o.kind(), SIM_DEP_UT, dep_ut);
                break;
            }
            case e_blocktype:
                controller.setObjectProperty(o.id(), o.kind(), SIM_BLOCKTYPE, to_string(xmlTextReaderConstValue(reader)));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    /*
     * Reset some properties loaded as array and initialized with non-empty value
     */
    std::vector<int> empty_int_array;
    controller.setObjectProperty(o.id(), o.kind(), NZCROSS, empty_int_array);
    controller.setObjectProperty(o.id(), o.kind(), NMODE, empty_int_array);

    return 1;
}

int XMIResource::loadPort(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == PORT);

    // ignore datatype as it is managed as an XML node

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_uid:
            {
                std::string uid = to_string(xmlTextReaderConstValue(reader));
                controller.setObjectProperty(o.id(), o.kind(), UID, uid);
                references.insert(std::make_pair(uid, o.id()));
                break;
            }
            case e_firing:
                controller.setObjectProperty(o.id(), o.kind(), FIRING, to_double(xmlTextReaderConstValue(reader)));
                break;
            case e_sourceBlock:
            {
                // not lookup needed thanks to the XML hierarchy
                const model::BaseObject& parent = processed.back();
                controller.setObjectProperty(o.id(), o.kind(), SOURCE_BLOCK, parent.id());
                break;
            }
            case e_kind:
            {
                std::string portKindStr = to_string(xmlTextReaderConstValue(reader));
                int k;
                if ("in" == portKindStr)
                {
                    k = PORT_IN;
                }
                else if ("out" == portKindStr)
                {
                    k = PORT_OUT;
                }
                else if ("ein" == portKindStr)
                {
                    k = PORT_EIN;
                }
                else if ("eout" == portKindStr)
                {
                    k = PORT_EOUT;
                }
                else
                {
                    k = PORT_UNDEF;
                }
                controller.setObjectProperty(o.id(), o.kind(), PORT_KIND, k);
                break;
            }
            case e_implicit:
                controller.setObjectProperty(o.id(), o.kind(), IMPLICIT, to_int(xmlTextReaderConstValue(reader)));
                break;
            case e_connectedSignal:
                // will be resolved later
                unresolved.push_back(
                    unresolvedReference(o.id(), o.kind(), CONNECTED_SIGNALS,
                                        to_string(xmlTextReaderConstValue(reader))));
                break;
            case e_style:
                controller.setObjectProperty(o.id(), o.kind(), STYLE, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_label:
                controller.setObjectProperty(o.id(), o.kind(), LABEL, to_string(xmlTextReaderConstValue(reader)));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    return 1;
}

int XMIResource::loadLink(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == LINK);

    // load the base class
    int ret = loadAbstractBaseObject(reader, o);
    if (ret != 1)
    {
        return ret;
    }

    // geometry is handled as in independent node
    // controlPoint is handled as in independent node

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_uid:
            {
                std::string uid = to_string(xmlTextReaderConstValue(reader));
                controller.setObjectProperty(o.id(), o.kind(), UID, uid);
                references.insert(std::make_pair(uid, o.id()));
                break;
            }
            case e_sourcePort:
                // will be resolved later
                unresolved.push_back(
                    unresolvedReference(o.id(), o.kind(), SOURCE_PORT, to_string(xmlTextReaderConstValue(reader))));
                break;
            case e_destinationPort:
                // will be resolved later
                unresolved.push_back(
                    unresolvedReference(o.id(), o.kind(), DESTINATION_PORT,
                                        to_string(xmlTextReaderConstValue(reader))));
                break;
            case e_style:
                controller.setObjectProperty(o.id(), o.kind(), STYLE, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_label:
                controller.setObjectProperty(o.id(), o.kind(), LABEL, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_lineWidth:
            {
                std::vector<double> thick;
                controller.getObjectProperty(o.id(), o.kind(), THICK, thick);
                thick[0] = to_double(xmlTextReaderConstValue(reader));
                controller.setObjectProperty(o.id(), o.kind(), THICK, thick);
                break;
            }
            case e_lineHeight:
            {
                std::vector<double> thick;
                controller.getObjectProperty(o.id(), o.kind(), THICK, thick);
                thick[1] = to_double(xmlTextReaderConstValue(reader));
                controller.setObjectProperty(o.id(), o.kind(), THICK, thick);
                break;
            }
            case e_color:
                controller.setObjectProperty(o.id(), o.kind(), COLOR, to_int(xmlTextReaderConstValue(reader)));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    return ret;
}

int XMIResource::loadAnnotation(xmlTextReaderPtr reader, const model::BaseObject& o)
{
    assert(o.kind() == ANNOTATION);

    // load the base class
    int ret = loadAbstractBaseObject(reader, o);
    if (ret != 1)
    {
        return ret;
    }

    // geometry is handled as a node

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstName(reader));
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_description:
                controller.setObjectProperty(o.id(), o.kind(), DESCRIPTION, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_font:
                controller.setObjectProperty(o.id(), o.kind(), FONT, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_fontSize:
                controller.setObjectProperty(o.id(), o.kind(), FONT_SIZE, to_string(xmlTextReaderConstValue(reader)));
                break;
            case e_style:
                controller.setObjectProperty(o.id(), o.kind(), STYLE, to_string(xmlTextReaderConstValue(reader)));
                break;
            default:
                // ignore other parameters
                // TODO: Does other metamodels might be managed there ?
                break;
        }
    }

    return ret;
}

int XMIResource::processNode(xmlTextReaderPtr reader)
{
    // manage only xcos related XML nodes
    const xmlChar* nsURI = xmlTextReaderConstNamespaceUri(reader);
    if (nsURI == xcosNamespaceUri || nsURI == nullptr)
    {
        xmlReaderTypes nodeType = (xmlReaderTypes) xmlTextReaderNodeType(reader);
        switch (nodeType)
        {
            case XML_READER_TYPE_NONE:
                return 1;
            case XML_READER_TYPE_ELEMENT:
                return processElement(reader);
            case XML_READER_TYPE_ATTRIBUTE:
                sciprint("xmlReader attributes node not supported\n");
                return -1;
            case XML_READER_TYPE_TEXT:
                return processText(reader);
            case XML_READER_TYPE_CDATA:
                return processText(reader);
            case XML_READER_TYPE_ENTITY_REFERENCE:
                sciprint("xmlReader entity reference not supported\n");
                return -1;
            case XML_READER_TYPE_ENTITY:
                sciprint("xmlReader entity not supported\n");
                return -1;
            case XML_READER_TYPE_PROCESSING_INSTRUCTION:
                sciprint("xmlReader processing instruction not supported\n");
                return -1;
            case XML_READER_TYPE_COMMENT:
                return 1;
            case XML_READER_TYPE_DOCUMENT:
                return 1;
            case XML_READER_TYPE_DOCUMENT_TYPE:
                sciprint("xmlReader document type not supported\n");
                return -1;
            case XML_READER_TYPE_DOCUMENT_FRAGMENT:
                sciprint("xmlReader document fragment not supported\n");
                return -1;
            case XML_READER_TYPE_NOTATION:
                sciprint("xmlReader notation not supported\n");
                return -1;
            case XML_READER_TYPE_WHITESPACE:
                sciprint("xmlReader whitespace not supported\n");
                return -1;
            case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                return 1; // ignore indent or end-of-line
            case XML_READER_TYPE_END_ELEMENT:
                return processEndElement(reader);
            case XML_READER_TYPE_END_ENTITY:
                sciprint("xmlReader end entity not supported\n");
                return -1;
            case XML_READER_TYPE_XML_DECLARATION:
                sciprint("xmlReader XML declaration not supported\n");
                return -1;
        }
    }
    else
    {
        // TODO mixed model should be preserved in some way and restored back on XMIResource_save.cpp .
    }
    sciprint("unable to process node\n");
    return -1;
}

int XMIResource::processElement(xmlTextReaderPtr reader)
{
    const xmlChar *name = xmlTextReaderConstLocalName(reader);
    parent = NB_XCOS_NAMES;

    // lookup for known node names
    // thanks to the string intern-ing, the pointer comparison could be used
    auto found = std::find(constXcosNames.begin(), constXcosNames.end(), name);
    enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
    switch (current)
    {
        case e_Diagram:
        {
            // the root diagram should be decoded
            model::BaseObject o(root, DIAGRAM);

            processed.push_back(o);
            return loadDiagram(reader, o);
        }
        case e_child:
        {
            // this is a child of a diagram, resolve the type and call the loaders
            // iterate on attributes to lookup for EMF type

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* nsURI  = xmlTextReaderConstNamespaceUri(reader);
                if (nsURI != xsiNamespaceUri)
                {
                    continue;
                }

                auto foundName = std::find(constXcosNames.begin(), constXcosNames.end(), xmlTextReaderConstLocalName(reader));
                enum xcosNames currentName = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), foundName));
                if (currentName != e_type)
                {
                    continue;
                }

                const xmlChar* value = xmlTextReaderConstValue(reader);
                const xmlChar* valueWithoutPrefix = BAD_CAST(std::strchr((const char*) value, ':'));
                if (valueWithoutPrefix == nullptr)
                {
                    valueWithoutPrefix = value;
                }
                else
                {
                    // remove the leading ':'
                    valueWithoutPrefix = valueWithoutPrefix + 1;
                }
                const xmlChar* interned = xmlTextReaderConstString(reader, valueWithoutPrefix);

                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), interned);
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_Block:
                    {
                        ScicosID o = controller.createObject(BLOCK);

                        // assign the child
                        model::BaseObject parent = processed.back();
                        std::vector<ScicosID> children;
                        controller.getObjectProperty(parent.id(), parent.kind(), CHILDREN, children);
                        children.push_back(o);
                        controller.setObjectProperty(parent.id(), parent.kind(), CHILDREN, children);

                        model::BaseObject child(o, BLOCK);
                        processed.push_back(child);
                        return loadBlock(reader, child);
                    }
                    case e_Link:
                    {
                        ScicosID o = controller.createObject(LINK);

                        // assign the child
                        model::BaseObject parent = processed.back();
                        std::vector<ScicosID> children;
                        controller.getObjectProperty(parent.id(), parent.kind(), CHILDREN, children);
                        children.push_back(o);
                        controller.setObjectProperty(parent.id(), parent.kind(), CHILDREN, children);

                        model::BaseObject child(o, LINK);
                        processed.push_back(child);
                        return loadLink(reader, child);
                    }
                    case e_Annotation:
                    {
                        ScicosID o = controller.createObject(ANNOTATION);

                        // assign the child
                        model::BaseObject parent = processed.back();
                        std::vector<ScicosID> children;
                        controller.getObjectProperty(parent.id(), parent.kind(), CHILDREN, children);
                        children.push_back(o);
                        controller.setObjectProperty(parent.id(), parent.kind(), CHILDREN, children);

                        model::BaseObject child(o, ANNOTATION);
                        return loadAnnotation(reader, child);
                    }
                    default:
                        sciprint("Not handled child type=%s at line %d\n", *found,
                                 xmlTextReaderGetParserLineNumber(reader) - 1);
                        return -1;
                }
            }
            break;
        }
        case e_in: // no break on purpose
        case e_out: // no break on purpose
        case e_ein: // no break on purpose
        case e_eout:
        {
            ScicosID o = controller.createObject(PORT);

            enum object_properties_t p;
            switch (current)
            {
                case e_in:
                    p = INPUTS;
                    break;
                case e_out:
                    p = OUTPUTS;
                    break;
                case e_ein:
                    p = EVENT_INPUTS;
                    break;
                case e_eout:
                    p = EVENT_OUTPUTS;
                    break;
                default:
                    return -1;
            }

            model::BaseObject parent = processed.back();
            // add the port them to the parent
            std::vector<ScicosID> ports;
            controller.getObjectProperty(parent.id(), parent.kind(), p, ports);
            ports.push_back(o);
            controller.setObjectProperty(parent.id(), parent.kind(), p, ports);

            // decode content
            model::BaseObject child(o, PORT);
            return loadPort(reader, child);
        }
        case e_geometry:
            // geometry is used for rectangle coordinates of its parent
            return loadGeometry(reader, processed.back());
        case e_nzcross:
            // nzcross is a Block property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_nmode:
            // nmode is a Block property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_rpar:
            // rpar is a Block property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_ipar:
            // ipar is a Block property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_opar:
            // ipar is a Block property
            return loadBase64(reader, OPAR, processed.back());
        case e_state:
            // state is a Block property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_dstate:
            // dstate is a Block property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_odstate:
            // odstate is a Block property
            return loadBase64(reader, ODSTATE, processed.back());
        case e_equations:
            // equation is a Block property
            return loadBase64(reader, EQUATIONS, processed.back());
        case e_expression:
            // expression is a Block property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_exprs:
            // exprs is a Block property
            return loadBase64(reader, EXPRS, processed.back());
        case e_controlPoint:
            // controlPoint is a link property
            return loadPoint(reader, processed.back());
        case e_context:
            // context is a Layer property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        case e_properties:
            // properties is a Diagram property
            return loadSimulationConfig(reader, processed.back());
        case e_datatype:
            // datatype is a Port property
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                parent = current;
            }
            return 1;
        default:
            sciprint("Unknown \"%s\" element name at line %d\n", name, xmlTextReaderGetParserLineNumber(reader) - 1);
            return -1;
    }

    return 1;
}

int XMIResource::processText(xmlTextReaderPtr reader)
{
    int ret;

    switch (parent)
    {
        case e_nzcross:
            // nzcross is a Block property
            ret = loadIntArray(reader, NZCROSS, processed.back());
            break;
        case e_nmode:
            // nmode is a Block property
            ret = loadIntArray(reader, NMODE, processed.back());
            break;
        case e_rpar:
            // rpar is a Block property
            ret = loadDoubleArray(reader, RPAR, processed.back());
            break;
        case e_ipar:
            // ipar is a Block property
            ret = loadIntArray(reader, IPAR, processed.back());
            break;
        case e_state:
            // state is a Block property
            ret = loadDoubleArray(reader, RPAR, processed.back());
            break;
        case e_dstate:
            // dstate is a Block property
            ret = loadDoubleArray(reader, RPAR, processed.back());
            break;
        case e_expression:
            // expression is a Block property
            ret = loadStringArray(reader, EXPRS, processed.back());
            break;
        case e_context:
            // context is a Layer property
            ret = loadStringArray(reader, DIAGRAM_CONTEXT, processed.back());
            break;
        case e_datatype:
            // datatype is a port property
            ret = loadIntArray(reader, DATATYPE, processed.back());
            break;
        default:
            sciprint("Unable to decode text value at line %d\n", xmlTextReaderGetParserLineNumber(reader) - 1);
            ret = -1;
            break;
    }

    return ret;
}

int XMIResource::processEndElement(xmlTextReaderPtr)
{
    if (parent == NB_XCOS_NAMES)
    {
        processed.pop_back();
    }
    else
    {
        parent = NB_XCOS_NAMES;
    }

    return 1;
}

} /* namespace org_scilab_modules_xcos */
