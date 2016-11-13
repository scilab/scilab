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

#include "utilities.hxx"
#include "Controller.hxx"
#include "model/BaseObject.hxx"

#include "dynlib_scicos.h"

#include <map>
#include <array>
#include <string>
#include <vector>

extern "C" {
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
}

namespace org_scilab_modules_scicos
{

class SCICOS_IMPEXP XMIResource
{

public:
    XMIResource(ScicosID id);
    ~XMIResource();

    int save(const char* uri);
    int load(const char* uri);

private:
    /*
     * Save helpers
     */

    int writePoint(xmlTextWriterPtr writer, double x, double y);
    int writeGeometry(xmlTextWriterPtr writer, ScicosID id, kind_t kind);
    int writeAbstractLayer(xmlTextWriterPtr writer, ScicosID id, kind_t kind);
    int writeAbstractBaseObject(xmlTextWriterPtr writer, ScicosID id, kind_t kind);
    int writeDiagram(xmlTextWriterPtr writer);
    int writeSimulationConfig(xmlTextWriterPtr writer, ScicosID id);
    int writeBlock(xmlTextWriterPtr writer, ScicosID id);
    int writePort(xmlTextWriterPtr writer, enum object_properties_t container, ScicosID id);
    int writeLink(xmlTextWriterPtr writer, ScicosID id);
    int writeAnnotation(xmlTextWriterPtr writer, ScicosID id);

    /*
     * Load helpers
     */
    int loadDoubleArray(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o);
    int loadIntArray(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o);
    int loadStringArray(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o);
    int loadBase64(xmlTextReaderPtr reader, enum object_properties_t property, const model::BaseObject& o);
    int loadPoint(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadGeometry(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadAbstractBaseObject(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadDiagram(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadSimulationConfig(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadBlock(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadPort(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadLink(xmlTextReaderPtr reader, const model::BaseObject& o);
    int loadAnnotation(xmlTextReaderPtr reader, const model::BaseObject& o);

    int processNode(xmlTextReaderPtr reader);
    int processElement(xmlTextReaderPtr reader);
    int processText(xmlTextReaderPtr reader);
    int processEndElement(xmlTextReaderPtr reader);

private:
    /* shared controller */
    Controller controller;
    /* id of the diagram */
    ScicosID root;

    /*
     * load shared variables
     */

    /* interned string indexes */
    enum xcosNames
    {
        e_Annotation,
        e_BaseObject,
        e_Block,
        e_CompiledRepresentation,
        e_Diagram,
        e_Geometry,
        e_Layer,
        e_Link,
        e_Point,
        e_Port,
        e_PortKind,
        e_SimulationConfig,
        e_absoluteTolerance,
        e_base64,
        e_blocktype,
        e_child,
        e_color,
        e_connectedSignal,
        e_context,
        e_controlPoint,
        e_datatype,
        e_debugLevel,
        e_deltaH,
        e_deltaT,
        e_dependsOnT,
        e_dependsOnU,
        e_description,
        e_destinationPort,
        e_dstate,
        e_ein,
        e_eout,
        e_equations,
        e_expression,
        e_exprs,
        e_finalTime,
        e_firing,
        e_font,
        e_fontSize,
        e_functionAPI,
        e_functionName,
        e_geometry,
        e_height,
        e_implicit,
        e_in,
        e_interfaceFunction,
        e_ipar,
        e_kind,
        e_label,
        e_lineHeight,
        e_lineWidth,
        e_nmode,
        e_nzcross,
        e_odstate,
        e_opar,
        e_out,
        e_parent,
        e_parentDiagram,
        e_path,
        e_properties,
        e_realtimeScale,
        e_relativeTolerance,
        e_rpar,
        e_solver,
        e_sourceBlock,
        e_sourcePort,
        e_state,
        e_style,
        e_timeTolerance,
        e_title,
        e_type,
        e_uid,
        e_version,
        e_width,
        e_x,
        e_xcos,
        e_y,
        NB_XCOS_NAMES
    };
    /* temporary interned string content (owned by a reader) */
    std::array<const xmlChar*, NB_XCOS_NAMES> constXcosNames;
    /* temporary interned xcos namespace (owned by a reader) */
    const xmlChar* xcosNamespaceUri;
    /* temporary interned xsi namespace (owned by a reader) */
    const xmlChar* xsiNamespaceUri;

    /* stack uid / kind used */
    std::vector<model::BaseObject> processed;

    /* store the parent of a text node */
    enum xcosNames parent;

    /* uid string - ScicosID  map */
    std::map<std::string, ScicosID> references;
    /* reference to resolve after loading the whole file */
    struct unresolvedReference
    {
        unresolvedReference(ScicosID id, kind_t kind, enum object_properties_t prop, std::string uid)
            : m_id(id), m_kind(kind), m_prop(prop), m_uid(uid) {}
        ScicosID m_id;
        kind_t m_kind;
        enum object_properties_t m_prop;
        std::string m_uid;
    };
    /* temporary unresolved references */
    std::vector<unresolvedReference> unresolved;
};

}

