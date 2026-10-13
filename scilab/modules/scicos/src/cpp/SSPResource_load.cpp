/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2023-2025 - Dassault Systèmes S.E. - Clément DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include <algorithm>
#include <cassert>
#include <cmath>   // for fabs
#include <cstdlib> // for atoi and atof
#include <cstring> // for strcmp and strchr
#include <string>
#include <string_view>
#include <vector>

#include <filesystem>

#include "SSPResource.hxx"
#include "expandPathVariable.h"
#include "scicos_base64.hxx"
#include "utilities.hxx"

// units are stored in the model without being mapped on the Controller yet
#include "LoggerView.hxx"
#include "Model.hxx"
#include "controller_helpers.hxx"
#include "model/Block.hxx"
#include "model/Diagram.hxx"

extern "C"
{
#include <archive.h>
#include <archive_entry.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlreader.h>

#include "Sciwarning.h"
#include "sci_malloc.h"
#include "sci_tmpdir.h"
#include "sci_types.h"
#include "sciprint.h"
}

#ifdef _MSC_VER
#undef min // to remove #define from windows header
#endif

namespace org_scilab_modules_scicos
{

/**
 * Display on the Scilab console
 */
static void console_print(void*, const char* msg, ...) LIBXML_ATTR_FORMAT(2, 3);
void console_print(void*, const char* msg, ...)
{
    // print the message
    va_list ap;
    va_start(ap, msg);
    scivprint(msg, ap);
    va_end(ap);
}

namespace
{

/**
 * Helper class to set / reset the XML parser state
 */
struct LibXML2State
{
    LibXML2State()
    {
        xmlGenericErrorFunc f = &console_print;
        xmlSetGenericErrorFunc(nullptr, f);
    }
    ~LibXML2State()
    {
        xmlSetGenericErrorFunc(nullptr, nullptr);
    }
};

/**
 * Helper class to allocate and clean the archive and libxml2 state
 */
struct State
{
    struct archive* ar;
    struct archive* ext;

    struct archive_entry* ac;

    State() : ar(archive_read_new()), ext(archive_write_disk_new()), ac(archive_entry_new()) {};

    ~State()
    {
        archive_entry_free(ac);
        archive_write_free(ext);
        archive_read_free(ar);
    };

    // input zip file
    struct archive* input() { return ar; };
    // output temp directory
    struct archive* directory() { return ext; };
    // current entry
    struct archive_entry* entry() { return ac; };
};

/**
 * Copy data from ar to aw
 */
int copy_data(struct archive* ar, struct archive* aw)
{
    la_ssize_t r;
    const void* buff;
    size_t size;
    la_int64_t offset;

    for (;;)
    {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return ((int)r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK)
        {
            sciprint("%s\n", archive_error_string(aw));
            return int(r);
        }
    }
};

int ioread(void* context, char* buffer, int len)
{
    struct archive* a = static_cast<struct archive*>(context);
    return (int)archive_read_data(a, buffer, len);
};

int ioclose(void* context)
{
    return 0;
};

std::string interface_function(enum portKind kind, bool isImplicit, bool isMainDiagram)
{
    std::string interfaceBlock[] = {"", "OUT_f", "IN_f", "CLKOUTV_f", "CLKINV_f"};

    if (isImplicit)
    {
        interfaceBlock[PORT_IN] = "OUTIMPL_f";
        interfaceBlock[PORT_OUT] = "INIMPL_f";
    }

    if (isMainDiagram)
    {
        // corner case, this is implemented as fake subsystem
        interfaceBlock[PORT_IN] = "SSPOutputConnector";
        interfaceBlock[PORT_OUT] = "SSPInputConnector";
    }

    return interfaceBlock[kind];
}

std::string simulation_function(enum portKind kind, bool isImplicit, bool isMainDiagram)
{
    std::string simulationFunction[] = {"", "output", "input", "output", "input"};

    if (isImplicit)
    {
        simulationFunction[PORT_OUT] = "outimpl";
        simulationFunction[PORT_IN] = "inimpl";
    }

    if (isMainDiagram)
    {
        // corner case, this is implemented as fake subsystem
        simulationFunction[PORT_IN] = "csuper";
        simulationFunction[PORT_OUT] = "csuper";
    }

    return simulationFunction[kind];
}

}; /* anonymous namespace */

int SSPResource::load(const char* uri)
{
    int ret = 0;

    State st;

    /*
     * Decompress the SSP zip file into a temporary directory
     */
    archive_read_support_format_all(st.input());
    archive_read_support_filter_all(st.input());
    if (archive_read_open_filename(st.input(), uri, BLOCK_SIZE) != ARCHIVE_OK)
    {
        sciprint("Unable to open %s\n", uri);
        return -1;
    }

    /*
     * SystemStructure is extracted in memory, prepare a directory for other contents (fmu or other ressources)
     */
    archive_write_disk_set_standard_lookup(st.directory());
    /* Select which attributes we want to restore. */
    int flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;
    archive_write_disk_set_options(st.directory(), flags);
    archive_write_disk_set_standard_lookup(st.directory());

    for (;;)
    {
        int res = archive_read_next_header2(st.input(), st.entry());
        if (res == ARCHIVE_EOF)
        {
            break;
        }
        if (res < ARCHIVE_WARN)
        {
            sciprint("Unable to load %s: %s\n", uri, archive_error_string(st.input()));
            return -1;
        }
        if (res == ARCHIVE_WARN)
        {
            Sciwarning("Warning on %s load: %s\n", uri, archive_error_string(st.input()));
        }

        // fprintf(stderr, "reading %s\n", archive_entry_pathname(st.entry()));
        const char* pathname = archive_entry_pathname(st.entry());
        if (strcmp(pathname, "SystemStructure.ssd") == 0)
        {
            /*
             * Load the main system structure, this is a mandatory file.
             *
             * Allocate the reader object, this API is used as it is simpler to use than SAX2 :
             *  * we have direct access to a node object
             *  * Strings are interned by libxml2
             *  * partial SAX2 callbacks are not supported by libxml2
             */
            xmlTextReaderPtr reader;
            /* resolve xinclude and intern strings */

            reader = xmlReaderForIO(ioread, ioclose, st.input(), uri, NULL, XML_PARSE_XINCLUDE | XML_PARSE_COMPACT);
            internPredefinedStrings(reader);

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
                int line = xmlGetLineNo(xmlTextReaderCurrentNode(reader));
                xmlFreeTextReader(reader);
                if (ret < 0)
                {
                    sciprint("zip://%s#%s line %d was not parsed\n", uri, pathname, line);
                    return ret;
                }
            }
            else
            {
                sciprint("Unable to open %s\n", uri);
                return -1;
            }
        }
        else if (strncmp(pathname, "resources/", 10) == 0 || strncmp(pathname, "extra/", 6) == 0 || strncmp(pathname, "documentation/", 14) == 0)
        {
            // other files are extracted into the disk and can be used later
            char* tmpdir = getTMPDIR();
            const std::string fullPathname = tmpdir + std::string("/") + pathname;
            FREE(tmpdir);
            archive_entry_set_pathname(st.entry(), fullPathname.c_str());

            res = archive_write_header(st.directory(), st.entry());
            if (res < ARCHIVE_WARN)
            {
                sciprint("Unable to load %s: %s\n", uri, archive_error_string(st.directory()));
                return -1;
            }
            if (res == ARCHIVE_WARN)
            {
                Sciwarning("Warning on %s loading: %s\n", uri, archive_error_string(st.directory()));
            }
            res = copy_data(st.input(), st.directory());
            if (res < ARCHIVE_WARN)
            {
                sciprint("Unable to load %s: %s\n", uri, archive_error_string(st.directory()));
                return -1;
            }
            if (res == ARCHIVE_WARN)
            {
                Sciwarning("Warning on %s loading: %s\n", uri, archive_error_string(st.directory()));
            }
        }
        else
        {
            sciprint("The variant %s is not loaded from %s\n", pathname, uri);
        }
    }

    return ret;
}

/*
 * Convert an XML UTF-8 string to a model string
 */
static std::string to_string(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return "";
    }

    // the strings in the model are stored as UTF-8 as in libxml2
    return std::string((const char*)xmlStr);
}

/*
 * return the remanining string if the prefix is found, nullptr otherwise
 */
template<std::size_t N>
constexpr const xmlChar* starts_with(const xmlChar* str, char const (&prefix)[N])
{
    if (str == nullptr)
    {
        return nullptr;
    }

    // will get inlined with a constexpr prefix and do per character comparison
    if ((*(char*)str) != *prefix)
    {
        return nullptr;
    }

    return starts_with<N - 1>(str + 1, (const char (&)[N - 1])(*(prefix + 1)));
}
template<>
constexpr const xmlChar* starts_with<1>(const xmlChar* str, char const (&prefix)[1])
{
    if (str == nullptr)
    {
        return nullptr;
    }

    // char is the remaining string
    return str;
}

// equals_to():  return true if the string is equal to the value, false otherwise
template<std::size_t N>
constexpr bool equals_to(const xmlChar* str, char const (&value)[N])
{
    // will get inlined with a constexpr prefix and do per character comparison
    if (str == nullptr)
    {
        return false;
    }

    if ((*(char*)str) != *value)
    {
        return false;
    }

    return equals_to<N - 1>(str + 1, (const char (&)[N - 1])(*(value + 1)));
}
template<>
constexpr bool equals_to<1>(const xmlChar* str, char const (&value)[1])
{
    if (str == nullptr)
    {
        return false;
    }

    // str is at the end, check its null terminator
    return *str == '\0';
}

/*
 * Convert an XML UTF-8 string to a model int
 */
static int to_int(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return 0;
    }

    return std::atoi((const char*)xmlStr);
}

/*
 * Convert an XML UTF-8 string to a model boolean
 */
static bool to_boolean(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return false;
    }

    return equals_to(xmlStr, "true");
}

/*
 * Convert an XML UTF-8 string to a model double
 */
static double to_double(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return 0.0;
    }

    return std::atof((const char*)xmlStr);
}

int SSPResource::loadSystemStructureDescription(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM);

    std::vector<std::string> namespaces;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        const xmlChar* value = xmlTextReaderConstValue(reader);

        // record XML namespaces used for annotations
        const xmlChar* xmlns_ = starts_with(attribute, "xmlns:");
        if (xmlns_ != nullptr)
        {
            if (equals_to(xmlns_, "ssc") ||
                equals_to(xmlns_, "ssb") ||
                equals_to(xmlns_, "ssd") ||
                equals_to(xmlns_, "ssv") ||
                equals_to(xmlns_, "ssm") ||
                equals_to(xmlns_, "xcos"))
            {
                // used namespaces, will always be added on saving
                continue;
            }
            else
            {
                // namespace from other tools, store now and add on saving
                std::string ns{to_string(attribute)};
                ns.append("=");
                ns.append(to_string(value));
                namespaces.emplace_back(ns);
                continue;
            }
        }

        auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), attribute);
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));
        switch (current)
        {
            case e_version:
            {
                std::string version = to_string(xmlTextReaderConstValue(reader));
                if (version != std::string("1.0") && std::string(version, 0, 3) != std::string("2.0"))
                {
                    sciprint("SSD version %s is not supported\n", version.c_str());
                    return -1;
                }
                break;
            }

            case e_name:
            {
                std::string name = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, NAME, name) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_author:
            {
                std::string author = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, AUTHOR, author) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_fileversion:
            {
                std::string fileversion = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, FILE_VERSION, fileversion) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_copyright:
            {
                std::string copyright = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, COPYRIGHT, copyright) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_license:
            {
                std::string license = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, LICENSE, license) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_generationTool:
            {
                std::string generationTool = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, GENERATION_TOOL, generationTool) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_generationDateAndTime:
            {
                std::string generationDateAndTime = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, GENERATION_DATE, generationDateAndTime) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    // set the global XML namespaces
    if (controller.setObjectProperty(o, GLOBAL_XMLNS, namespaces) == FAIL)
    {
        return -1;
    }

    return 1;
}

int SSPResource::loadSystem(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM || o->kind() == BLOCK);

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
            case e_name:
            {
                auto v = xmlTextReaderConstValue(reader);
                temporaryComponentName = to_string(v);
                if (starts_with(v, "#") == nullptr)
                {
                    // the name has been set by the user, preserve it
                    if (controller.setObjectProperty(o, NAME, temporaryComponentName) == FAIL)
                    {
                        return -1;
                    }
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::updateSystem(model::BaseObject* o)
{
    //
    // relocate the IOBlock according to the Geometry
    //
    if (bounds.empty() || references.empty())
    {
        return 0; // skip processing if no bounds or references
    }
    auto [x1, y1, x2, y2] = bounds.back();

    // helper lambda function
    auto set_ioblock_geometry = [this, x1, y1, x2, y2](const Reference& ioBlock)
    {
        controller.getObjectProperty(ioBlock.block, GEOMETRY, _vecDblShared);

        // has been set by xcos:geometry
        if (_vecDblShared.size() >= 2 && _vecDblShared[0] != 0. && _vecDblShared[1] != 0)
        {
            return 0;
        }

        // x
        _vecDblShared[0] = (x1 + ioBlock.x * (x2 - x1) + 10) * ASPECT_RATIO;
        // y
        _vecDblShared[1] = ((1.0 - ioBlock.y) * (y2 - y1) - 10 - y2) * ASPECT_RATIO;
        // w
        _vecDblShared[2] = 40;
        // h
        _vecDblShared[3] = 30;

        if (controller.setObjectProperty(ioBlock.block, GEOMETRY, _vecDblShared) == FAIL)
        {
            sciprint("unable to set SystemGeometry\n");
            return -1;
        }

        return 0;
    };

    auto r_layer = references.back();
    if (references.size() == 1)
    {
        // position main System's connectors
        for (std::vector<Reference>::iterator ioBlock = r_layer.begin(); ioBlock != r_layer.end() && ioBlock->element == ""; ++ioBlock)
        {
            int ret = set_ioblock_geometry(*ioBlock);
            if (ret)
            {
                return ret;
            }
        }
    }
    else
    {
        // set some properties on connectors after layer loading completed
        for (auto it = r_layer.rbegin(); it != r_layer.rend(); it++)
        {
            int ret = set_ioblock_geometry(*it);
            if (ret)
            {
                return ret;
            }
        }
    }

    // translate y-axis on blocks and links according to the SystemGeometry
    // y-axis should already be inverted
    std::vector<ScicosID> children;
    controller.getObjectProperty(o, CHILDREN, children);
    for (ScicosID id : children)
    {
        model::BaseObject* child = controller.getBaseObject(id);

        switch (child->kind())
        {
            case BLOCK:
            {
                controller.getObjectProperty(child, GEOMETRY, _vecDblShared);
                _vecDblShared[0] = _vecDblShared[0] - x1;
                _vecDblShared[1] = _vecDblShared[1] + y2;
                controller.setObjectProperty(child, GEOMETRY, _vecDblShared);
                break;
            }
            case LINK:
            {
                controller.getObjectProperty(child, CONTROL_POINTS, _vecDblShared);
                for (size_t i = 1; i < _vecDblShared.size(); i += 2)
                {
                    _vecDblShared[i - 1] = _vecDblShared[i - 1] - x1;
                    _vecDblShared[i] = _vecDblShared[i] + y2;
                }
                controller.setObjectProperty(child, CONTROL_POINTS, _vecDblShared);
                break;
            }
            case ANNOTATION:
            {
                controller.getObjectProperty(child, GEOMETRY, _vecDblShared);
                _vecDblShared[0] = _vecDblShared[0] - x1;
                _vecDblShared[1] = _vecDblShared[1] + y2;
                controller.setObjectProperty(child, GEOMETRY, _vecDblShared);
                break;
            }
            default:
                break;
        }
    }

    return 1;
}

int SSPResource::loadDefaultExperiment(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM);

    double startTime = 0.;
    double stopTime = 30.;

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
            case e_startTime:
            {
                startTime = to_double(xmlTextReaderConstValue(reader));
                break;
            }

            case e_stopTime:
            {
                stopTime = to_double(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                break;
        }
    }

    std::vector<double> properties;
    controller.getObjectProperty(o, PROPERTIES, properties);

    properties[0] = stopTime - startTime;
    if (controller.setObjectProperty(o, PROPERTIES, properties) == FAIL)
    {
        return -1;
    }

    return 1;
}

int SSPResource::loadConnector(xmlTextReaderPtr reader, model::BaseObject* parent)
{
    int ret;
    model::BaseObject* innerBlock = nullptr;
    model::BaseObject* innerPort = nullptr;
    model::BaseObject* outterPort = nullptr;
    model::BaseObject* port = nullptr;

    auto& r_layer = references.back();
    bool isMainDiagram = references.size() == 1;
    if (isMainDiagram)
    {
        innerBlock = controller.createBaseObject(BLOCK);
        innerPort = controller.createBaseObject(PORT);
        controller.setObjectProperty(innerPort, SOURCE_BLOCK, innerBlock);

        r_layer.emplace_back(Reference("", "", innerBlock, innerPort));

        processed_push(reader, innerPort);
        port = innerPort;
    }
    else
    {
        auto& r_parent_layer = *(references.rbegin() + 1);

        outterPort = controller.createBaseObject(PORT);
        controller.setObjectProperty(outterPort, SOURCE_BLOCK, parent);

        innerBlock = controller.createBaseObject(BLOCK);
        innerPort = controller.createBaseObject(PORT);
        controller.setObjectProperty(innerPort, SOURCE_BLOCK, innerBlock);

        r_parent_layer.emplace_back(Reference(temporaryComponentName, "", parent, outterPort));
        r_layer.emplace_back(Reference("", "", innerBlock, innerPort));

        processed_push(reader, outterPort);
        port = outterPort;
    }

    ret = loadConnectorContent(reader, port);
    if (ret != 1)
    {
        sciprint("unable to set Connector\n");
        return ret;
    }

    return ret;
}

int SSPResource::loadConnectorContent(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT);

    // reset Dimension count, see loadDimension
    dimensionCount = 0;

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
            case e_id:
            {
                std::string id = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, UID, id) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }
            case e_name:
            {
                const xmlChar* v = xmlTextReaderConstValue(reader);
                if (starts_with(v, "#") == nullptr)
                {
                    if (controller.setObjectProperty(o, NAME, to_string(v)) == FAIL)
                    {
                        return -1;
                    }
                }
                references.back().back().connector = to_string(v);
                break;
            }

            case e_kind:
            {
                // input / output / inout are interned
                const xmlChar* kind = xmlTextReaderConstString(reader, xmlTextReaderConstValue(reader));
                auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), kind);
                if (found == readerConstInterned.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));

                enum portKind port = PORT_UNDEF;
                bool isImplicit = false;
                switch (current)
                {
                    case e_input:
                        port = PORT_IN;
                        break;
                    case e_output:
                        port = PORT_OUT;
                        break;
                    case e_parameter:
                        port = PORT_IN;
                        break;
                    case e_calculatedParameter:
                        port = PORT_OUT;
                        break;
                    case e_acausal: // fallthrough
                    case e_inout:
                        port = PORT_IN; // will be updated on geometry update
                        isImplicit = true;
                        break;
                    default:
                        return -1;
                }

                if (controller.setObjectProperty(o, PORT_KIND, port) == FAIL)
                {
                    return -1;
                }
                if (controller.setObjectProperty(o, IMPLICIT, isImplicit) == FAIL)
                {
                    return -1;
                }
                references.back().back().kind = port;
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    LoggerView* logger = get_or_allocate_logger();
    logger->log(LOG_DEBUG, [&](char* first, char* last)
    {
        const auto& r = references.back().back();
        return to_chars_t(first, last) + "block " + logger->id(r.block) + " port " + logger->id(r.port) + " named " + std::string_view(r.element) + " " + std::string_view(r.connector) + "\n";
    });
    return 1;
}

int SSPResource::loadReal(xmlTextReaderPtr reader, org_scilab_modules_scicos::model::BaseObject* o, enum xcosNames name)
{
    assert(o->kind() == PORT || o->kind() == BLOCK || o->kind() == DIAGRAM);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 1) == FAIL)
            {
                return -1;
            }

            if (xmlTextReaderMoveToFirstAttribute(reader) > 0)
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), attribute);
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));
                if (current == e_unit)
                {
                    std::string unit = to_string(xmlTextReaderConstValue(reader));
                    // put the unit on the outter port
                    if (controller.setObjectProperty(o, PARAMETER_UNIT, unit) == FAIL)
                    {
                        return -1;
                    }
                }
            }
            break;
        }

        case DIAGRAM: // fallthrough
        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("real");
            }
            else
            {
                types.back() = "real";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
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
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }
                    case e_unit:
                    {
                        std::vector<std::string> units;
                        if (!controller.getObjectProperty(o, PARAMETER_UNIT, units))
                        {
                            return -1;
                        }
                        units.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_UNIT, units) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadInteger(xmlTextReaderPtr reader, model::BaseObject* o, enum xcosNames name)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 3) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("integer");
            }
            else
            {
                types.back() = "integer";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
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
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadBoolean(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 5) == FAIL)
            {
                return -1;
            }
            break;
        }
        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("boolean");
            }
            else
            {
                types.back() = "boolean";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
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
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadString(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            // string can be passed around as a pointer to a string
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 1) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("string");
            }
            else
            {
                types.back() = "string";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
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
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadEnumeration(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 3) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("enumeration");
            }
            else
            {
                types.back() = "enumeration";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
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
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadBinary(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            // from the specification: a length-terminated binary data type
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 5) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("binary");
            }
            else
            {
                types.back() = "binary";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
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
                    case e_source: // fallthrough
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    case e_mime_type:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_ENCODING, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_ENCODING, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadDimension(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT);

    // as been reset on loadConnector
    dimensionCount++;

    int sz = 1;

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
            case e_size:
            {
                sz = to_int(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    object_properties_t p;
    switch (dimensionCount)
    {
        case 1:
            p = DATATYPE_ROWS;
            break;
        case 2:
            p = DATATYPE_COLS;
            break;
        default:
            // unable to decode or used dimension higher than 2
            return -1;
    }

    if (controller.setObjectProperty(o, p, sz) == FAIL)
    {
        return -1;
    }

    return 1;
}

int SSPResource::loadClock(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT);

    auto& r_layer = references.back();
    enum portKind& kind = r_layer.back().kind;

    // set the type as Clock
    if (kind == PORT_IN)
    {
        kind = PORT_EIN;
        if (controller.setObjectProperty(o, PORT_KIND, PORT_EIN) == FAIL)
        {
            return -1;
        }
    }
    else if (kind == PORT_OUT)
    {
        kind = PORT_EOUT;
        if (controller.setObjectProperty(o, PORT_KIND, PORT_EOUT) == FAIL)
        {
            return -1;
        }
    }
    return 1;
}

int SSPResource::loadConnection(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    model::BaseObject* link = controller.createBaseObject(LINK);

    // assign the child
    model::BaseObject* parent = processed.back();
    controller.setObjectProperty(link, PARENT_DIAGRAM, root);
    if (parent->kind() == BLOCK)
    {
        controller.setObjectProperty(link, PARENT_BLOCK, parent->id());
    }
    controller.getObjectProperty(parent, CHILDREN, _vecIDShared);
    _vecIDShared.push_back(link->id());
    controller.setObjectProperty(parent, CHILDREN, _vecIDShared);

    // store into processed if there is children
    processed_push(reader, link);

    std::string startElement;
    std::string startConnector;
    std::string endElement;
    std::string endConnector;

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
            case e_startElement:
            {
                startElement = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_startConnector:
            {
                startConnector = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_endElement:
            {
                endElement = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_endConnector:
            {
                endConnector = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_suppressUnitConversion:
            {
                // FIXME: not decoded yet ; should incompatible unit produce an error ?
                break;
            }
            case e_id:
            {
                std::string id = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(link, NAME, id) == FAIL)
                {
                    return -1;
                }
                break;
            }
            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(link, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                sciprint("unable to decode Connection\n");
                return -1;
        }
    }

    // resolve linkage with available references
    auto r_layer = references.back();
    auto startIT = std::find_if(r_layer.rbegin(), r_layer.rend(), [&startElement, &startConnector](const Reference& r)
                                { return r.element == startElement && r.connector == startConnector; });
    if (startIT == r_layer.rend())
    {
        sciprint("unable to decode Connection - startConnector reference for startElement=\"%s\" startConnector=\"%s\"\n", startElement.c_str(), startConnector.c_str());
        return -1;
    }

    if (controller.setObjectProperty(link, SOURCE_PORT, startIT->port->id()) == FAIL)
    {
        sciprint("unable to decode Connection - startConnector set\n");
        return -1;
    }
    if (controller.setObjectProperty(startIT->port, CONNECTED_SIGNALS, link->id()) == FAIL)
    {
        sciprint("unable to decode Connection - startConnector set block\n");
        return -1;
    }

    auto endIT = std::find_if(r_layer.rbegin(), r_layer.rend(), [&endElement, &endConnector](const Reference& r)
                              { return r.element == endElement && r.connector == endConnector; });
    if (endIT == r_layer.rend())
    {
        sciprint("unable to decode Connection - endConnector reference for endElement=\"%s\" endConnector=\"%s\"\n", endElement.c_str(), endConnector.c_str());
        return -1;
    }

    if (controller.setObjectProperty(link, DESTINATION_PORT, endIT->port->id()) == FAIL)
    {
        sciprint("unable to decode Connection - endConnector set\n");
        return -1;
    }
    if (controller.setObjectProperty(endIT->port, CONNECTED_SIGNALS, link->id()) == FAIL)
    {
        sciprint("unable to decode Connection - endConnector set block\n");
        return -1;
    }

    // verbose for debugging
    LoggerView* logger = get_or_allocate_logger();
    logger->log(LOG_DEBUG, [&](char* first, char* last) {
        return to_chars_t(first, last) + "connect link " + logger->id(link) + " : from " + logger->id(startIT->port) + " - to " + logger->id(endIT->port) + "\n";
    });

    return 1;
}

int SSPResource::loadSystemGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM || o->kind() == BLOCK);

    // update in place, will be processed at system exit
    if (bounds.empty())
    {
        return 0; // skip processing if no bounds
    }
    auto& systemGeom = bounds.back();

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
            case e_x1:
            {
                systemGeom.x1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y1:
            {
                systemGeom.y1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_x2:
            {
                systemGeom.x2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y2:
            {
                systemGeom.y2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                break;
        }
    }

    return 1;
}

int SSPResource::loadConnectorGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    // the argument should be the associated IOBlock
    assert(o->kind() == BLOCK || o->kind() == PORT);
    model::BaseObject* port;
    if (o->kind() == PORT)
    {
        port = o;
    }
    else
    {
        port = references.back().back().port;
    }

    // in SSP coordinates
    double x = 1.05;
    double y = 0.2;
    double systemInnerX = std::numeric_limits<double>::quiet_NaN();
    double systemInnerY = std::numeric_limits<double>::quiet_NaN();

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
            case e_x:
            {
                x = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y:
            {
                y = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_systemInnerX:
            {
                systemInnerX = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_systemInnerY:
            {
                systemInnerY = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            default:
                sciprint("unable to decode ConnectorGeometry\n");
                return -1;
        }
    }

    // all IMPLICIT connectors are allocated as INPUTS, depending on the geometry they can be moved to OUTPUTS
    bool isImplicit;
    controller.getObjectProperty(port, IMPLICIT, isImplicit);
    if (isImplicit)
    {
        std::vector<ScicosID> ports;
        controller.getObjectProperty(o, INPUTS, ports);

        if (x > 0.5)
        {
            enum portKind k = PORT_OUT;

            auto newEnd = std::remove(ports.begin(), ports.end(), o->id());
            ports.erase(newEnd);
            controller.setObjectProperty(o, INPUTS, ports);

            controller.setObjectProperty(o, PORT_KIND, k);

            object_properties_t opposite = property_from_port(k);
            std::vector<ScicosID> ports;
            controller.getObjectProperty(o, opposite, ports);
            ports.push_back(o->id());
            if (controller.setObjectProperty(o, opposite, ports) == FAIL)
            {
                return -1;
            }
        }
    }

    // set the geometry on the ioBlock
    auto ioBlock = references.back().rbegin();
    ioBlock->x = x;
    ioBlock->y = y;
    ioBlock->systemInnerX = systemInnerX;
    ioBlock->systemInnerY = systemInnerY;

    // set the geometry on the port
    if (references.size() > 1)
    {
        auto& outter = (references.rbegin() + 1)->back();
        if (ioBlock->element == "" && ioBlock->connector == outter.connector)
        {
            outter.x = x;
            outter.y = y;
            outter.systemInnerX = systemInnerX;
            outter.systemInnerY = systemInnerY;
        }
    }

    return 1;
}

int SSPResource::loadElementGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK);

    // in Xcos coordinates
    std::vector<double> x_y_w_h;
    x_y_w_h.resize(4);

    // in SSP coordinates
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double rotation = 0;

    // for images
    std::string iconSource;
    // double iconRotation;
    // bool iconFlip;
    // bool fixedAspectRatio;

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
            case e_x1:
            {
                x1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y1:
            {
                y1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_x2:
            {
                x2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y2:
            {
                y2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_rotation:
            {
                rotation = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_iconSource:
            {
                iconSource = to_string(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                break;
        }
    }

    // (x1,y1) and (x2,y2) define the positions of the lower-left and upper-right corners of the model element in the coordinate system of its parent system. If x1>x2 this indicates horizontal flipping, y1>y2 indicates vertical flipping.
    // The optional attribute rotation (in degrees) defines an additional rotation (applied after flipping), where positive numbers indicate a counter clockwise rotation.

    std::string style;
    controller.getObjectProperty(o, INTERFACE_FUNCTION, style);

    // special style for any FMU block and where the fmu_wrapper toolbox is not already loaded
    if (style == std::string("SimpleFMU"))
    {
        style += ";blockWithLabel;displayedLabel=FMU %1$s";
    }

    if (x1 < x2)
    {
        x_y_w_h[0] = x1;
        x_y_w_h[2] = x2 - x1;
    }
    else
    {
        x_y_w_h[0] = x2;
        x_y_w_h[2] = x1 - x2;

        style += ";mirror=true";
    }
    // y-axis will be translated on SystemGeometry decoding
    if (y1 < y2)
    {
        x_y_w_h[1] = -y2;
        x_y_w_h[3] = y2 - y1;
    }
    else
    {
        x_y_w_h[1] = -y1;
        x_y_w_h[3] = y2 - y1;

        style += ";flip=true";
    }

    if (controller.setObjectProperty(o, GEOMETRY, x_y_w_h) == FAIL)
    {
        sciprint("unable to set ElementGeometry\n");
        return -1;
    }

    if (std::abs(rotation) > std::numeric_limits<double>::epsilon())
    {
        style += ";rotation=" + std::to_string(rotation);
    }
    if (controller.setObjectProperty(o, STYLE, style) == FAIL)
    {
        sciprint("unable to set ConnectorGeometry\n");
        return -1;
    }

    return 1;
}

int SSPResource::loadConnectionGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == LINK);

    // read values
    std::vector<double> points;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        bool allocated = !points.empty();

        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), attribute);
        if (found == readerConstInterned.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));
        switch (current)
        {
            case e_pointsX:
            {
                char* ptr = (char*)xmlTextReaderConstValue(reader);
                char* end;
                double x = std::strtod(ptr, &end);
                double y = 0.;
                for (size_t i = 0; ptr != end; x = std::strtod(ptr, &end), i++)
                {
                    ptr = end;

                    if (allocated && (2 * i + 1) < points.size())
                    {
                        points[2 * i] = ASPECT_RATIO * x;
                    }
                    else
                    {
                        points.push_back(ASPECT_RATIO * x);
                        points.push_back(y);
                    }
                }
                break;
            }

            case e_pointsY:
            {
                char* ptr = (char*)xmlTextReaderConstValue(reader);
                char* end;
                double x = 0.;
                double y = std::strtod(ptr, &end);
                for (size_t i = 0; ptr != end; y = std::strtod(ptr, &end), i++)
                {
                    ptr = end;

                    if (allocated && (2 * i + 1) < points.size())
                    {
                        points[2 * i + 1] = ASPECT_RATIO * -y;
                    }
                    else
                    {
                        points.push_back(x);
                        points.push_back(ASPECT_RATIO * -y);
                    }
                }
                break;
            }

            default:
                sciprint("unable to decode ConnectionGeometry\n");
                return -1;
        }
    }

    // duplicate start and end points
    if (!points.empty() && points.size() >= 2)
    {
        points.insert(points.begin(), {points[0], points[1]});
        points.insert(points.end(), {points[points.size() - 2], points[points.size() - 1]});
    }

    if (controller.setObjectProperty(o, CONTROL_POINTS, points) == FAIL)
    {
        sciprint("unable to set ConnectionGeometry\n");
        return -1;
    }

    return 1;
}

int SSPResource::loadGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    // load Xcos geometry and reconciliate with SSP geometry
    std::vector<double> x_y_w_h;
    x_y_w_h.resize(4);

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
            case e_x:
            {
                x_y_w_h[0] = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y:
            {
                x_y_w_h[1] = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_width:
            {
                x_y_w_h[2] = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_height:
            {
                x_y_w_h[3] = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            default:
                sciprint("unable to decode Geometry\n");
                return -1;
        }
    }

    // set the geometry on the block
    model::BaseObject* geomObject = o;
    if (o->kind() == PORT)
    {
        geomObject = references.back().back().block;
    }

    if (!controller.getObjectProperty(geomObject, GEOMETRY, _vecDblShared))
    {
        sciprint("unable to get Geometry\n");
        return -1;
    }

    // TODO: reconcile the two geometries in objects has been moved on other tools
    //
    // Xcos geometry is in pixel (x,y,width,height):
    //  - the origin is at the top-left corner
    //  - increasing x-axis to the right, increasing y-axis is downward
    //  - width and height are positive
    //  - values are absolute in pixels * zoom factor
    // SSP geometry is two points : (x1,y1) (x2,y2):
    //  - the origin is at the center
    //  - increasing x-axis to the right, increasing y-axis is *upward*
    //  - x1 < x2 and y1 < y2 if no flipping
    //  - values are relative ; maximum value is given by all object in the layer

    if (controller.setObjectProperty(geomObject, GEOMETRY, x_y_w_h) == FAIL)
    {
        sciprint("unable to set Geometry\n");
        return -1;
    }

    return 1;
}

int SSPResource::loadParameterSet(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    switch (o->kind())
    {
        case BLOCK: // fallthrough
        case DIAGRAM:
        {
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
                    case e_version:
                    {
                        const xmlChar* version = xmlTextReaderConstValue(reader);
                        if (!equals_to(version, "1.0") && !equals_to(version, "2.0"))
                        {
                            sciprint("unable to decode ParameterSet version\n");
                            return -1;
                        }
                        break;
                    }
                    case e_name:
                    {
                        // the name is ignored, only one ParameterSet is handled
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            sciprint("unable to decode ParameterSet\n");
            return -1;
    }

    return 1;
}

int SSPResource::loadParameter(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    switch (o->kind())
    {
        case BLOCK: // fallthrough
        case DIAGRAM:
        {
            // first allocate a new "undefined" parameter
            std::vector<std::string> parameters;
            if (!controller.getObjectProperty(o, PARAMETER_NAME, parameters))
            {
                sciprint("unable to retrieve Parameter name\n");
                return -1;
            }
            parameters.push_back("");
            if (controller.setObjectProperty(o, PARAMETER_NAME, parameters) == FAIL)
            {
                sciprint("unable to assign Parameter name\n");
                return -1;
            }

            std::vector<std::string> descriptions;
            if (!controller.getObjectProperty(o, PARAMETER_DESCRIPTION, descriptions))
            {
                sciprint("unable to retrieve Parameter description\n");
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
                    case e_name:
                    {
                        parameters.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_NAME, parameters) == FAIL)
                        {
                            sciprint("unable to assign Parameter name\n");
                            return -1;
                        }
                        break;
                    }
                    case e_description:
                    {
                        descriptions.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_DESCRIPTION, descriptions) == FAIL)
                        {
                            sciprint("unable to assign Parameter name\n");
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            sciprint("unable to decode Parameter\n");
            return -1;
    }

    return 1;
}

int SSPResource::loadUnit(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    // allocate a new temporary unit
    unit = model::Unit();

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
            case e_name:
            {
                unit.name = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_description:
            {
                unit.description = to_string(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                sciprint("unable to decode Unit\n");
                return -1;
        }
    }

    return 1;
}

int SSPResource::loadBaseUnit(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    // decode the temporary unit, iterate on attributes
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
            case e_kg:
            {
                unit.kg = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_m:
            {
                unit.m = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_s:
            {
                unit.s = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_A:
            {
                unit.A = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_K:
            {
                unit.K = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_mol:
            {
                unit.mol = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_cd:
            {
                unit.cd = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_rad:
            {
                unit.rad = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_factor:
            {
                unit.factor = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_offset:
            {
                unit.offset = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            default:
                sciprint("unable to decode BaseUnit\n");
                return -1;
        }
    }

    // assign the unit to the default datatype on the correct layer
    if (o->kind() == BLOCK)
    {
        model::Block* block = (model::Block*)o;
        model::Datatype d;

        // the datatype with a unit is scalar double
        d.m_datatype_id = 1;
        d.m_rows = 1;
        d.m_columns = 1;

        // assign the temporary unit
        d.m_unit = unit;

        std::vector<model::Datatype*> datatypes = block->getDatatypes();
        datatypes.push_back(controller.getInternalModel().flyweight(std::move(d)));
        block->setDatatypes(datatypes);
    }
    else if (o->kind() == DIAGRAM)
    {
        model::Diagram* diagram = (model::Diagram*)o;
        model::Datatype d;

        // the datatype with a unit is scalar double
        d.m_datatype_id = 1;
        d.m_rows = 1;
        d.m_columns = 1;

        // assign the temporary unit
        d.m_unit = unit;

        std::vector<model::Datatype*> datatypes = diagram->getDatatypes();
        datatypes.push_back(controller.getInternalModel().flyweight(std::move(d)));
        diagram->setDatatypes(datatypes);
    }
    else
    {
        sciprint("unable to assign BaseUnit\n");
        return -1;
    }

    return 1;
}

int SSPResource::loadAnnotation(xmlTextReaderPtr reader, model::BaseObject* o)
{
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

            case e_type:
            {
                // only xcos URI is managed here, store others as raw strings
                const xmlChar* type = xmlTextReaderConstString(reader, xmlTextReaderConstValue(reader));
                if (type == readerConstInterned[e_org_scilab_xcos_ssp])
                {
                    // xcos annotation will be converted to model values
                }
                else
                {
                    //
                    // this is unknown annotation, store it as raw string
                    //

                    // read current node and its subtree
                    xmlNodePtr node = xmlTextReaderExpand(reader);
                    if (node == nullptr)
                    {
                        return -1;
                    }
                    // store into an output buffer
                    xmlDocPtr doc = xmlTextReaderCurrentDoc(reader);
                    xmlOutputBufferPtr output = xmlAllocOutputBuffer(nullptr);
                    if (output == nullptr)
                    {
                        return -1;
                    }
                    xmlNodeDumpOutput(output, doc, node, 0, 1, nullptr);

                    // store the annotation (within the current object or as global one)
                    enum object_properties_t p = SSP_ANNOTATION;
                    if (o->kind() == DIAGRAM && processed.size() <= 3)
                    {
                        // processed stack is: {SystemStructureDescription DIAGRAM, Annotations DIAGRAM, Annotation DIAGRAM}
                        p = GLOBAL_SSP_ANNOTATION;
                    }

                    if (!controller.getObjectProperty(o, p, _vecStrShared))
                    {
                        return -1;
                    }

                    // append the annotation
                    _vecStrShared.emplace_back((const char*)xmlOutputBufferGetContent(output), xmlOutputBufferGetSize(output));

                    if (xmlOutputBufferClose(output) < 0)
                    {
                        return -1;
                    }

                    if (controller.setObjectProperty(o, p, _vecStrShared) == FAIL)
                    {
                        return -1;
                    }

                    // skip the following nodes
                    annotated = true;
                    return 1;
                }
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::loadComponent(xmlTextReaderPtr reader, model::BaseObject* o)
{
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

            case e_type:
            {
                const xmlChar* type = xmlTextReaderConstString(reader, xmlTextReaderConstName(reader));
                auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), type);
                if (found == readerConstInterned.end())
                {
                    continue;
                }
                enum xcosNames t = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));
                switch (t)
                {
                    case e_application_x_fmu_sharedlibrary: // fallthrough
                    default:
                    {
                        controller.setObjectProperty(o, INTERFACE_FUNCTION, std::string("SimpleFMU"));
                        break;
                    }

                    case e_application_x_ssp_definition:
                    {
                        sciprint("application/x-ssp-definition is not supported\n");
                        return -1;
                        break;
                    }

                    case e_application_x_ssp_package:
                    {
                        sciprint("application/x-ssp-package is not supported\n");
                        return -1;
                        break;
                    }
                }
                break;
            }

            case e_source:
            {
                std::string source = to_string(xmlTextReaderConstValue(reader));

                const std::string fullPathname = std::string("TMPDIR/") + source;
                static constexpr const char prefix[] = "resources/";
                std::string resourcesPathname = source;
                if (source.compare(0, sizeof(prefix) - 1, prefix) == 0)
                {
                    resourcesPathname = source.substr(sizeof(prefix) - 1);
                }

                auto pFullPathname = std::filesystem::path(fullPathname);
                std::string workdir = pFullPathname.replace_filename(pFullPathname.stem()).string();

                // create the directories
                char* pStrWorkdir = expandPathVariable(workdir.c_str());
                auto pPathWorkdir = std::filesystem::path(pStrWorkdir);
                FREE(pStrWorkdir);
                std::filesystem::create_directories(pPathWorkdir.parent_path());
                std::filesystem::create_directories(pPathWorkdir);

                if (!controller.getObjectProperty(o, EXPRS, _vecStrShared))
                {
                    return -1;
                }
                if (_vecStrShared.size() < 2)
                {
                    _vecStrShared.resize(2);
                }
                _vecStrShared[0] = resourcesPathname;
                _vecStrShared[1] = workdir;
                if (controller.setObjectProperty(o, EXPRS, _vecStrShared) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_implementation:
            {
                // the implementation is mapped to a selected fmuImpl
                if (!controller.getObjectProperty(o, EXPRS, _vecStrShared))
                {
                    return -1;
                }
                if (_vecStrShared.size() < 3)
                {
                    _vecStrShared.resize(3);
                }

                const xmlChar* implementation = xmlTextReaderConstString(reader, xmlTextReaderConstValue(reader));
                auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), implementation);
                if (found == readerConstInterned.end())
                {
                    continue;
                }
                enum xcosNames impl = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));
                switch (impl)
                {
                        // TODO: select the FMU v3 impl
                    case e_ModelExchange:
                        _vecStrShared[2] = "me 2.0";
                        break;
                    case e_CoSimulation:
                        _vecStrShared[2] = "cs 2.0";
                        break;
                    case e_any:
                        _vecStrShared[2] = "";
                        break;
                    default:
                        sciprint("Component implementation \"%s\" is not supported\n", xmlTextReaderConstValue(reader));
                        return -1;
                }
                if (controller.setObjectProperty(o, EXPRS, _vecStrShared) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_name:
            {
                auto v = xmlTextReaderConstValue(reader);
                temporaryComponentName = to_string(v);
                if (starts_with(v, "#") == nullptr)
                {
                    // the name has been set by the user, preserve it
                    if (controller.setObjectProperty(o, NAME, temporaryComponentName) == FAIL)
                    {
                        return -1;
                    }
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::loadNote(xmlTextReaderPtr reader, model::BaseObject* o)
{
    // in Xcos coordinates
    std::vector<double> x_y_w_h;
    x_y_w_h.resize(4);

    // in SSP coordinates
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;

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
            case e_x1:
            {
                x1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y1:
            {
                y1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_x2:
            {
                x2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y2:
            {
                y2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_text:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    std::string style = "TEXT_f";
    if (x1 < x2)
    {
        x_y_w_h[0] = x1;
        x_y_w_h[2] = x2 - x1;
    }
    else
    {
        x_y_w_h[0] = x2;
        x_y_w_h[2] = x1 - x2;

        style += ";mirror=true";
    }
    // y-axis will be translated on SystemGeometry decoding
    if (y1 < y2)
    {
        x_y_w_h[1] = -y2;
        x_y_w_h[3] = y2 - y1;
    }
    else
    {
        x_y_w_h[1] = -y1;
        x_y_w_h[3] = y2 - y1;

        style += ";flip=true";
    }

    if (controller.setObjectProperty(o, GEOMETRY, x_y_w_h) == FAIL)
    {
        return -1;
    }
    if (controller.setObjectProperty(o, STYLE, style) == FAIL)
    {
        return -1;
    }

    return 1;
}

int SSPResource::processNode(xmlTextReaderPtr reader)
{
    auto logger = get_or_allocate_logger();
    const xmlChar* name = xmlTextReaderConstLocalName(reader);

    // verbose for debugging
    if (logger->getLevel() <= LOG_DEBUG)
    {
        int line = xmlGetLineNo(xmlTextReaderCurrentNode(reader));
        logger->log(LOG_DEBUG, "parsing line %d name %s\n", line, name);
    }

    // manage only xcos related XML nodes
    const xmlChar* nsURI = xmlTextReaderConstNamespaceUri(reader);
    if (nsURI == xmlnsXCOS || nsURI == xmlnsSSC || nsURI == xmlnsSSB || nsURI == xmlnsSSD || nsURI == xmlnsSSV || nsURI == xmlnsSSM || nsURI == nullptr)
    {
        xmlReaderTypes nodeType = (xmlReaderTypes)xmlTextReaderNodeType(reader);
        switch (nodeType)
        {
            case XML_READER_TYPE_NONE:
                return 1;
            case XML_READER_TYPE_ELEMENT:
                return processElement(reader, nsURI);
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
        if (annotated)
        {
            // within an ssc:Annotation
            // the content has already been stored as a string
            // skip up to its end
            return 1;
        }
    }

    int line = xmlGetLineNo(xmlTextReaderCurrentNode(reader));
    logger->log(LOG_ERROR, "unable to process %s node at line %d\n", name, line);
    return -1;
}

// assign IOBlock indexes if applicable
void SSPResource::assignInnerPortIndexes(model::BaseObject* parent)
{
    std::vector<Reference*> ioBlocks[] = {{}, {}, {}, {}, {}};

    auto& r_layer = references.back();

    // add all System's connectors, ioBlocks are the first elements without named element,
    for (std::vector<Reference>::iterator ioBlock = r_layer.begin(); ioBlock != r_layer.end() && ioBlock->element == ""; ++ioBlock)
    {
        ioBlocks[opposite_port(ioBlock->kind)].push_back(&*ioBlock);
    }

    // sort per reference position
    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
    {
        std::stable_sort(ioBlocks[(portKind)kind].begin(), ioBlocks[(portKind)kind].end(), [](Reference* a, Reference* b){
            if (std::fabs(a->x - b->x) <= std::numeric_limits<double>::epsilon())
                return a->y > b->y;
            return a->x < b->x;
        });
    }

    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
    {
        // layer computes its port number from its already decoded children
        for (int i = 0; i < ioBlocks[(portKind)kind].size(); ++i)
        {
            model::BaseObject* innerBlock = ioBlocks[(portKind)kind][i]->block;
            int index = i + 1;

            if (references.size() == 1)
            {
                // on an SSPInputConnector or SSPOutputConnector
                std::vector<std::string> exprs;
                if (kind == PORT_IN)
                {
                    exprs = {ioBlocks[(portKind)kind][i]->connector, "256"};
                }
                else
                {
                    exprs = {ioBlocks[(portKind)kind][i]->connector};
                }

                controller.setObjectProperty(innerBlock, EXPRS, exprs);
                controller.setObjectProperty(innerBlock, IPAR, 1);
            }
            else
            {
                // on a subsystem input/output
                controller.setObjectProperty(innerBlock, IPAR, index);
                controller.setObjectProperty(innerBlock, EXPRS, std::to_string(index));
            }
        }
    }
}

// assign IOBlocks for the Block implemented as sub-system
int SSPResource::assignIOBlockChildren(model::BaseObject* parent, bool alwaysAssign)
{
    if (!controller.getObjectProperty(parent, CHILDREN, _vecIDShared))
    {
        return -1;
    }
    size_t children_size = _vecIDShared.size();
    auto& r_layer = references.back();

    if (!alwaysAssign && children_size == 0)
    {
        // delete the I/O object
        for (const Reference& r : r_layer)
        {
            controller.deleteBaseObject(r.block);
        }

        return 0;
    }

    // assign the children and parent for I/O blocks
    for (const Reference& r : r_layer)
    {
        // only if the block is unnamed
        // (eg. not regular block outter port)
        if (r.element != "")
        {
            continue;
        }

        if (0 < r.index && r.index <= _vecIDShared.size())
        {
            _vecIDShared.insert(_vecIDShared.begin() + r.index - 1, r.block->id());
        }   
        else
        {
            _vecIDShared.push_back(r.block->id());
        }

        if (parent->kind() == BLOCK)
        {
            if (controller.setObjectProperty(r.block, PARENT_BLOCK, parent->id()) == FAIL)
            {
                return -1;
            }
        }
        if (controller.setObjectProperty(r.block, PARENT_DIAGRAM, root) == FAIL)
        {
            return -1;
        }
    }

    if (controller.setObjectProperty(parent, CHILDREN, _vecIDShared) == FAIL)
    {
        return -1;
    }
    return 1;
}

// assign port indexes
void SSPResource::assignOutterPortIndexes(model::BaseObject* parent)
{
    std::vector<Reference*> ports[] = {{}, {}, {}, {}, {}};

    auto& r_layer = references.back();

    // look for all Component's connectors (or System's outter ports)
    for (auto it = std::find_if(r_layer.begin(), r_layer.end(), [parent](const Reference& r)
                                { return r.block == parent; });
         it != r_layer.end() && it->block == parent;
         it++)
    {
        int kind = it->kind;
        if (it->kind == PORT_UNDEF)
        {
            controller.getObjectProperty(it->port, PORT_KIND, kind);
        }
        ports[(portKind) kind].push_back(&*it);
    }

    // sort per reference position
    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
    {
        std::stable_sort(ports[(portKind)kind].begin(), ports[(portKind)kind].end(), [](Reference* a, Reference* b){
            if (std::fabs(a->x - b->x) <= std::numeric_limits<double>::epsilon())
                return a->y > b->y;
            return a->x < b->x;
        });
    }

    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
    {
        object_properties_t p = property_from_port(kind);

        // compute the port number
        if (!ports[(portKind)kind].empty()) // layer or Component have outter ports
        {
            _vecIDShared.clear();
            _vecIDShared.reserve(ports[(portKind)kind].size());
            for (const Reference* r : ports[(portKind)kind])
            {
                _vecIDShared.push_back(r->port->id());
            }
            controller.setObjectProperty(parent, p, _vecIDShared);
        }
    }
}

int SSPResource::processElement(xmlTextReaderPtr reader, const xmlChar* nsURI)
{
    const xmlChar* name = xmlTextReaderConstLocalName(reader);

    auto logger = get_or_allocate_logger();
    logger->log(LOG_DEBUG, [&](char* first, char* last) {
        to_chars_t io(first, last);
        io = io + "processed depth is ";
        if (processed.size() > 0)
        {
            model::BaseObject* o = processed[0];
            io = io + "( " + logger->id(o) + " , " + o->kind() + " )";
        }
        for (int i = 1; i < processed.size(); i++)
        {
            model::BaseObject* o = processed[i];
            io = io + ", ( " + logger->id(o) + " , " + o->kind() + " )";
        }
        return io + "\n";
    });

    // lookup for known node names
    // thanks to the string intern-ing, the pointer comparison could be used
    auto found = std::find(readerConstInterned.begin(), readerConstInterned.end(), name);
    if (found == readerConstInterned.end())
    {
        sciprint("Unknown \"%s\" element name\n", name);
        return -1;
    }
    enum xcosNames current = static_cast<enum xcosNames>(std::distance(readerConstInterned.begin(), found));
    if (nsURI == xmlnsXCOS)
    {
        if (processed.empty())
        {
            sciprint("Unexpected XCOS element without processed context\n");
            return -1;
        }
        model::BaseObject* component = processed.back();

        switch (current)
        {
            case e_color:
                return loadComponentObjectProperty(reader, component, COLOR, e_color, _vecIntShared);
            case e_context:
                return loadComponentObjectProperty(reader, component, DIAGRAM_CONTEXT, e_context, _vecStrShared);
            case e_control_points:
                return loadComponentObjectProperty(reader, component, CONTROL_POINTS, e_control_points, _vecDblShared);
            case e_datatype:
                return loadComponentObjectProperty(reader, component, DATATYPE, e_datatype, _vecIntShared);
            case e_debug_level:
                _vecIntShared.resize(1);
                return loadComponentObjectProperty(reader, component, DEBUG_LEVEL, e_debug_level, _vecIntShared[0]);
            case e_description:
                return loadComponentObjectProperty(reader, component, DESCRIPTION, e_description, _strShared);
            case e_dstate:
                return loadComponentObjectProperty(reader, component, DSTATE, e_dstate, _vecDblShared);
            case e_equations:
                return loadComponentObjectProperty(reader, component, EQUATIONS, e_equations, _vecDblShared);
            case e_exprs:
                return loadComponentObjectProperty(reader, component, EXPRS, e_exprs, _vecDblShared);
            case e_firing:
                _vecDblShared.resize(1);
                return loadComponentObjectProperty(reader, component, FIRING, e_firing, _vecDblShared[0]);
            case e_font:
                return loadComponentObjectProperty(reader, component, FONT, e_font, _strShared);
            case e_font_size:
                return loadComponentObjectProperty(reader, component, FONT_SIZE, e_font_size, _strShared);
            case e_geometry:
                return loadGeometry(reader, processed.back());
            case e_implicit:
            {
                bool implicit = false;
                return loadComponentObjectProperty(reader, component, IMPLICIT, e_implicit, implicit);
            }
            case e_interface_function:
                return loadComponentObjectProperty(reader, component, INTERFACE_FUNCTION, e_interface_function, _strShared);
            case e_ipar:
                // ipar can be used to set I/O block index ; store it and assign the I/O block with it
                if (component->kind() == PORT)
                {
                    return loadValue(reader, e_ipar, _vecIntShared, [&]() {
                        if (_vecIntShared.empty())
                        {
                            return -1;
                        }
                        references.back().back().index = _vecIntShared[0];
                        return 1;
                    });
                }
                else
                {
                    return loadComponentObjectProperty(reader, component, IPAR, e_ipar, _vecIntShared);
                }
            case e_kind:
                _vecIntShared.resize(1);
                switch (component->kind())
                {
                    case PORT:
                        return loadComponentObjectProperty(reader, component, PORT_KIND, e_kind, _vecIntShared[0]);
                    case LINK:
                        return loadComponentObjectProperty(reader, component, KIND, e_kind, _vecIntShared[0]);
                    default:
                        return -1;
                }
                break;
            case e_label:
            {
                model::BaseObject* o = controller.createBaseObject(ANNOTATION);
                if (component->kind() == PORT)
                {
                    if (controller.setObjectProperty(references.back().back().block, LABEL, o->id()) == FAIL)
                    {
                        return -1;
                    }
                }
                else
                {
                    if (controller.setObjectProperty(component, LABEL, o->id()) == FAIL)
                    {
                        return -1;
                    }
                }
                // TODO store it for decoding its values later, maybe on processed.back()
                processed_push(reader, o);
                return 1;
            }
            case e_name:
                return loadComponentObjectProperty(reader, component, NAME, e_name, _strShared);
            case e_nmode:
                return loadComponentObjectProperty(reader, component, NMODE, e_nmode, _vecIntShared);
            case e_nzcross:
                return loadComponentObjectProperty(reader, component, NZCROSS, e_nzcross, _vecIntShared);
            case e_odstate:
                return loadComponentObjectProperty(reader, component, ODSTATE, e_odstate, _vecDblShared);
            case e_opar:
                return loadComponentObjectProperty(reader, component, OPAR, e_opar, _vecDblShared);
            case e_path:
                return loadComponentObjectProperty(reader, component, PATH, e_path, _strShared);
            case e_properties:
                return loadComponentObjectProperty(reader, component, PROPERTIES, e_properties, _vecDblShared);
            case e_rpar:
                return loadComponentObjectProperty(reader, component, RPAR, e_rpar, _vecDblShared);
            case e_sim_blocktype:
                return loadComponentObjectProperty(reader, component, SIM_BLOCKTYPE, e_sim_blocktype, _strShared);
            case e_sim_dep_ut:
                return loadComponentObjectProperty(reader, component, SIM_DEP_UT, e_sim_dep_ut, _vecIntShared);
            case e_sim_function_api:
                _vecIntShared.resize(1);
                return loadComponentObjectProperty(reader, component, SIM_FUNCTION_API, e_sim_function_api, _vecIntShared[0]);
            case e_sim_function_name:
                return loadComponentObjectProperty(reader, component, SIM_FUNCTION_NAME, e_sim_function_name, _strShared);
            case e_state:
                return loadComponentObjectProperty(reader, component, STATE, e_state, _vecDblShared);
            case e_style:
                if (component->kind() == PORT)
                {
                    return loadComponentObjectProperty(reader, references.back().back().block, STYLE, e_style, _strShared);
                }
                else
                {
                    return loadComponentObjectProperty(reader, component, STYLE, e_style, _strShared);
                }
            case e_thick:
                return loadComponentObjectProperty(reader, component, THICK, e_thick, _vecDblShared);
            case e_uid:
                return loadComponentObjectProperty(reader, component, UID, e_uid, _strShared);
            case e_version:
                return loadComponentObjectProperty(reader, component, VERSION_NUMBER, e_version, _strShared);

            default:
                sciprint("Unknown \"%s\" element name on namespace %s\n", name, nsURI);
                return -1;
        }
    }
    else if (nsURI == xmlnsSSC)
    {
        switch (current)
        {
            case e_Real:
                return loadReal(reader, processed.back(), current);
            case e_Float64:
                return loadReal(reader, processed.back(), current);
            case e_Float32:
                return loadReal(reader, processed.back(), current);
            case e_Integer:
                return loadInteger(reader, processed.back(), current);
            case e_Int8:
                return loadInteger(reader, processed.back(), current);
            case e_UInt8:
                return loadInteger(reader, processed.back(), current);
            case e_Int16:
                return loadInteger(reader, processed.back(), current);
            case e_UInt16:
                return loadInteger(reader, processed.back(), current);
            case e_Int32:
                return loadInteger(reader, processed.back(), current);
            case e_UInt32:
                return loadInteger(reader, processed.back(), current);
            case e_Int64:
                return loadInteger(reader, processed.back(), current);
            case e_UInt64:
                return loadInteger(reader, processed.back(), current);
            case e_Boolean:
                return loadBoolean(reader, processed.back());
            case e_String:
                return loadString(reader, processed.back());
            case e_Enumeration:
                return loadEnumeration(reader, processed.back());
            case e_Binary:
                return loadBinary(reader, processed.back());
            case e_Dimension:
                return loadDimension(reader, processed.back());
            case e_Clock:
                return loadClock(reader, processed.back());
            case e_Unit:
                processed_push(reader);
                return loadUnit(reader, processed.back());
            case e_BaseUnit:
                return loadBaseUnit(reader, processed.back());
            case e_Annotation:
                processed_push(reader);
                return loadAnnotation(reader, processed.back());
            default:
                sciprint("Unknown \"%s\" element name on namespace %s\n", name, nsURI);
                return -1;
        }
    }
    else if (nsURI == xmlnsSSB)
    {
        switch (current)
        {
            default:
                sciprint("Unknown \"%s\" element name on namespace %s\n", name, nsURI);
                return -1;
        }
    }
    else if (nsURI == xmlnsSSD)
    {
        switch (current)
        {
            case e_SystemStructureDescription:
            {
                // the root diagram should be decoded
                model::BaseObject* o = controller.getBaseObject(root);
                // reset the processed stack to the root diagram only
                processed.assign(1, o);

                return loadSystemStructureDescription(reader, o);
            }
            case e_DefaultExperiment:
            {
                return loadDefaultExperiment(reader, processed.back());
            }
            case e_System:
            {
                if (processed.size() == 1)
                {
                    // this is the main diagram, resolve it
                    processed_push(reader);
                }
                else
                {
                    // this is a child of a diagram, create it
                    model::BaseObject* parent = processed.back();
                    model::BaseObject* o = controller.createBaseObject(BLOCK);
                    processed_push(reader, o);

                    // assign the child
                    controller.setObjectProperty(o, PARENT_DIAGRAM, root);
                    if (parent->kind() == BLOCK)
                    {
                        controller.setObjectProperty(o, PARENT_BLOCK, parent->id());
                    }
                    controller.getObjectProperty(parent, CHILDREN, _vecIDShared);
                    _vecIDShared.push_back(o->id());
                    controller.setObjectProperty(parent, CHILDREN, _vecIDShared);

                    controller.setObjectProperty(o, INTERFACE_FUNCTION, std::string("SUPER_f"));
                }
                references.emplace_back();
                bounds.emplace_back();
                return loadSystem(reader, processed.back());
            }
            case e_Connectors:
                processed_push(reader);
                break;
            case e_Connector:
                return loadConnector(reader, processed.back());
            case e_SystemGeometry:
                // SystemGeometry is used to relocate connectors with absolute coordinates
                return loadSystemGeometry(reader, processed.back());
            case e_ElementGeometry:
                return loadElementGeometry(reader, processed.back());
            case e_ConnectorGeometry:
                return loadConnectorGeometry(reader, processed.back());
            case e_ConnectionGeometry:
                // geometry is used for rectangle coordinates of its parent
                return loadConnectionGeometry(reader, processed.back());
            case e_Connections:
                processed_push(reader);
                break;
            case e_Connection:
                return loadConnection(reader, processed.back());
            case e_ParameterBindings:
                processed_push(reader);
                break;
            case e_ParameterBinding:
                processed_push(reader);
                break;
            case e_ParameterValues:
                processed_push(reader);
                break;
            case e_Elements:
                processed_push(reader);
                break;
            case e_Component:
            {
                model::BaseObject* o = controller.createBaseObject(BLOCK);
                model::BaseObject* parent = processed.back();
                processed_push(reader, o);

                controller.setObjectProperty(o, PARENT_DIAGRAM, root);
                if (parent->kind() == BLOCK)
                {
                    controller.setObjectProperty(o, PARENT_BLOCK, parent->id());
                }
                controller.getObjectProperty(parent, CHILDREN, _vecIDShared);
                _vecIDShared.push_back(o->id());
                controller.setObjectProperty(parent, CHILDREN, _vecIDShared);

                references.emplace_back();
                return loadComponent(reader, o);
            }
            case e_GraphicalElements:
                processed_push(reader);
                break;
            case e_Note:
            {
                model::BaseObject* o = controller.createBaseObject(ANNOTATION);
                model::BaseObject* parent = processed.back();
                processed_push(reader, o);

                controller.setObjectProperty(o, PARENT_DIAGRAM, root);
                if (parent->kind() == BLOCK)
                {
                    controller.setObjectProperty(o, PARENT_BLOCK, parent->id());
                }
                controller.getObjectProperty(parent, CHILDREN, _vecIDShared);
                _vecIDShared.push_back(o->id());
                controller.setObjectProperty(parent, CHILDREN, _vecIDShared);

                return loadNote(reader, o);
            }
            case e_Annotations:
                processed_push(reader);
                break;
            case e_Units:
                processed_push(reader);
                break;

            default:
                sciprint("Unknown \"%s\" element name on namespace %s\n", name, nsURI);
                return -1;
        }
    }
    else if (nsURI == xmlnsSSV)
    {
        switch (current)
        {
            case e_ParameterSet:
                processed_push(reader);
                return loadParameterSet(reader, processed.back());
            case e_Parameters:
                processed_push(reader);
                break;
            case e_Parameter:
                processed_push(reader);
                return loadParameter(reader, processed.back());
            case e_Units: // ssv:Units is in the demo file, in the 2.0 spec should be ssc:Units
                processed_push(reader);
                break;
            case e_Real:
                return loadReal(reader, processed.back(), current);
            case e_Float64:
                return loadReal(reader, processed.back(), current);
            case e_Float32:
                return loadReal(reader, processed.back(), current);
            case e_Integer:
                return loadInteger(reader, processed.back(), current);
            case e_Int8:
                return loadInteger(reader, processed.back(), current);
            case e_UInt8:
                return loadInteger(reader, processed.back(), current);
            case e_Int16:
                return loadInteger(reader, processed.back(), current);
            case e_UInt16:
                return loadInteger(reader, processed.back(), current);
            case e_Int32:
                return loadInteger(reader, processed.back(), current);
            case e_UInt32:
                return loadInteger(reader, processed.back(), current);
            case e_Int64:
                return loadInteger(reader, processed.back(), current);
            case e_UInt64:
                return loadInteger(reader, processed.back(), current);
            case e_Boolean:
                return loadBoolean(reader, processed.back());
            case e_String:
                return loadString(reader, processed.back());
            case e_Enumeration:
                return loadEnumeration(reader, processed.back());
            case e_Binary:
                return loadBinary(reader, processed.back());
            default:
                sciprint("Unknown \"%s\" element name on namespace %s\n", name, nsURI);
                return -1;
        }
    }
    else if (nsURI == xmlnsSSM)
    {
        switch (current)
        {
            default:
                sciprint("Unknown \"%s\" element name on namespace %s\n", name, nsURI);
                return -1;
        }
    }

    return 1;
}

int SSPResource::processText(xmlTextReaderPtr reader)
{
    int ret;
    const xmlChar* name = xmlTextReaderConstLocalName(reader);
    const xmlChar* value = xmlTextReaderValue(reader);

    sciprint("Unable to decode text value %s at node %s.\n", value, name);
    ret = -1;

    return ret;
}

int SSPResource::processEndElement(xmlTextReaderPtr reader)
{
    const xmlChar* xmlns = xmlTextReaderConstNamespaceUri(reader);
    const xmlChar* name = xmlTextReaderConstLocalName(reader);
    
    // Check if processed is empty - this can happen with unknown namespaces
    if (processed.empty())
    {
        return 1; // skip processing
    }
    
    model::BaseObject* o = processed.back();

    // verbose for debugging
    auto logger = get_or_allocate_logger();
    if (logger->getLevel() <= LOG_DEBUG)
    {
        int line = xmlGetLineNo(xmlTextReaderCurrentNode(reader));
        logger->log(LOG_DEBUG, "end element %s line %d\n", name, line);
    }

    if (annotated && xmlns == xmlnsSSC && name == readerConstInterned[e_Annotation])
    {
        annotated = false;
    }
    else if (xmlns == xmlnsSSD && name == readerConstInterned[e_System])
    {
        // on System (eg. layer) ending
        assignIOBlockChildren(o, true);

        // update geometries
        updateSystem(o);

        // reorder and assign I/O Blocks indexes
        assignInnerPortIndexes(o);

        // clear layer stored values
        references.pop_back();
        bounds.pop_back();

        // assign outter port indexes
        if (o->kind() == BLOCK)
        {
            assignOutterPortIndexes(o);
        }
    }
    else if (xmlns == xmlnsSSD && name == readerConstInterned[e_Component])
    {
        // on Component ending

        // if the CHILDREN are used, setup I/O block
        if(assignIOBlockChildren(o, false) > 0)
        {
            // reorder and assign I/O Blocks indexes
            assignInnerPortIndexes(o);
        }

        // remove temporaries
        references.pop_back();

        // assign port indexes
        assignOutterPortIndexes(o);
    }
    else if (xmlns == xmlnsSSD && name == readerConstInterned[e_Connector])
    {
        bool isMainDiagram = references.size() == 1;

        auto& r_layer = references.back();
        model::BaseObject* innerPort = r_layer.back().port;
        model::BaseObject* innerBlock = r_layer.back().block;
        model::BaseObject* outterPort = o;

        // set inner block's port kind
        portKind innerKind = opposite_port(r_layer.back().kind);
        std::vector<ScicosID> ports = {innerPort->id()};
        controller.setObjectProperty(innerBlock, property_from_port(innerKind), ports);
        
        copy_property(outterPort, innerBlock, NAME, _strShared);
        copy_property(outterPort, innerBlock, DESCRIPTION, _strShared);

        copy_property(outterPort, innerPort, DATATYPE, _vecIntShared);
        bool isImplicit;
        copy_property(outterPort, innerPort, IMPLICIT, isImplicit);

        controller.setObjectProperty(innerBlock, INTERFACE_FUNCTION, interface_function(innerKind, isImplicit, isMainDiagram));
        controller.setObjectProperty(innerBlock, SIM_FUNCTION_NAME, simulation_function(innerKind, isImplicit, isMainDiagram));
        controller.setObjectProperty(innerBlock, STYLE, interface_function(innerKind, isImplicit, isMainDiagram));

        // refresh the outter connector to connect it later
        if (!isMainDiagram)
        {
            auto& r_parent_layer = *(references.rbegin() + 1);
            r_parent_layer.back().connector = r_layer.back().connector;
            r_parent_layer.back().kind = r_layer.back().kind;
            r_parent_layer.back().x = r_layer.back().x;
            r_parent_layer.back().y = r_layer.back().y;
        }
    }

    // Only pop if this is a namespace we actually processed
    bool shouldPop = (xmlns == xmlnsXCOS || xmlns == xmlnsSSC || xmlns == xmlnsSSB || 
                     xmlns == xmlnsSSD || xmlns == xmlnsSSV || xmlns == xmlnsSSM || xmlns == nullptr);
    if (shouldPop && !processed.empty())
    {
        processed.pop_back();
    }
    return 1;
}

} // namespace org_scilab_modules_scicos
