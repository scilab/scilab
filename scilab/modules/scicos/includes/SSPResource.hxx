/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2023 - Dassault Systèmes S.E. - Clément DAVID
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
#include "scicos_base64.hxx"

#include "dynlib_scicos.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <map>
#include <unordered_set>
#include <stack>
#include <string>
#include <system_error>
#include <vector>

extern "C" {
#include <archive.h>
#include <archive_entry.h>

#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
}

#define BLOCK_SIZE 10240

#ifdef max
#undef max
#endif // max


namespace org_scilab_modules_scicos
{

namespace 
{
    // helper template: write as value="string value" is only for string vector with one element
    template<typename T>
    constexpr bool can_write_to_string(const T& shared) {
        return std::is_arithmetic<T>::value;
    };
    template<>
    bool can_write_to_string(const std::string& shared) {
        // can only write string if it does not contains xml special characters
        // there is no need to embed XML as string, store it encoded in base64
        if (shared.find_first_of("<>&'\"") != std::string::npos) {
            return false;
        }
        return true;
    };
    template<typename T>
    bool can_write_to_string(const std::vector<T>& shared) {
        return  (shared.size() == 0) ||
                (shared.size() > 0 && can_write_to_string(shared[0]));
    };
    template<>
    bool can_write_to_string(const std::vector<double>& shared) {
        // can only write double as int
        return std::all_of(shared.begin(), shared.end(), [](double d) { return ((int) d) == d; });
    };
    template<>
    bool can_write_to_string(const std::vector<std::string>& shared) {
        // hide the string separator in the base64 encoding
        return shared.size() == 1 && can_write_to_string(shared[0]);
    };
} /* anonymous namespace */

class SCICOS_IMPEXP SSPResource
{

public:
struct Result
{
private:
    // if code is -1, it means that the Result is not recoverable
    int code;
    // property on Result
    object_properties_t property;
    // object that caused the Result
    model::BaseObject* object;

public:
    inline Result(int c, object_properties_t p, model::BaseObject* o) : code(c), property(p), object(o) {};

    static inline Result Ok()
    {
        return {0, MAX_OBJECT_PROPERTIES, nullptr};
    };
    static inline Result Error()
    {
        return {-1, MAX_OBJECT_PROPERTIES, nullptr};
    };
    // Error when getting a model object
    static inline Result Error(model::BaseObject* object)
    {
        Result r = Error();
        r.object = object;
        return r;
    };
    // Error when getting a model property
    static inline Result Error(model::BaseObject* object, object_properties_t property)
    {
        Result r = Error();
        r.object = object;
        r.property = property;
        return r;
    };

    // check a result from libxml2
    static inline Result FromXML(int code)
    {
        if(code >= 0)
            return Ok();
        Result r = Error();
        r.code = code;
        return r;
    }
    // check a result from libarchive
    static inline Result FromArchive(int code)
    {
        if(code == ARCHIVE_OK)
            return Ok();
        if(code == ARCHIVE_EOF)
            return Ok();
        // Archive error codes are negative integers, store them
        Result r = Error();
        r.code = code;
        return r;
    }

    // is this result ok or recoverable ? 
    inline bool ok()
    {
        return code >= 0;
    }
    // is there any unrecoverable Result ?
    inline bool error()
    {
        return code < 0;
    }

    std::string report();
};

// Canvas of visible elements, used to handle Y-axis inversion
struct SystemCanvas {    
    // x coordinate of the lower-left corner of the system canvas.
    double x1;
    // y coordinate of the lower-left corner of the system canvas.
    double y1;
    // x coordinate of the upper-right corner of the system canvas.
    double x2;
    // y coordinate of the upper-right corner of the system canvas.
    double y2;

    static inline
    SystemCanvas default_xcos_ccordinates()
    {
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()};
    }

    static inline
    SystemCanvas default_ssp_ccordinates()
    {
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()};
    }

    // grow the canvas by the geometry of the object
    Result grow_by_xcos_ccordinates(Controller &controller, model::BaseObject *o, std::vector<double>& _vecDblShared);
};

// Classify children depending on some internal properties.
// This is used to categories once and process children on sub-functions
class ChildrenCategories
{
public:
    // temporary, shared storage with Controller
    std::string& _strShared;
    // temporary, shared storage with Controller
    std::vector<double>& _vecDblShared;
    // temporary, shared storage with Controller
    std::vector<int>& _vecIntShared;
    // temporary, shared storage with Controller
    std::vector<std::string>& _vecStrShared;
    // temporary, shared storage with Controller
    std::vector<ScicosID>& _vecIDShared;

    ChildrenCategories(std::string& _strShared, std::vector<double>& _vecDblShared, std::vector<int>& _vecIntShared, std::vector<std::string>& _vecStrShared, std::vector<ScicosID>& _vecIDShared) : 
        _strShared(_strShared),
        _vecDblShared(_vecDblShared),
        _vecIntShared(_vecIntShared),
        _vecStrShared(_vecStrShared),
        _vecIDShared(_vecIDShared)
    {
    }

    struct all_port_t
    {
        // kinf of the port
        enum portKind kind;
        // index of the port (in the INPUTS, OUTPUTS, EVENT_INPUTS, EVENT_OUTPUTS)
        int port_index;
        // true if the name was generated
        bool generated;
        // port inside the block
        model::BaseObject* inner_port;
        // port inside the block
        model::BaseObject* outter_port;
        // inner I/O block or SSPInput, SSPOutput outter block
        model::BaseObject* block;
        // index of the I/O block
        size_t block_index;
        // name, set or generated
        std::string name;
    };

    // All outter ports
    std::vector<all_port_t> all_ports;
    // Count enum portKind in a layer
    std::array<int, 5> max_indexes{1, 1, 1, 1, 1};
    
    // all blocks
    std::vector<model::BaseObject*> elements;
    // all links
    std::vector<model::BaseObject*> connections;
    // all graphical elements
    std::vector<model::BaseObject*> graphical_elements;

    // names for generated blocks
    std::map<model::BaseObject*, std::string> names;
    std::unordered_set<std::string> used_names;

    // bounds of the current system
    SystemCanvas canvas;

private:
    // unamed element counter, increased for each unnamed element
    unsigned long long unnamed_counter{0};

public:
    // decode Block IPAR or EXPRS to its index
    Result decode_ipar_or_exprs(Controller& controller, model::BaseObject* o, int& index);
    // Get the associated port
    Result store_port(Controller& controller, model::BaseObject* o, object_properties_t p, size_t index, std::string& name, const std::string& prefix);
    // assign all outter_ports to their corresponding port_index
    Result assign_outter_ports(Controller& controller, model::BaseObject* parent);
    // fill the categories with all children
    Result load_children(Controller& controller, model::BaseObject* parent, const std::vector<ScicosID>& children);
    // fill the categories with outer ports
    Result load_ports(Controller& controller, model::BaseObject* block);
    // retrieve the name of a block or port (may be generated)
    std::string retrieve_name(Controller& controller, model::BaseObject* o);
    // get the name of a block or port or ""
    std::string get_name(Controller& controller, model::BaseObject* o);
    // retrieve the name of a block or port (set or generated)
    void insert_all_names(const ChildrenCategories& blockPorts);
};

/* interned string indexes */
enum xcosNames
{
    e_A,
    e_Annotation,
    e_Annotations,
    e_BaseUnit,
    e_Binary,
    e_Boolean,
    e_BooleanMappingTransformation,
    e_Clock,
    e_CoSimulation,
    e_Complex,
    e_Component,
    e_Connection,
    e_ConnectionGeometry,
    e_Connections,
    e_Connector,
    e_ConnectorGeometry,
    e_Connectors,
    e_DefaultExperiment,
    e_DictionaryEntry,
    e_Dimension,
    e_ElementGeometry,
    e_Elements,
    e_Enumeration,
    e_EnumerationMappingTransformation,
    e_Enumerations,
    e_Float32,
    e_Float64,
    e_GraphicalElements,
    e_Int16,
    e_Int32,
    e_Int64,
    e_Int8,
    e_Integer,
    e_IntegerMappingTransformation,
    e_Item,
    e_K,
    e_LinearTransformation,
    e_MapEntry,
    e_MappingEntry,
    e_ModelExchange,
    e_Note,
    e_Parameter,
    e_ParameterBinding,
    e_ParameterBindings,
    e_ParameterMapping,
    e_ParameterSet,
    e_ParameterValues,
    e_Parameters,
    e_Real,
    e_SSD,
    e_ScheduledExecution,
    e_SignalDictionaries,
    e_SignalDictionary,
    e_SignalDictionaryReference,
    e_String,
    e_System,
    e_SystemGeometry,
    e_SystemStructureDescription,
    e_UInt16,
    e_UInt32,
    e_UInt64,
    e_UInt8,
    e_Unit,
    e_Units,
    e_acausal,
    e_any,
    e_application_x_fmu_sharedlibrary,
    e_application_x_scilab_xcos,
    e_application_x_ssp_definition,
    e_application_x_ssp_package,
    e_author,
    e_base64,
    e_calculatedParameter,
    e_cd,
    e_color,
    e_context,
    e_control_points,
    e_copyright,
    e_datatype,
    e_debug_level,
    e_description,
    e_dictionary,
    e_dstate,
    e_endConnector,
    e_endElement,
    e_equations,
    e_exprs,
    e_factor,
    e_fileversion,
    e_firing,
    e_font,
    e_font_size,
    e_generationDateAndTime,
    e_generationTool,
    e_geometry,
    e_height,
    e_iconFixedAspectRatio,
    e_iconFlip,
    e_iconRotation,
    e_iconSource,
    e_id,
    e_implementation,
    e_implicit,
    e_inout,
    e_input,
    e_interface_function,
    e_ipar,
    e_kg,
    e_kind,
    e_label,
    e_license,
    e_m,
    e_mime_type,
    e_mol,
    e_name,
    e_nmode,
    e_nzcross,
    e_odstate,
    e_offset,
    e_opar,
    e_org_scilab_xcos_ssp,
    e_output,
    e_parameter,
    e_path,
    e_pointsX,
    e_pointsY,
    e_prefix,
    e_properties,
    e_rad,
    e_rotation,
    e_rpar,
    e_s,
    e_sim_blocktype,
    e_sim_dep_ut,
    e_sim_function_api,
    e_sim_function_name,
    e_size,
    e_source,
    e_sourceBase,
    e_ssb,
    e_ssc,
    e_ssd,
    e_ssm,
    e_ssv,
    e_startConnector,
    e_startElement,
    e_startTime,
    e_state,
    e_stopTime,
    e_style,
    e_suppressUnitConversion,
    e_systemInnerX,
    e_systemInnerY,
    e_target,
    e_text,
    e_text_x_modelica,
    e_thick,
    e_type,
    e_uid,
    e_unit,
    e_value,
    e_version,
    e_width,
    e_x,
    e_x1,
    e_x2,
    e_xcos,
    e_xmlns,
    e_y,
    e_y1,
    e_y2,
    NB_XCOS_NAMES
};

public:
    SSPResource(ScicosID id);
    ~SSPResource();

    Result save(const char* uri);
    int load(const char* uri);
    Result export_to_dot(const char* uri);

private:
    /*
     * Save helpers
     */
    Result writeSystemStructureFile(void* context);
    Result writeSystemStructureDescription(xmlTextWriterPtr writer);
    Result writeSystem(xmlTextWriterPtr writer, model::BaseObject* o, ChildrenCategories& categories);
    Result writeConnectors(xmlTextWriterPtr writer, ChildrenCategories& inner);
    Result writeConnector(xmlTextWriterPtr writer, const ChildrenCategories::all_port_t& o, std::string name, const std::array<int, 5>& max_indexes);
    Result writeType(xmlTextWriterPtr writer, enum portKind kind, model::BaseObject* o);
    Result writeConnectorGeometry(xmlTextWriterPtr writer, const ChildrenCategories::all_port_t& o, const std::array<int, 5>& max_indexes);
    Result writeParameterBindings(xmlTextWriterPtr writer);
    Result writeElements(xmlTextWriterPtr writer, ChildrenCategories& categories);
    Result writeComponent(xmlTextWriterPtr writer, model::BaseObject* c, ChildrenCategories& categories);
    Result writeComponentObjectProperties(xmlTextWriterPtr writer, model::BaseObject* component, const ChildrenCategories& categories);
    template<typename T>
    Result writeAnnotationObjectProperty(xmlTextWriterPtr writer, model::BaseObject* o, enum object_properties_t prop, enum xcosNames element, T& shared)
    {
        Result status = Result::Ok();
    
        if (!controller.getObjectProperty(o, prop, shared))
        {
            return Result::Error(o, prop);
        }
        /* TODO: will reduce file size
        T value = shared;

        // compare against default value, skip serialization if this is the default
        if (!controller.getObjectProperty(defaultValues[o->kind()], prop, shared))
        {
            return Result::Error(o, prop);
        }
        if (value == shared)
        {
            return Result::Ok();
        }
        */

        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_xcos],  rawKnownStr[element], nullptr));
        if (status.error())
        {
            return status;
        }
    
        if (can_write_to_string(shared))
        {
            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_value], BAD_CAST(base64::to_string(shared).c_str())));
            if (status.error())
            {
                return status;
            }
        }
        else
        {
            std::string str = base64::encode(shared);

            // Uncomment to check the base64 encoding
            // this is a conservative approach to ensure the saved file will be readable later
            // T decoded;
            // base64::decode(str, decoded);
            // if (decoded != shared)
            // {
            //     // error in base64 encoding
            //     return Result::Error(o, prop);
            // }

            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_base64], BAD_CAST(str.c_str())));
            if (status.error())
            {
                return status;
            }
        }
    
        status = Result::FromXML(xmlTextWriterEndElement(writer));
        if (status.error())
        {
            return status;
        }
        
        return Result::Ok();
    }
    Result writeElementGeometry(xmlTextWriterPtr writer, model::BaseObject* o);
    Result writeConnections(xmlTextWriterPtr writer, ChildrenCategories& categories);
    Result writeConnection(xmlTextWriterPtr writer, model::BaseObject* connection, ChildrenCategories& categories);
    Result writeConnectionGeometry(xmlTextWriterPtr writer, model::BaseObject* connection);
    Result writeConnectionObjectProperties(xmlTextWriterPtr writer, model::BaseObject* c);
    Result writeXcosAnnotationObjectProperties(xmlTextWriterPtr writer, model::BaseObject* c);
    Result writeSystemGeometry(xmlTextWriterPtr writer, model::BaseObject* o, ChildrenCategories& categories);
    Result writeGraphicalElements(xmlTextWriterPtr writer, ChildrenCategories& categories);
    Result writeNote(xmlTextWriterPtr writer, model::BaseObject* o, ChildrenCategories& categories);
    Result writeUnits(xmlTextWriterPtr writer, model::BaseObject* o);
    Result writeUnit(xmlTextWriterPtr writer, model::Datatype* d);
    Result writeBaseUnit(xmlTextWriterPtr writer, model::Datatype* d);
    Result writeAnnotations(xmlTextWriterPtr writer);
    Result writeAnnotations(xmlTextWriterPtr writer, model::BaseObject* o, const ChildrenCategories& categories);
    Result writeAnnotationLabel(xmlTextWriterPtr writer, model::BaseObject* o);
    Result writeAnnotations(xmlTextWriterPtr writer, const ChildrenCategories::all_port_t &o);

    /*
     * Load helpers
     */
    int loadSystemStructureDescription(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadDefaultExperiment(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadSystem(xmlTextReaderPtr reader, model::BaseObject* o);
    int updateSystem(model::BaseObject* o);
    int loadComponent(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadNote(xmlTextReaderPtr reader, model::BaseObject* o);
    template<typename T>
    int loadValue(xmlTextReaderPtr reader, enum xcosNames element, T& shared, std::function<int()> func)
    {
        // defensive programming: ensure this is the expected attribute
        if (readerConstInterned[element] != xmlTextReaderConstLocalName(reader))
        {
            return -1;
        }

        // iterate on attributes
        for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
        {
            const xmlChar* attribute = xmlTextReaderConstName(reader);
            auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), attribute);
            if (found == readerConstInterned.end())
            {
                continue;
            }
            enum xcosNames current = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));
            switch (current)
            {
                case e_base64:
                {
                    base64::decode((char*) xmlTextReaderConstValue(reader), shared);
                    return func();
                }

                case e_value:
                {
                    std::errc err = base64::from_string((char*) xmlTextReaderConstValue(reader), shared);
                    if (err != std::errc())
                    {
                        return -1;
                    }
                    return func();
                }
    
                default:
                    break;
            }
        }
        
        return 1;
    }
    template<typename T>
    int loadComponentObjectProperty(xmlTextReaderPtr reader, model::BaseObject* o, enum object_properties_t prop, enum xcosNames element, T& shared)
    {
        return loadValue(reader, element, shared, [&](){
            if (controller.setObjectProperty(o, prop, shared) == FAIL)
            {
                return -1;
            }
            return 1;
        });
    }
    int loadConnector(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnectorContent(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadParameterSet(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadParameter(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadUnit(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadBaseUnit(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnection(xmlTextReaderPtr reader, model::BaseObject* o);

    int loadReal(xmlTextReaderPtr reader, model::BaseObject* o, enum xcosNames name);
    int loadInteger(xmlTextReaderPtr reader, model::BaseObject* o, enum xcosNames name);
    int loadBoolean(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadString(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadEnumeration(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadBinary(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadDimension(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadClock(xmlTextReaderPtr reader, model::BaseObject* o);
    
    int loadSystemGeometry(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnectorGeometry(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadElementGeometry(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnectionGeometry(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadGeometry(xmlTextReaderPtr reader, model::BaseObject* o);

    int loadAnnotation(xmlTextReaderPtr reader, model::BaseObject* o);

    int processNode(xmlTextReaderPtr reader);
    int processElement(xmlTextReaderPtr reader, const xmlChar* nsURI);
    int processText(xmlTextReaderPtr reader);
    int processEndElement(xmlTextReaderPtr reader);

    // intern known string to speedup and simplify comparaison 
    void internPredefinedStrings(xmlTextReaderPtr reader);
    
    // copy property from src to dest using shared for intermediate storage
    template<typename T> inline
    update_status_t copy_property(model::BaseObject* src, object_properties_t src_prop, model::BaseObject* dest, object_properties_t dest_prop, T& shared)
    {
        if (controller.getObjectProperty(src, src_prop, shared))
        {
            return controller.setObjectProperty(dest, dest_prop, shared);
        }
        return FAIL;
    }
    template<typename T> inline
    update_status_t copy_property(model::BaseObject* src, model::BaseObject* dest, object_properties_t prop, T& shared)
    {
        return copy_property<T>(src, prop, dest, prop, shared);
    }
    void assignInnerPortIndexes(model::BaseObject* parent);
    int assignIOBlockChildren(model::BaseObject* parent, bool alwaysAssign);
    void assignOutterPortIndexes(model::BaseObject* parent);

private:
    /* shared controller */
    Controller controller;
    /* id of the diagram */
    ScicosID root;
        
    /* raw string content */
    std::array<const xmlChar*, NB_XCOS_NAMES> rawKnownStr;


    /*
     * load shared variables
     */
    
    /* temporary interned string content (owned by a reader), can be compared by pointer value */
    std::array<const xmlChar*, NB_XCOS_NAMES> readerConstInterned;

    /* temporary interned xsi namespace (owned by a reader) */
    const xmlChar* xmlnsXCOS;
    const xmlChar* xmlnsSSC;
    const xmlChar* xmlnsSSB;
    const xmlChar* xmlnsSSD;
    const xmlChar* xmlnsSSV;
    const xmlChar* xmlnsSSM;

    /* true is within an annotation, false otherwise */
    bool annotated;

    /* stack uid / kind used */
    std::vector<model::BaseObject*> processed;

    inline
    void processed_push(xmlTextReaderPtr reader, model::BaseObject* o)
    {
        if (!xmlTextReaderIsEmptyElement(reader))
            processed.push_back(o);
    }
    inline
    void processed_push(xmlTextReaderPtr reader)
    {
        if (!xmlTextReaderIsEmptyElement(reader) && !processed.empty())
            processed.push_back(processed.back());
    }

    /* temporary storage, currently decoded Dimension count */
    size_t dimensionCount;

    /* temporary storage, currently decoded Unit*/
    model::Unit unit;
    /* temporary storage, currently decoded Component name */
    std::string temporaryComponentName;

    /* uid string - ScicosID  map */
    struct Reference
    {
        std::string element;
        std::string connector;
        
        double x;
        double y;
        double systemInnerX;
        double systemInnerY;

        model::BaseObject* block;
        model::BaseObject* port;
        
        enum portKind kind;
        int index;

        inline Reference(std::string e, std::string c, model::BaseObject* b, model::BaseObject* p) : element(e), connector(c), x(0), y(0), systemInnerX(std::numeric_limits<double>::quiet_NaN()), systemInnerY(std::numeric_limits<double>::quiet_NaN()), block(b), port(p), kind(PORT_UNDEF), index(0) {};

        inline
        bool operator==(const Reference& other) const {
            return (element == other.element) && (connector == other.connector);
        }
    };
    std::vector<std::vector<Reference>> references;
    
    // shared temporary storage to retrieve values with Controller.getObjectProperties
    std::string _strShared;
    std::vector<double> _vecDblShared{0.};
    std::vector<int> _vecIntShared{0};
    std::vector<std::string> _vecStrShared{""};
    std::vector<ScicosID> _vecIDShared{ScicosID()};

    // Aspect ratio for visual compatibility between Dymola, EasySSP and Xcos
    // from "System Structure and Parameterization 1.0.1":
    // When importing or exporting systems, the nominal unit of the coordinates is 1 mm for all axis. The
    // nominal unit is intended to ensure similar visual sizing and appearances when combining systems from
    // different implementations
    const double ASPECT_RATIO = 1; // set to 1, was 3. with Dymola

    // computed SystemGeometry bounding box per layer
    // will be used to set flipping
    std::vector<SystemCanvas> bounds;


    /*
     * save shared variables
     */

    // temporary storage for the current archive state
    void* context; 

    // default object values per kind
    std::array<model::BaseObject*, 5> defaultValues{nullptr};
};

}

