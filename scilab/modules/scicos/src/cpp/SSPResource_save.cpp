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

#include "SSPResource.hxx"
#include "controller_helpers.hxx"
#include "expandPathVariable.h"
#include "scicos_base64.hxx"
#include "utilities.hxx"

// units are stored in the model without being mapped on the Controller yet
#include "LoggerView.hxx"
#include "Model.hxx"
#include "model/Block.hxx"
#include "model/Diagram.hxx"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

extern "C"
{
#include "sci_types.h"

#include <archive.h>
#include <archive_entry.h>

#include <libxml/xmlwriter.h>

#include "FileExist.h"
#include "Sciwarning.h"
#include "deleteafile.h"
#include "getFullFilename.h"
#include "sci_malloc.h"
#include "sci_types.h"
#include "sciprint.h"
#include "version.h"
}

// on Windows, these macros are defined
#undef min
#undef max

namespace org_scilab_modules_scicos
{

namespace
{
/**
 * Helper class to allocate and clean the archive and libxml2 state
 */
struct State
{
    const char* _uri;
    struct archive* _ar;

    struct archive_entry* _sharedEntry;
    std::vector<std::string> _fmupath;

    State(const char* uri) : _uri(uri), _ar(archive_write_new()), _sharedEntry(archive_entry_new()), _fmupath() {};

    ~State()
    {
        archive_entry_free(_sharedEntry);
        archive_read_free(_ar); // will call archive_read_close()
    };

    // output zip file
    struct archive* output() { return _ar; };
    // shared entry, will be reused
    struct archive_entry* entry() { return _sharedEntry; };
    // add an FMU to be stored
    void insert_fmu(std::string path) { _fmupath.push_back(path); };
};

// libxml2 write callback function
int iowrite(void* context, const char* buffer, int len)
{
    State* st = static_cast<State*>(context);

    la_ssize_t archive_error_code = archive_write_data(st->output(), buffer, len);
    if (archive_error_code > std::numeric_limits<int>::max())
    {
        return std::numeric_limits<int>::max();
    }
    if (archive_error_code < std::numeric_limits<int>::min())
    {
        return std::numeric_limits<int>::min();
    }
    if (archive_error_code < 0)
    {
        Sciwarning("libarchive reported errno #%d: %s", archive_errno(st->output()), archive_error_string(st->output()));
    }

    return (int)archive_error_code;
};

// libxml2 close callback function
int ioclose(void* context)
{
    // archive_write_close() will be called at the end, so nothing to do here
    return 0;
};

using all_port_t = SSPResource::ChildrenCategories::all_port_t;

static inline void unique(std::vector<all_port_t>& all_ports)
{
    std::stable_sort(all_ports.begin(), all_ports.end(), [](const all_port_t& a, const all_port_t& b)
                        { return a.name < b.name; });

    // Ensure port names are unique, number it like foo1, foo2, foo3, etc..
    // This will ensure multiple unnamed input, with generated "in" label will have increased indexes
    auto it = all_ports.begin();
    auto next = it != all_ports.end() ? it + 1 : it;
    for (; next != all_ports.end(); ++it, ++next)
    {
        if (it->name != "" && it->name == next->name)
        {
            std::string shared_name = it->name;
            int counter = 1;

            it->name = shared_name + std::to_string(counter++);
            it->generated = true;
            for (; next != all_ports.end() && shared_name == next->name; next++, counter++)
            {
                next->name = shared_name + std::to_string(counter);
                next->generated = true;
            }

            it = next - 1;
            if (next == all_ports.end())
            {
                break;
            }
        }
    }

    // sort back to keep original id() order
    std::sort(all_ports.begin(), all_ports.end(), [](const all_port_t& a, const all_port_t& b)
    {
        if (a.inner_port != nullptr && b.inner_port != nullptr)
        {
            return a.inner_port->id() < b.inner_port->id();
        }
        if (a.outter_port != nullptr && b.outter_port != nullptr)
        {
            return a.outter_port->id() < b.outter_port->id();
        }
        return a.block_index < b.block_index;
    });
}

template<typename T>
inline std::string to_string(T v)
{
    // default implementation is provided by std::to_string
    return std::to_string(v);
}

template<>
inline std::string to_string(bool v)
{
    if (v)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

template<>
inline std::string to_string(double v)
{
    if (std::trunc(v) == v)
    {
        return to_string((int)v);
    }

    std::string str(15, '\0');
    // std::snprintf(const_cast<char*>(str.data()), str.size(), "%.6E", v);
    int sz = std::sprintf(const_cast<char*>(str.data()), "%.6E", v);
    if (sz > 0)
    {
        str.resize(sz);
    }
    return str;
}

}; /* anonymous namespace*/

std::string SSPResource::Result::report()
{
    std::stringstream ss;
    if (object != nullptr)
    {
        ss << " at object " << object->id() << " kind " << object->kind();
    }
    if (property != MAX_OBJECT_PROPERTIES)
    {
        ss << " for property " << property;
    }

    if (code == 0)
    {
        ss << " : ok\n";
    }
    else
    {
        ss << " : error " << code << "\n";
    }
    return ss.str();
}

SSPResource::Result SSPResource::save(const char* uri)
{
    Result status = Result::Ok();
    State st(uri);
    context = &st;

    status = Result::FromArchive(archive_write_set_format_zip(st.output()));
    if (status.error())
    {
        return status;
    }
    status = Result::FromArchive(archive_write_set_options(st.output(), "compression=deflate,compression-level=9"));
    if (status.error())
    {
        return status;
    }
    status = Result::FromArchive(archive_write_open_filename(st.output(), uri));
    if (status.error())
    {
        return status;
    }

    // write the SystemStructure.ssd entry and data
    archive_entry_set_pathname(st.entry(), "SystemStructure.ssd");
    archive_entry_unset_size(st.entry());
    archive_entry_set_filetype(st.entry(), AE_IFREG);
    archive_entry_set_perm(st.entry(), 0644);

    if (status.ok())
    {
        status = Result::FromArchive(archive_write_header(st.output(), st.entry()));
    }
    if (status.ok())
    {
        status = writeSystemStructureFile(&st);
    }

    // write the resources/ subdirectory entries
    if (status.ok())
    {
        status = Result::FromArchive(archive_write_zip_set_compression_store(st.output()));
    }

    std::vector<char> buff(8192);
    for (std::string f : st._fmupath)
    {
        if (status.error())
        {
            break;
        }

        // copy FMU entry
        archive_entry_set_pathname(st.entry(), ("resources/" + f).c_str());
        archive_entry_unset_size(st.entry());
        archive_entry_set_filetype(st.entry(), AE_IFREG);
        archive_entry_set_perm(st.entry(), 0644);

        status = Result::FromArchive(archive_write_header(st.output(), st.entry()));
        if (status.error())
        {
            break;
        }

        // resolve FMU fullpath, use FMU_WRAPPER.sci as reference
        std::string filepath = f;
        if (!FileExist(filepath.c_str()))
        {
            char* tmp = getFullFilename(("resources/" + f).c_str());
            if (tmp != nullptr)
            {
                filepath = tmp;
                FREE(tmp);
            }
        }
        if (!FileExist(filepath.c_str()))
        {
            char* tmp = expandPathVariable(("TMPDIR/resources/" + f).c_str());
            if (tmp != nullptr)
            {
                filepath = tmp;
                FREE(tmp);
            }
        }
        if (!FileExist(filepath.c_str()))
        {
            // fmigetPath()
            char* tmp = expandPathVariable(("SCI/modules/fmu/" + f).c_str());
            if (tmp != nullptr)
            {
                filepath = tmp;
                FREE(tmp);
            }
        }
        if (!FileExist(filepath.c_str()))
        {
            Sciwarning("FMU file %s not found", f.c_str());
            status = Result::Error();
        }

        // copy FMU data
        std::ifstream inputFile(filepath, std::ios::in | std::ios::binary);
        while (inputFile)
        {
            inputFile.read(buff.data(), buff.size());
            std::streamsize bytesRead = inputFile.gcount();
            if (bytesRead > 0)
            {
                la_ssize_t archive_error_code = archive_write_data(st.output(), buff.data(), bytesRead);
                if (archive_error_code < 0)
                {
                    Sciwarning("libarchive reported errno #%d: %s", archive_errno(st.output()), archive_error_string(st.output()));
                    status = Result::Error();
                    break;
                }
                if (archive_error_code != bytesRead)
                {
                    Sciwarning("libarchive reported partial write, expected %d, got %d", bytesRead, archive_error_code);
                    status = Result::Error();
                    break;
                }
            }
        }
        inputFile.close();
    }

    // ensure file does not exist on error
    if (status.error() && FileExist(uri))
    {
        deleteafile(uri);
    }

    return status;
}

static bool is_empty_matrix(const std::vector<double>& v)
{
    // v == {1, 2, 0, 0, 0}
    return v.size() == 5 && static_cast<int>(v[0]) == sci_matrix && v[1] == 2. && v[2] == 0. && v[3] == 0. && v[4] == 0.;
}

static bool is_empty_list(const std::vector<double>& v)
{
    // v == {15, 0}
    return v.size() == 2 && static_cast<int>(v[0]) == sci_list && v[1] == 0.;
}

static bool is_string_vector(const std::vector<double>& v)
{
    return v.size() > 2 && static_cast<int>(v[0]) == sci_strings && v[1] != 0;
}

/* helper function to decode simple string EXPRS */
static std::vector<std::string> to_string_vector(const std::vector<double>& v)
{
    std::vector<std::string> ret;
    std::vector<double>::const_iterator it = v.begin();

    int strHeader = static_cast<int>(*it++);
    if (strHeader != sci_strings)
    {
        return ret;
    }
    unsigned int iDims = static_cast<unsigned int>(*it++);

    // manage multi-dimensional arrays (will be serialized as a vector)
    unsigned int iElements = 1;
    for (unsigned int i = 0; i < iDims; ++i)
    {
        iElements *= static_cast<unsigned int>(*it++);
    }

    // retrieve the length of each encoded string, stored as a stack
    std::vector<unsigned int> stringsLength;
    stringsLength.reserve(iElements + 1);
    stringsLength.push_back(0);
    for (unsigned int i = 0; i < iElements; ++i)
    {
        stringsLength.push_back(static_cast<unsigned int>(*it++));
    }

    // Retrieving the pointers (already UTF-8 encoded char*) and store them as strings
    ret.reserve(ret.size() + iElements);
    for (unsigned int i = 0; i < iElements; ++i)
    {
        // push the data
        const double* strData = &(*(it + stringsLength[i]));
        ret.emplace_back((char*)strData);
    }

    return ret;
}

SSPResource::Result SSPResource::writeSystemStructureFile(void* context)
{
    Result status = Result::Ok();

    xmlOutputBufferPtr buffer = xmlOutputBufferCreateIO(iowrite, ioclose, context, NULL);
    if (buffer == NULL)
    {
        return Result::Error();
    }

    xmlTextWriterPtr writer = xmlNewTextWriter(buffer);
    if (writer == NULL)
    {
        return Result::Error();
    }

    status = Result::FromXML(xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL));
    if (status.error())
    {
        xmlFreeTextWriter(writer);
        return Result::Error();
    }

    status = Result::FromXML(xmlTextWriterSetIndent(writer, 4));
    if (status.error())
    {
        xmlFreeTextWriter(writer);
        return Result::Error();
    }

    std::string versionComment = " Generated by " + std::string(SCI_VERSION_STRING) + " " + std::string(SCI_VERSION_REVISION) + " " + std::to_string(SCI_VERSION_TIMESTAMP) + " ";
    status = Result::FromXML(xmlTextWriterWriteComment(writer, BAD_CAST(versionComment.c_str())));
    if (status.error())
    {
        xmlFreeTextWriter(writer);
        return Result::Error();
    }

    /* TODO: will reduce file size
    // allocate default object values
    defaultValues[BLOCK] = controller.createBaseObject(BLOCK);
    defaultValues[DIAGRAM] = controller.createBaseObject(DIAGRAM);
    defaultValues[LINK] = controller.createBaseObject(LINK);
    defaultValues[ANNOTATION] = controller.createBaseObject(ANNOTATION);
    defaultValues[PORT] = controller.createBaseObject(PORT);
    */

    status = writeSystemStructureDescription(writer);
    if (status.error())
    {
        xmlFreeTextWriter(writer);
        return status;
    }

    /* TODO: will reduce file size
    controller.deleteBaseObject(defaultValues[BLOCK]);
    controller.deleteBaseObject(defaultValues[DIAGRAM]);
    controller.deleteBaseObject(defaultValues[LINK]);
    controller.deleteBaseObject(defaultValues[ANNOTATION]);
    controller.deleteBaseObject(defaultValues[PORT]);
    defaultValues = {0};
    */

    status = Result::FromXML(xmlTextWriterEndDocument(writer));
    if (status.error())
    {
        xmlFreeTextWriter(writer);
        return status;
    }

    xmlFreeTextWriter(writer);
    return Result::Ok();
}

SSPResource::Result SSPResource::ChildrenCategories::decode_ipar_or_exprs(Controller& controller, model::BaseObject* o, int& index)
{
    if (!controller.getObjectProperty(o, IPAR, _vecIntShared))
    {
        return Result::Error(o, IPAR);
    }

    if (!controller.getObjectProperty(o, IPAR, _vecIntShared))
    {
        return Result::Error(o, IPAR);
    }
    if (_vecIntShared.size() < 1)
    {
        // block might not be updated, decode exprs if needed
        if (!controller.getObjectProperty(o, EXPRS, _vecStrShared))
        {
            return Result::Error(o, EXPRS);
        }
        if (_vecStrShared.size() < 1)
        {
            return Result::Error(o, EXPRS);
        }

        char* end = nullptr;
        index = std::strtol(_vecStrShared[0].c_str(), &end, 10);
        if (index <= 0)
        {
            return Result::Error(o, EXPRS);
        }
    }
    else
    {
        index = _vecIntShared[0];
    }
    return Result::Ok();
}

SSPResource::Result SSPResource::ChildrenCategories::store_port(Controller& controller, model::BaseObject* o, object_properties_t p, size_t index, std::string& name, const std::string& prefix)
{
    SSPResource::Result status = Result::Ok();
    std::vector<ScicosID> ports = {ScicosID()};

    int port_index;
    status = decode_ipar_or_exprs(controller, o, port_index);
    if (status.error())
    {
        return status;
    }

    model::BaseObject* inner_port;
    if (!controller.getObjectProperty(o, opposite_property(p), ports))
    {
        return Result::Error(o, opposite_property(p));
    }
    if (ports.size() < 1)
    {
        return Result::Error(o, opposite_property(p));
    }
    inner_port = controller.getBaseObject(ports[0]);
    if (inner_port == nullptr)
    {
        return Result::Error(o, opposite_property(p));
    }

    bool generated = false;
    if (name == "")
    {
        name = prefix;
        generated = true;
    }

    enum portKind port_k = (enum portKind) port_from_property(p);
    all_ports.push_back({port_k, port_index, generated, inner_port, nullptr, o, index + 1, name});
    max_indexes[port_k] = std::max(max_indexes[port_k], port_index + 1);

    return Result::Ok();
}

SSPResource::Result SSPResource::ChildrenCategories::assign_outter_ports(Controller& controller, model::BaseObject* parent)
{
    if (parent->kind() != BLOCK)
    {
        // only blocks have outter ports
        return Result::Ok();
    }

    // categorize all_ports per kind
    std::map<enum portKind, std::vector<size_t>> all_ports_idx = {
        {PORT_IN, {}},
        {PORT_OUT, {}},
        {PORT_EIN, {}},
        {PORT_EOUT, {}}
    };
    for (size_t i = 0; i < all_ports.size(); i++)
    {
        all_ports_idx[all_ports[i].kind].push_back(i);
    }

    // generated name prefixes, indexed by enum portKind
    const std::string prefixes[] = {"", "#in", "#out", "#clkin", "#clkout"};

    // outter ports from parent
    std::vector<ScicosID> outter_ports_ids;
    for (auto& pair : all_ports_idx)
    {
        if (!controller.getObjectProperty(parent, property_from_port(pair.first), outter_ports_ids))
        {
            return Result::Error(parent, property_from_port(pair.first));
        }

        // assign by port_index, keeping gaps ; ignore extra or duplicated inner blocks
        std::vector<bool> assigned(outter_ports_ids.size(), false);
        for (size_t idx : pair.second)
        {
            int i = all_ports[idx].port_index - 1;
            if (i < 0 || i >= (int) outter_ports_ids.size() || assigned[i])
            {
                continue;
            }
            assigned[i] = true;
            all_ports[idx].outter_port = controller.getBaseObject(outter_ports_ids[i]);
            if (all_ports[idx].outter_port == nullptr)
            {
                return Result::Error(parent, property_from_port(pair.first));
            }
            names.insert({all_ports[idx].outter_port, all_ports[idx].name});
        }

        // outter ports without inner block are still connectors, with a generated name
        for (size_t i = 0; i < outter_ports_ids.size(); i++)
        {
            if (assigned[i])
            {
                continue;
            }

            int counter = (int) i + 1;
            std::string name;
            do
            {
                name = prefixes[pair.first] + std::to_string(counter++);
            }
            while (std::any_of(all_ports.begin(), all_ports.end(), [&name](const all_port_t& p) { return p.name == name; }));

            model::BaseObject* outter_port = controller.getBaseObject(outter_ports_ids[i]);
            if (outter_port == nullptr)
            {
                return Result::Error(parent, property_from_port(pair.first));
            }
            all_ports.push_back({pair.first, (int) i + 1, true, nullptr, outter_port, nullptr, 0, name});
            names.insert({outter_port, name});
            used_names.insert(name);
        }
        max_indexes[pair.first] = std::max(max_indexes[pair.first], (int) outter_ports_ids.size() + 1);
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::ChildrenCategories::load_children(Controller& controller, model::BaseObject* parent, const std::vector<ScicosID>& children)
{
    Result status = Result::Ok();

    std::string name;
    std::string interfaceFunction;
    std::vector<ScicosID> ports;

    canvas = SystemCanvas::default_xcos_ccordinates();

    for (size_t i = 0; i < children.size(); i++)
    {
        model::BaseObject* o = controller.getBaseObject(children[i]);
        if (o == nullptr)
        {
            return Result::Error();
        }

        switch (o->kind())
        {
            case BLOCK:
            {
                // store the name
                if (!controller.getObjectProperty(o, NAME, name))
                {
                    return Result::Error(o, NAME);
                }

                if (!controller.getObjectProperty(o, INTERFACE_FUNCTION, interfaceFunction))
                {
                    return Result::Error(o, INTERFACE_FUNCTION);
                }

                else if ((parent->kind() == BLOCK) && (interfaceFunction == "IN_f" || interfaceFunction == "INIMPL_f"))
                {
                    // on Block containing an IN_f, it is treated as a Connector
                    status = store_port(controller, o, INPUTS, (int) i, name, "#in");
                    if (status.error())
                    {
                        return status;
                    }
                }
                else if ((parent->kind() == DIAGRAM) && interfaceFunction == "SSPInputConnector")
                {
                    int index;
                    if (!controller.getObjectProperty(o, EXPRS, _vecStrShared))
                    {
                        return Result::Error(o, EXPRS);
                    }
                    if (_vecStrShared.size() < 1)
                    {
                        return Result::Error(o, EXPRS);
                    }
                    model::BaseObject* port;
                    if (!controller.getObjectProperty(o, OUTPUTS, ports))
                    {
                        return Result::Error(o, OUTPUTS);
                    }
                    if (ports.size() < 1)
                    {
                        return Result::Error(o, OUTPUTS);
                    }
                    port = controller.getBaseObject(ports[0]);
                    if (port == nullptr)
                    {
                        return Result::Error(o, OUTPUTS);
                    }

                    index = max_indexes[PORT_IN]++;
                    all_ports.push_back({PORT_IN, index, false, port, nullptr, o, 0, _vecStrShared[0]});
                }
                else if ((parent->kind() == BLOCK) && (interfaceFunction == "CLKINV_f" || interfaceFunction == "CLKIN_f"))
                {
                    status = store_port(controller, o, EVENT_INPUTS, i, name, "#clkin");
                    if (status.error())
                    {
                        return status;
                    }
                }
                else if ((parent->kind() == BLOCK) && (interfaceFunction == "OUT_f" || interfaceFunction == "OUTIMPL_f"))
                {
                    status = store_port(controller, o, OUTPUTS, i, name, "#out");
                    if (status.error())
                    {
                        return status;
                    }
                }
                else if ((parent->kind() == DIAGRAM) && interfaceFunction == "SSPOutputConnector")
                {
                    int index;
                    if (!controller.getObjectProperty(o, EXPRS, _vecStrShared))
                    {
                        return Result::Error(o, EXPRS);
                    }
                    if (_vecStrShared.size() < 1)
                    {
                        return Result::Error(o, EXPRS);
                    }
                    model::BaseObject* port;
                    if (!controller.getObjectProperty(o, INPUTS, ports))
                    {
                        return Result::Error(o, INPUTS);
                    }
                    if (ports.size() < 1)
                    {
                        return Result::Error(o, INPUTS);
                    }
                    port = controller.getBaseObject(ports[0]);
                    if (port == nullptr)
                    {
                        return Result::Error(o, INPUTS);
                    }

                    index = max_indexes[PORT_OUT]++;
                    all_ports.push_back({PORT_OUT, index, false, port, nullptr, o, 0, _vecStrShared[0]});
                }
                else if ((parent->kind() == BLOCK) && (interfaceFunction == "CLKOUTV_f" || interfaceFunction == "CLKOUT_f"))
                {
                    status = store_port(controller, o, EVENT_OUTPUTS, i, name, "#clkout");
                    if (status.error())
                    {
                        return status;
                    }
                }
                else
                {
                    elements.push_back(o);
                }

                status = canvas.grow_by_xcos_ccordinates(controller, o, _vecDblShared);
                if (status.error())
                {
                    return status;
                }
                break;
            }

            case ANNOTATION:
            {
                graphical_elements.push_back(o);

                status = canvas.grow_by_xcos_ccordinates(controller, o, _vecDblShared);
                if (status.error())
                {
                    return status;
                }
                break;
            }

            case LINK:
            {
                connections.push_back(o);

                status = canvas.grow_by_xcos_ccordinates(controller, o, _vecDblShared);
                if (status.error())
                {
                    return status;
                }
                break;
            }

            default:
                break;
        }
    }

    // generate valid names for un-named connectors
    unique(all_ports);

    // add all ports with a generated name
    for (auto it = all_ports.begin(); it != all_ports.end(); ++it)
    {
        if (it->name == "")
        {
            return Result::Error(it->inner_port, NAME);
        }
        if (it->generated)
        {
            names.insert({it->inner_port, it->name});
            used_names.insert(it->name);
        }
    }

    return Result::Ok();
}
SSPResource::Result SSPResource::ChildrenCategories::load_ports(Controller& controller, model::BaseObject* o)
{
    int index = 0;

    struct port_t
    {
        object_properties_t block_property;
        enum portKind kind;
        std::string prefix;
    };
    port_t map[] = {
        {INPUTS, PORT_IN, "#in"},
        {OUTPUTS, PORT_OUT, "#out"},
        {EVENT_INPUTS, PORT_EIN, "#clkin"},
        {EVENT_OUTPUTS, PORT_EOUT, "#clkout"},
    };

    for (auto m : map)
    {
        if (!controller.getObjectProperty(o, m.block_property, _vecIDShared))
        {
            return Result::Error(o, m.block_property);
        }

        for (ScicosID id : _vecIDShared)
        {
            index++;

            model::BaseObject* port = controller.getBaseObject(id);
            if (!controller.getObjectProperty(port, NAME, _strShared))
            {
                return Result::Error(port, NAME);
            }

            bool generated = false;
            if (_strShared == "")
            {
                _strShared = m.prefix;
                generated = true;
            }

            all_ports.push_back({m.kind, index, generated, nullptr, port, nullptr, 0, _strShared});
        }

        max_indexes[m.kind] = index + 1;
    }

    // generate valid names for un-named connectors
    unique(all_ports);

    // add all ports by name
    for (auto it = all_ports.begin(); it != all_ports.end(); ++it)
    {
        if (it->name == "")
        {
            return Result::Error(it->outter_port, NAME);
        }
        names.insert({it->outter_port, it->name});
    }

    return Result::Ok();
}

std::string SSPResource::ChildrenCategories::get_name(Controller& controller, model::BaseObject* o)
{
    // get a previously stored name
    auto names_it = names.find(o);
    if (names_it != names.end())
    {
        return names_it->second;
    }
    return "";
}

std::string SSPResource::ChildrenCategories::retrieve_name(Controller& controller, model::BaseObject* o)
{
    // retrieve a previously stored name
    auto names_it = names.find(o);
    if (names_it != names.end())
    {
        // name already generated
        return names_it->second;
    }

    // this is a new object, try to use the user-defined name
    if (controller.getObjectProperty(o, NAME, _strShared) && _strShared != "")
    {
        auto used_names_it = used_names.find(_strShared);
        if (used_names_it == used_names.end())
        {
            // name has not been used yet, register as used
            names.insert(names_it, {o, _strShared});
            return *used_names.emplace_hint(used_names_it, _strShared);
        }
    }

    // generate a name for unnamed elements
    // '#' is forbidden in C, also forbidden in Xcos
    // might also be the case on other tools
    _strShared = "#" + std::to_string(++unnamed_counter);
    auto used_names_it = used_names.find(_strShared);
    while (used_names_it != used_names.end())
    {
        _strShared = "#" + std::to_string(++unnamed_counter);
        used_names_it = used_names.find(_strShared);
    }

    // insert for resolving it later
    names.insert(names_it, {o, _strShared});
    return *used_names.emplace_hint(used_names_it, _strShared);
}

void SSPResource::ChildrenCategories::insert_all_names(const ChildrenCategories& blockPorts)
{
    for (const auto& p : blockPorts.all_ports)
    {
        if (p.outter_port != nullptr)
        {
            auto it = blockPorts.names.find(p.outter_port);
            if (it != blockPorts.names.end())
            {
                names.insert({p.outter_port, it->second});
            }
        }
        if (p.inner_port != nullptr && p.outter_port != nullptr)
        {
            auto it = blockPorts.names.find(p.inner_port);
            if (it != blockPorts.names.end())
            {
                names.insert({p.outter_port, it->second});
            }
        }
    }
}

SSPResource::Result SSPResource::writeSystemStructureDescription(xmlTextWriterPtr writer)
{
    Result status = Result::Ok();

    model::BaseObject* o = controller.getBaseObject(root);
    if (o == nullptr)
    {
        return Result::Error();
    }

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_SystemStructureDescription], nullptr));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_version], BAD_CAST("2.0")));
    if (status.error())
    {
        return status;
    }

    if (!controller.getObjectProperty(o, NAME, _strShared))
    {
        return Result::Error();
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_name], BAD_CAST(_strShared.c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_generationTool], BAD_CAST(SCI_VERSION_STRING)));
    if (status.error())
    {
        return status;
    }

    // Do not save the optional generationDateAndTime on purpose, this will avoid some diff on committed file
    /*
    {
        auto now = std::time(nullptr);
        auto tm = *std::gmtime(&now);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_generationDateAndTime], BAD_CAST(oss.str().c_str())));
        if (status.error())
        {
            return status;
        }
    }
    */

    //
    // write all the namespaces
    //
    if (!controller.getObjectProperty(o, GLOBAL_XMLNS, _vecStrShared))
    {
        return Result::Error(o, GLOBAL_XMLNS);
    }

    // stored namespaces
    for (const std::string& xmlns : _vecStrShared)
    {
        std::string xmlns_name = xmlns.substr(0, xmlns.find("="));
        const char* xmlns_content = xmlns.c_str() + xmlns_name.size() + 1;
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, BAD_CAST(xmlns_name.c_str()), BAD_CAST(xmlns_content)));
        if (status.error())
        {
            return status;
        }
    }

    // append used ones
    status = Result::FromXML(xmlTextWriterWriteAttributeNS(writer, rawKnownStr[e_xmlns], rawKnownStr[e_ssc], nullptr, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureCommon")));
    if (status.error())
    {
        return status;
    }
    status = Result::FromXML(xmlTextWriterWriteAttributeNS(writer, rawKnownStr[e_xmlns], rawKnownStr[e_ssb], nullptr, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureSignalDictionary")));
    if (status.error())
    {
        return status;
    }
    status = Result::FromXML(xmlTextWriterWriteAttributeNS(writer, rawKnownStr[e_xmlns], rawKnownStr[e_ssd], nullptr, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureDescription")));
    if (status.error())
    {
        return status;
    }
    status = Result::FromXML(xmlTextWriterWriteAttributeNS(writer, rawKnownStr[e_xmlns], rawKnownStr[e_ssv], nullptr, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureParameterValues")));
    if (status.error())
    {
        return status;
    }
    status = Result::FromXML(xmlTextWriterWriteAttributeNS(writer, rawKnownStr[e_xmlns], rawKnownStr[e_ssm], nullptr, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureParameterMapping")));
    if (status.error())
    {
        return status;
    }
    status = Result::FromXML(xmlTextWriterWriteAttributeNS(writer, rawKnownStr[e_xmlns], rawKnownStr[e_xcos], nullptr, BAD_CAST("http://scilab.org/software/xcos/SystemStructurePackage")));
    if (status.error())
    {
        return status;
    }

    // write the System content
    ChildrenCategories c(_strShared, _vecDblShared, _vecIntShared, _vecStrShared, _vecIDShared);
    status = writeSystem(writer, o, c);
    if (status.error())
    {
        return status;
    }

    status = writeUnits(writer, o);
    if (status.error())
    {
        return status;
    }

    status = writeAnnotations(writer);
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    return status;
}

SSPResource::Result SSPResource::writeSystem(xmlTextWriterPtr writer, model::BaseObject* o, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_System], nullptr));
    if (status.error())
    {
        return status;
    }

    // use provided name from variable-based modeling tools
    _strShared = categories.retrieve_name(controller, o);
    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_name], BAD_CAST(_strShared.c_str())));
    if (status.error())
    {
        return status;
    }

    if (!controller.getObjectProperty(o, DESCRIPTION, _strShared))
    {
        return Result::Error(o, DESCRIPTION);
    }
    if (_strShared != "")
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_description], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }
    }

    // verbose for debugging
    auto logger = get_or_allocate_logger();
    if (logger->getLevel() <= LOG_DEBUG)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "ScicosID %lld", o->id()));
        if (status.error())
        {
            return status;
        }
    }

    if (!controller.getObjectProperty(o, CHILDREN, _vecIDShared))
    {
        return Result::Error(o, CHILDREN);
    }

    ChildrenCategories c{_strShared, _vecDblShared, _vecIntShared, _vecStrShared, _vecIDShared};
    status = c.load_children(controller, o, _vecIDShared);
    if (status.error())
    {
        return status;
    }
    status = c.assign_outter_ports(controller, o);
    if (status.error())
    {
        return status;
    }

    /*
     * Serialize outter ports on sub-systems, the inner I/O blocks will match the interface
     */

    status = writeConnectors(writer, c);
    if (status.error())
    {
        return status;
    }

    status = writeParameterBindings(writer);
    if (status.error())
    {
        return status;
    }

    status = writeElements(writer, c);
    if (status.error())
    {
        return status;
    }

    status = writeConnections(writer, c);
    if (status.error())
    {
        return status;
    }

    status = writeGraphicalElements(writer, c);
    if (status.error())
    {
        return status;
    }

    status = writeSystemGeometry(writer, o, c);
    if (status.error())
    {
        return status;
    }

    status = writeAnnotations(writer, o, c);
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    // copy generated names into the parent
    categories.insert_all_names(c);

    return Result::Ok();
}

SSPResource::Result SSPResource::writeConnectors(xmlTextWriterPtr writer, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    if (categories.all_ports.empty())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Connectors], nullptr));
    if (status.error())
    {
        return status;
    }

    // names are always set, by load_children() or load_ports()
    for (const ChildrenCategories::all_port_t& o : categories.all_ports)
    {
        status = writeConnector(writer, o, o.name, categories.max_indexes);
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

    return status;
}

SSPResource::Result SSPResource::writeConnector(xmlTextWriterPtr writer, const ChildrenCategories::all_port_t& o, std::string name, const std::array<int, 5>& max_indexes)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Connector], nullptr));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_name], BAD_CAST(name.c_str())));
    if (status.error())
    {
        return status;
    }

    _vecStrShared = {"", "input", "output", "input", "output"};
    if (o.kind < 0 || o.kind >= _vecStrShared.size())
    {
        return Result::Error(o.outter_port, PORT_KIND);
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_kind], BAD_CAST(_vecStrShared[o.kind].c_str())));
    if (status.error())
    {
        return status;
    }

    model::BaseObject* port;
    if (o.outter_port != nullptr)
    {
        port = o.outter_port;
    }
    else
    {
        port = o.inner_port;
    }

    if (!controller.getObjectProperty(port, DESCRIPTION, _strShared))
    {
        return Result::Error(port, DESCRIPTION);
    }
    if (_strShared != "")
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_description], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }
    }

    // verbose for debugging
    auto logger = get_or_allocate_logger();
    if (logger->getLevel() <= LOG_DEBUG)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "ScicosID %lld", port->id()));
        if (status.error())
        {
            return status;
        }
        if (o.block != nullptr)
        {
            status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "block ScicosID %lld", o.block->id()));
            if (status.error())
            {
                return status;
            }
        }
        if (o.inner_port != nullptr)
        {
            status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "inner_port ScicosID %lld", o.inner_port->id()));
            if (status.error())
            {
                return status;
            }
        }
        if (o.outter_port != nullptr)
        {
            status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "outter_port ScicosID %lld", o.outter_port->id()));
            if (status.error())
            {
                return status;
            }
        }
    }

    status = writeType(writer, o.kind, port);
    if (status.error())
    {
        return status;
    }

    status = writeConnectorGeometry(writer, o, max_indexes);
    if (status.error())
    {
        return status;
    }

    status = writeAnnotations(writer, o);
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeType(xmlTextWriterPtr writer, enum portKind kind, model::BaseObject* o)
{
    Result status = Result::Ok();

    if (!controller.getObjectProperty(o, DATATYPE, _vecIntShared))
    {
        return Result::Error(o, DATATYPE);
    }
    if (_vecIntShared.size() != 3)
    {
        return Result::Error(o, DATATYPE);
    }

    int rows = _vecIntShared[0];
    int columns = _vecIntShared[1];
    int datatype_id = _vecIntShared[2];

    // map to the SSP type names, scicos values are:
    // * 1 : double
    // * 2 : complex
    // * 3 : int32
    // * 4 : int16
    // * 5 : int8
    // * 6 : uint32
    // * 7 : uint16
    // * 8 : uint8
    enum xcosNames datatype = NB_XCOS_NAMES;
    switch (datatype_id)
    {
        case 1:
            datatype = e_Real;
            break;
        case 2:
            // convert complex to raw Real for real part
            datatype = e_Real;
            break;
        case 3:
            datatype = e_Int32;
            break;
        case 4:
            datatype = e_Int16;
            break;
        case 5:
            datatype = e_Int8;
            break;
        case 6:
            datatype = e_UInt32;
            break;
        case 7:
            datatype = e_UInt16;
            break;
        case 8:
            datatype = e_UInt8;
            break;

        default:
            // Port is using type flow propagation (eg. inherit), do not set a datatype in SSP
            datatype = NB_XCOS_NAMES;
            break;
    }

    if (datatype != NB_XCOS_NAMES)
    {
        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[datatype], nullptr));
        if (status.error())
        {
            return status;
        }

        if (!controller.getObjectProperty(o, PARAMETER_UNIT, _strShared))
        {
            return Result::Error(o, PARAMETER_UNIT);
        }

        if (!_strShared.empty())
        {
            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_unit], BAD_CAST(_strShared.c_str())));
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
    }

    // Write SSP Dimension

    if (rows == 1 && columns == 1)
    {
        // scalar case, there is no Dimension. SSP representation is enough for scalar.
    }

    // write rows
    if (rows > 1 || (rows == 1 && columns > 1))
    {
        // SSP supported dimension, use the Dimension element for rows
        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[e_Dimension], nullptr));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_size], BAD_CAST(to_string(rows).c_str())));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterEndElement(writer));
        if (status.error())
        {
            return status;
        }
    }

    // write columns
    if (rows >= 1 && columns > 1)
    {
        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[e_Dimension], nullptr));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_size], BAD_CAST(to_string(columns).c_str())));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterEndElement(writer));
        if (status.error())
        {
            return status;
        }
    }

    // Write Clock
    if (kind == PORT_EIN || kind == PORT_EOUT)
    {
        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[e_Clock], nullptr));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterEndElement(writer));
        if (status.error())
        {
            return status;
        }
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeConnectorGeometry(xmlTextWriterPtr writer, const ChildrenCategories::all_port_t& o, const std::array<int, 5>& max_indexes)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_ConnectorGeometry], nullptr));
    if (status.error())
    {
        return status;
    }

    // invert y-axis, geometry is relative
    double x, y;
    switch (o.kind)
    {
        case PORT_IN:
            x = 0.;
            y = 1. - static_cast<double>(o.port_index) / max_indexes[PORT_IN];
            break;
        case PORT_OUT:
            x = 1.;
            y = 1. - static_cast<double>(o.port_index) / max_indexes[PORT_OUT];
            break;
        case PORT_EIN:
            x = static_cast<double>(o.port_index) / max_indexes[PORT_EIN];
            y = 1;
            break;
        case PORT_EOUT:
            x = static_cast<double>(o.port_index) / max_indexes[PORT_EOUT];
            y = 0;
            break;
        default:
            return Result::Error(o.block, PORT_KIND);
    }

    if (x < 0 || 1 < x || y < 0 || 1 < y)
    {
        // something goes wrong, stop the save with a failure message
        return Result::Error(o.outter_port, GEOMETRY);
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x], BAD_CAST(to_string(x).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y], BAD_CAST(to_string(y).c_str())));
    if (status.error())
    {
        return status;
    }

    // write the inner geometry in SSP format
    if (o.block != nullptr)
    {
        if (!controller.getObjectProperty(o.block, GEOMETRY, _vecDblShared))
        {
            return Result::Error(o.block, GEOMETRY);
        }
        if (_vecDblShared.size() != 4)
        {
            return Result::Error(o.block, GEOMETRY);
        }
        auto [x, y, w, h] = *(double (*)[4])_vecDblShared.data();

        // FIXME compute systemInnerX and systemInnerY
        double systemInnerX = x;
        double systemInnerY = y;

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_systemInnerX], BAD_CAST(to_string(systemInnerX).c_str())));
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_systemInnerY], BAD_CAST(to_string(systemInnerY).c_str())));
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

SSPResource::Result SSPResource::writeParameterBindings(xmlTextWriterPtr writer)
{
    Result status = Result::Ok();

    /*
     * Parameters bindings does not exist in Scicos, so we create a dummy one
     * TODO: implement a proper way to load and save parameter bindings from other tools
     */

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_ParameterBindings], nullptr));
    if (status.error())
    {
        return status;
    }

    {
        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_ParameterBinding], nullptr));
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_prefix], BAD_CAST("")));
        if (status.error())
        {
            return status;
        }

        {
            status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_ParameterValues], nullptr));
            if (status.error())
            {
                return status;
            }

            {
                status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssv], rawKnownStr[e_ParameterSet], nullptr));
                if (status.error())
                {
                    return status;
                }

                status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_version], BAD_CAST("2.0")));
                if (status.error())
                {
                    return status;
                }

                status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_name], BAD_CAST("Parameter-Set")));
                if (status.error())
                {
                    return status;
                }

                {
                    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssv], rawKnownStr[e_Parameters], nullptr));
                    if (status.error())
                    {
                        return status;
                    }

                    status = Result::FromXML(xmlTextWriterEndElement(writer));
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
            }

            status = Result::FromXML(xmlTextWriterEndElement(writer));
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
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeElements(xmlTextWriterPtr writer, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Elements], nullptr));
    if (status.error())
    {
        return status;
    }

    for (model::BaseObject* o : categories.elements)
    {
        _strShared.clear();
        if (o->kind() == BLOCK && !controller.getObjectProperty(o, INTERFACE_FUNCTION, _strShared))
        {
            return Result::Error(o, INTERFACE_FUNCTION);
        }

        if (_strShared == "SUPER_f")
        {
            status = writeSystem(writer, o, categories);
            if (status.error())
            {
                return status;
            }
        }
        else
        {
            status = writeComponent(writer, o, categories);
            if (status.error())
            {
                return status;
            }
        }
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeComponent(xmlTextWriterPtr writer, model::BaseObject* component, ChildrenCategories& categories)
{
    Result status = Result::Ok();
    State* st = static_cast<State*>(context);

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Component], nullptr));
    if (status.error())
    {
        return status;
    }

    // use provided name from variable-based modeling tools
    _strShared = categories.retrieve_name(controller, component);
    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_name], BAD_CAST(_strShared.c_str())));
    if (status.error())
    {
        return status;
    }

    if (!controller.getObjectProperty(component, DESCRIPTION, _strShared))
    {
        return Result::Error(component, DESCRIPTION);
    }
    if (_strShared != "")
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_description], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }
    }

    _strShared.clear();
    if (component->kind() == BLOCK && !controller.getObjectProperty(component, INTERFACE_FUNCTION, _strShared))
    {
        return Result::Error(component, INTERFACE_FUNCTION);
    }

    // special case: for FMU blocks, make it available to other tools following the SSP specification
    bool isFMU = _strShared == "FMU_WRAPPER" || _strShared == "SimpleFMU";
    if (isFMU)
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_type], rawKnownStr[e_application_x_fmu_sharedlibrary]));
        if (status.error())
        {
            return status;
        }

        if (!controller.getObjectProperty(component, EXPRS, _vecStrShared))
        {
            return Result::Error(component, EXPRS);
        }

        // delayed store for the FMU file path
        if (_vecStrShared.size() > 0)
        {
            std::string filepath = _vecStrShared[0];
            st->insert_fmu(filepath);

            _strShared = "resources/" + filepath;
            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_source], BAD_CAST(_strShared.c_str())));
            if (status.error())
            {
                return status;
            }
        }
        // std::string workdir = _vecStrShared[1]; // not used
        if (_vecStrShared.size() > 2)
        {
            std::string fmuImpl = _vecStrShared[2];
            if (fmuImpl.size() > 3)
            {
                std::string fmiType = fmuImpl.substr(0, 2);
                std::string fmiVersion = fmuImpl.substr(3);

                if (fmiType == "cs")
                {
                    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_implementation], rawKnownStr[e_CoSimulation]));
                }
                else if (fmiType == "me")
                {
                    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_implementation], rawKnownStr[e_ModelExchange]));
                }
                else if (fmiType == "se")
                {
                    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_implementation], rawKnownStr[e_ScheduledExecution]));
                }
                else
                {
                    return Result::Error(component, EXPRS);
                }
                if (status.error())
                {
                    return status;
                }
            }
        }
    }

    // verbose for debugging
    auto logger = get_or_allocate_logger();
    if (logger->getLevel() <= LOG_DEBUG)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "ScicosID %lld", component->id()));
        if (status.error())
        {
            return status;
        }
    }

    ChildrenCategories c{_strShared, _vecDblShared, _vecIntShared, _vecStrShared, _vecIDShared};
    status = c.load_ports(controller, component);
    if (status.error())
    {
        return status;
    }

    status = writeConnectors(writer, c);
    if (status.error())
    {
        return status;
    }

    status = writeAnnotations(writer, component, c);
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    // copy generated names into the parent
    categories.insert_all_names(c);

    return Result::Ok();
}

SSPResource::Result SSPResource::writeComponentObjectProperties(xmlTextWriterPtr writer, model::BaseObject* component, const ChildrenCategories& categories)
{
    Result status = Result::Ok();

    // write INTERFACE_FUNCTION
    status = writeAnnotationObjectProperty(writer, component, INTERFACE_FUNCTION, e_interface_function, _strShared);
    if (status.error())
    {
        return status;
    }
    bool writtenAsSystem = _strShared == "SUPER_f";

    // write EXPRS
    status = writeAnnotationObjectProperty(writer, component, EXPRS, e_exprs, _vecDblShared);
    if (status.error())
    {
        return status;
    }

    // write NAME as it may set by the user with an SSP invalid value (eg. non unique or with special chars)
    status = writeAnnotationObjectProperty(writer, component, NAME, e_name, _strShared);
    if (status.error())
    {
        return status;
    }

    // write STYLE
    status = writeAnnotationObjectProperty(writer, component, STYLE, e_style, _strShared);
    if (status.error())
    {
        return status;
    }

    // write NZCROSS
    status = writeAnnotationObjectProperty(writer, component, NZCROSS, e_nzcross, _vecIntShared);
    if (status.error())
    {
        return status;
    }

    // write NMODE
    status = writeAnnotationObjectProperty(writer, component, NMODE, e_nmode, _vecIntShared);
    if (status.error())
    {
        return status;
    }

    // write EQUATIONS
    status = writeAnnotationObjectProperty(writer, component, EQUATIONS, e_equations, _vecDblShared);
    if (status.error())
    {
        return status;
    }

    // write SIM_FUNCTION_NAME
    status = writeAnnotationObjectProperty(writer, component, SIM_FUNCTION_NAME, e_sim_function_name, _strShared);
    if (status.error())
    {
        return status;
    }

    // write SIM_FUNCTION_API
    _vecIntShared.resize(1);
    status = writeAnnotationObjectProperty(writer, component, SIM_FUNCTION_API, e_sim_function_api, _vecIntShared[0]);
    if (status.error())
    {
        return status;
    }

    // write SIM_DEP_UT
    status = writeAnnotationObjectProperty(writer, component, SIM_DEP_UT, e_sim_dep_ut, _vecIntShared);
    if (status.error())
    {
        return status;
    }

    // write SIM_BLOCKTYPE
    status = writeAnnotationObjectProperty(writer, component, SIM_BLOCKTYPE, e_sim_blocktype, _strShared);
    if (status.error())
    {
        return status;
    }

    // write RPAR
    status = writeAnnotationObjectProperty(writer, component, RPAR, e_rpar, _vecDblShared);
    if (status.error())
    {
        return status;
    }
    // write IPAR
    status = writeAnnotationObjectProperty(writer, component, IPAR, e_ipar, _vecIntShared);
    if (status.error())
    {
        return status;
    }
    // write OPAR
    status = writeAnnotationObjectProperty(writer, component, OPAR, e_opar, _vecDblShared);
    if (status.error())
    {
        return status;
    }

    // TODO: named parameters are not serialized

    // write STATE
    status = writeAnnotationObjectProperty(writer, component, STATE, e_state, _vecDblShared);
    if (status.error())
    {
        return status;
    }

    // write DSTATE
    status = writeAnnotationObjectProperty(writer, component, DSTATE, e_dstate, _vecDblShared);
    if (status.error())
    {
        return status;
    }

    // write ODSTATE
    status = writeAnnotationObjectProperty(writer, component, ODSTATE, e_odstate, _vecDblShared);
    if (status.error())
    {
        return status;
    }

    // write COLOR
    status = writeAnnotationObjectProperty(writer, component, COLOR, e_color, _vecIntShared);
    if (status.error())
    {
        return status;
    }

    // write PROPERTIES (eg. Simulation properties)
    status = writeAnnotationObjectProperty(writer, component, PROPERTIES, e_properties, _vecDblShared);
    if (status.error())
    {
        return status;
    }

    // write DEBUG_LEVEL
    _vecIntShared.resize(1);
    status = writeAnnotationObjectProperty(writer, component, DEBUG_LEVEL, e_debug_level, _vecIntShared[0]);
    if (status.error())
    {
        return status;
    }

    // write CONTEXT
    status = writeAnnotationObjectProperty(writer, component, DIAGRAM_CONTEXT, e_context, _vecStrShared);
    if (status.error())
    {
        return status;
    }

    // write children as sub-system for Xcos block implemented as blocks
    // this will write ssd:System within ssc:Annotation, preserving the hierarchy and decoding features
    if (!writtenAsSystem)
    {
        if (!controller.getObjectProperty(component, CHILDREN, _vecIDShared))
        {
            return Result::Error(component, CHILDREN);
        }

        if (_vecIDShared.size() > 0)
        {
            ChildrenCategories c{_strShared, _vecDblShared, _vecIntShared, _vecStrShared, _vecIDShared};
            // add inner children
            status = c.load_children(controller, component, _vecIDShared);
            if (status.error())
            {
                return status;
            }

            // assign each outter_port to their corresponding inner port
            status = c.assign_outter_ports(controller, component);
            if (status.error())
            {
                return status;
            }

            // inner connections refer to the component connectors, named by load_ports()
            for (auto& p : c.all_ports)
            {
                auto it = categories.names.find(p.outter_port);
                if (p.outter_port != nullptr && it != categories.names.end())
                {
                    p.name = it->second;
                }
            }

            status = writeElements(writer, c);
            if (status.error())
            {
                return status;
            }

            status = writeConnections(writer, c);
            if (status.error())
            {
                return status;
            }

            status = writeSystemGeometry(writer, component, c);
            if (status.error())
            {
                return status;
            }
        }
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeElementGeometry(xmlTextWriterPtr writer, model::BaseObject* component)
{
    Result status = Result::Ok();

    if (!controller.getObjectProperty(component, GEOMETRY, _vecDblShared))
    {
        return Result::Error(component, GEOMETRY);
    }
    if (_vecDblShared.size() != 4)
    {
        return Result::Error(component, GEOMETRY);
    }
    auto [x, y, w, h] = *(double (*)[4])_vecDblShared.data();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_ElementGeometry], nullptr));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x1], BAD_CAST(to_string(x).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y1], BAD_CAST(to_string(y).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x2], BAD_CAST(to_string(x + w).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y2], BAD_CAST(to_string(y - h).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeConnections(xmlTextWriterPtr writer, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Connections], nullptr));
    if (status.error())
    {
        return status;
    }

    for (model::BaseObject* o : categories.connections)
    {
        status = writeConnection(writer, o, categories);
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

SSPResource::Result SSPResource::writeConnection(xmlTextWriterPtr writer, model::BaseObject* connection, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Connection], nullptr));
    if (status.error())
    {
        return status;
    }

    /*
     * Resolve all needed objects
     */
    model::BaseObject* parent;
    model::BaseObject* sourcePort;
    model::BaseObject* sourceBlock;
    model::BaseObject* destinationPort;
    model::BaseObject* destinationBlock;
    {
        ScicosID id;

        if (!controller.getObjectProperty(connection, PARENT_BLOCK, id))
        {
            return Result::Error(connection, PARENT_BLOCK);
        }
        if (id == ScicosID())
        {
            if (!controller.getObjectProperty(connection, PARENT_DIAGRAM, id))
            {
                return Result::Error(connection, PARENT_DIAGRAM);
            }
        }
        parent = controller.getBaseObject(id);
        if (parent == nullptr)
        {
            return Result::Error(connection, PARENT_BLOCK);
        }

        if (!controller.getObjectProperty(connection, SOURCE_PORT, id))
        {
            return Result::Error(connection, SOURCE_PORT);
        }
        sourcePort = controller.getBaseObject(id);
        if (sourcePort == nullptr)
        {
            return Result::Error(connection, SOURCE_PORT);
        }

        if (!controller.getObjectProperty(sourcePort, SOURCE_BLOCK, id))
        {
            return Result::Error(sourcePort, SOURCE_BLOCK);
        }
        sourceBlock = controller.getBaseObject(id);
        if (sourceBlock == nullptr)
        {
            return Result::Error(sourcePort, SOURCE_BLOCK);
        }

        if (!controller.getObjectProperty(connection, DESTINATION_PORT, id))
        {
            return Result::Error(connection, DESTINATION_PORT);
        }
        destinationPort = controller.getBaseObject(id);
        if (destinationPort == nullptr)
        {
            return Result::Error(connection, DESTINATION_PORT);
        }

        if (!controller.getObjectProperty(destinationPort, SOURCE_BLOCK, id))
        {
            return Result::Error(destinationPort, SOURCE_BLOCK);
        }
        destinationBlock = controller.getBaseObject(id);
        if (destinationBlock == nullptr)
        {
            return Result::Error(destinationPort, SOURCE_BLOCK);
        }
    }

    auto it = std::find_if(categories.all_ports.begin(), categories.all_ports.end(), [=](const ChildrenCategories::all_port_t& p)
                           { return p.inner_port == sourcePort; });
    if (it != categories.all_ports.end())
    {
        // local ioBlock is resolved, do not need to resolve startElement
        _strShared = it->name;
    }
    else
    {
        // resolve startElement
        _strShared = categories.get_name(controller, sourceBlock);
        if (_strShared == "")
        {
            return Result::Error(sourceBlock, NAME);
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_startElement], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }

        // resolve startConnector
        _strShared = categories.get_name(controller, sourcePort);
    }

    if (_strShared == "")
    {
        return Result::Error(sourcePort, NAME);
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_startConnector], BAD_CAST(_strShared.c_str())));
    if (status.error())
    {
        return status;
    }

    it = std::find_if(categories.all_ports.begin(), categories.all_ports.end(), [=](const ChildrenCategories::all_port_t& o)
                      { return o.inner_port == destinationPort; });
    if (it != categories.all_ports.end())
    {
        // local ioBlock is resolved, do not need to resolve endElement
        _strShared = it->name;
    }
    else
    {
        // resolve endElement
        _strShared = categories.get_name(controller, destinationBlock);
        if (_strShared == "")
        {
            return Result::Error(destinationBlock, NAME);
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_endElement], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }

        // resolve endConnector
        _strShared = categories.get_name(controller, destinationPort);
    }

    if (_strShared == "")
    {
        return Result::Error(destinationPort, NAME);
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_endConnector], BAD_CAST(_strShared.c_str())));
    if (status.error())
    {
        return status;
    }

    if (!controller.getObjectProperty(connection, NAME, _strShared))
    {
        return Result::Error(connection, NAME);
    }
    if (_strShared != "")
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_id], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }
    }

    if (!controller.getObjectProperty(connection, DESCRIPTION, _strShared))
    {
        return Result::Error(connection, DESCRIPTION);
    }
    if (_strShared != "")
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_description], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }
    }

    // verbose for debugging
    auto logger = get_or_allocate_logger();
    if (logger->getLevel() <= LOG_DEBUG)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "ScicosID %lld : from ScicosID %lld to ScicosID %lld", connection->id(), sourcePort->id(), destinationPort->id()));
        if (status.error())
        {
            return status;
        }
    }

    status = writeConnectionGeometry(writer, connection);
    if (status.error())
    {
        return status;
    }

    status = writeAnnotations(writer, connection, categories);
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeConnectionGeometry(xmlTextWriterPtr writer, model::BaseObject* o)
{
    // list of intermediate waypoint coordinates, which are to be interpreted as for the svg:polyline primitive
    Result status = Result::Ok();

    if (!controller.getObjectProperty(o, CONTROL_POINTS, _vecDblShared))
    {
        return Result::Error(o, CONTROL_POINTS);
    }

    // do not output empty geometry
    if (_vecDblShared.empty())
    {
        return Result::Ok();
    }

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_ConnectionGeometry], nullptr));
    if (status.error())
    {
        return status;
    }

    _strShared.clear();
    for (size_t i = 2; i < _vecDblShared.size() - 2; i += 2)
    {
        _strShared.append(to_string(_vecDblShared[i]));
        _strShared.append(" ");
    }
    if (_strShared.size() > 0)
    {
        _strShared.resize(_strShared.size() - 1);
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_pointsX], BAD_CAST(_strShared.c_str())));
    if (status.error())
    {
        return status;
    }

    // TODO: translate the coordinates to the SVG coordinate system

    _strShared.clear();
    for (size_t i = 2; i < _vecDblShared.size() - 2; i += 2)
    {
        _strShared.append(to_string(_vecDblShared[i + 1]));
        _strShared.append(" ");
    }
    if (_strShared.size() > 0)
    {
        _strShared.resize(_strShared.size() - 1);
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_pointsY], BAD_CAST(_strShared.c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeSystemGeometry(xmlTextWriterPtr writer, model::BaseObject* o, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    // SSP specifiction states: If undefined, the system canvas extent defaults to the bounding box of all ElementGeometry elements of the child elements of the system.
    // however we need to compute the bounding box of all elements in the system, to be able to revert the Y-axis
    const SystemCanvas& canvas = categories.canvas;

    // ensure the canvas is valid
    // from SSP specification: This element defines the extent of the system canvas. (x1,y1) and (x2,y2) define the lower-left and upper-right corner, respectively. Different from ElementGeometry, where x1 > x2 and y1 > y2 indicate flipping, x1 < x2 and y1 < y2 MUST hold here.
    if (canvas.x1 >= canvas.x2 || canvas.y1 >= canvas.y2)
    {
        // do not write SystemGeometry on empty canvas
        return Result::Ok();
    }

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_SystemGeometry], nullptr));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x1], BAD_CAST(to_string(canvas.x1).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y1], BAD_CAST(to_string(canvas.y1).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x2], BAD_CAST(to_string(canvas.x2).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y2], BAD_CAST(to_string(canvas.y2).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }
    return Result::Ok();
}

SSPResource::Result SSPResource::writeGraphicalElements(xmlTextWriterPtr writer, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    if (categories.graphical_elements.empty())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_GraphicalElements], nullptr));
    if (status.error())
    {
        return status;
    }

    for (model::BaseObject* o : categories.graphical_elements)
    {
        status = writeNote(writer, o, categories);
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

SSPResource::Result SSPResource::writeNote(xmlTextWriterPtr writer, model::BaseObject* o, ChildrenCategories& categories)
{
    Result status = Result::Ok();

    // verbose for debugging
    auto logger = get_or_allocate_logger();
    if (logger->getLevel() <= LOG_DEBUG)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatComment(writer, "ScicosID %lld", o->id()));
        if (status.error())
        {
            return status;
        }
    }

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Note], nullptr));
    if (status.error())
    {
        return status;
    }

    if (!controller.getObjectProperty(o, GEOMETRY, _vecDblShared))
    {
        return Result::Error(o, GEOMETRY);
    }
    if (_vecDblShared.size() != 4)
    {
        return Result::Error(o, GEOMETRY);
    }
    auto [x, y, w, h] = *(double (*)[4])_vecDblShared.data();

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x1], BAD_CAST(to_string(x).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y1], BAD_CAST(to_string(y).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x2], BAD_CAST(to_string(x + w).c_str())));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y2], BAD_CAST(to_string(y - h).c_str())));
    if (status.error())
    {
        return status;
    }

    // Write the description text within the text if possible
    if (!controller.getObjectProperty(o, DESCRIPTION, _strShared))
    {
        return Result::Error(o, DESCRIPTION);
    }
    if (can_write_to_string(_strShared))
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_text], BAD_CAST(_strShared.c_str())));
        if (status.error())
        {
            return status;
        }
    }

    status = writeAnnotations(writer, o, categories);
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }
    return Result::Ok();
}

SSPResource::Result SSPResource::writeUnits(xmlTextWriterPtr writer, model::BaseObject* o)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Units], nullptr));
    if (status.error())
    {
        return status;
    }

    std::vector<model::Datatype*> datatypes;
    if (o->kind() == BLOCK)
    {
        model::Block* block = (model::Block*)o;
        datatypes = block->getDatatypes();
    }
    else if (o->kind() == DIAGRAM)
    {
        model::Diagram* diagram = (model::Diagram*)o;
        datatypes = diagram->getDatatypes();
    }

    for (model::Datatype* d : datatypes)
    {
        status = writeUnit(writer, d);
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

SSPResource::Result SSPResource::writeUnit(xmlTextWriterPtr writer, model::Datatype* d)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[e_Unit], nullptr));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_name], BAD_CAST(d->m_unit.name.c_str())));
    if (status.error())
    {
        return status;
    }

    status = writeBaseUnit(writer, d);
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }
    return Result::Ok();
}

SSPResource::Result SSPResource::writeBaseUnit(xmlTextWriterPtr writer, model::Datatype* d)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[e_BaseUnit], nullptr));
    if (status.error())
    {
        return status;
    }

#define WRITE(ATT)                                                                                                             \
    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, BAD_CAST(#ATT), BAD_CAST(to_string(d->m_unit.ATT).c_str()))); \
    if (status.error())                                                                                                        \
    {                                                                                                                          \
        return status;                                                                                                         \
    }

    WRITE(kg);
    WRITE(m);
    WRITE(s);
    WRITE(A);
    WRITE(K);
    WRITE(mol);
    WRITE(cd);
    WRITE(rad);
    WRITE(factor);
    WRITE(offset);

#undef WRITE

    status = Result::FromXML(xmlTextWriterEndElement(writer));
    if (status.error())
    {
        return status;
    }
    return Result::Ok();
}

SSPResource::Result SSPResource::writeAnnotations(xmlTextWriterPtr writer)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Annotations], nullptr));
    if (status.error())
    {
        return status;
    }

    if (!controller.getObjectProperty(root, DIAGRAM, GLOBAL_SSP_ANNOTATION, _vecStrShared))
    {
        return Result::Error(controller.getBaseObject(root), GLOBAL_SSP_ANNOTATION);
    }

    for (const std::string& s : _vecStrShared)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatRaw(writer, "%s\n", s.c_str()));
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

SSPResource::Result SSPResource::writeAnnotations(xmlTextWriterPtr writer, model::BaseObject* o, const ChildrenCategories& categories)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Annotations], nullptr));
    if (status.error())
    {
        return status;
    }

    //
    // write Xcos annotations
    //

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[e_Annotation], nullptr));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_type], rawKnownStr[e_org_scilab_xcos_ssp]));
    if (status.error())
    {
        return status;
    }

    if (o->kind() != DIAGRAM)
    {
        status = writeAnnotationObjectProperty(writer, o, UID, e_uid, _strShared);
        if (status.error())
        {
            return status;
        }
    }

    // write the full geometry as Xcos annotation, reconciliation might happen when loading the file back
    if (o->kind() == BLOCK || o->kind() == ANNOTATION)
    {
        // xcos:geometry
        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_xcos], rawKnownStr[e_geometry], nullptr));
        if (status.error())
        {
            return status;
        }

        if (!controller.getObjectProperty(o, GEOMETRY, _vecDblShared))
        {
            return Result::Error(o, GEOMETRY);
        }
        if (_vecDblShared.size() != 4)
        {
            return Result::Error(o, GEOMETRY);
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x], BAD_CAST(to_string(_vecDblShared[0]).c_str())));
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y], BAD_CAST(to_string(_vecDblShared[1]).c_str())));
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_width], BAD_CAST(to_string(_vecDblShared[2]).c_str())));
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_height], BAD_CAST(to_string(_vecDblShared[3]).c_str())));
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterEndElement(writer));
        if (status.error())
        {
            return status;
        }
    }

    if (o->kind() == BLOCK)
    {
        status = writeComponentObjectProperties(writer, o, categories);
        if (status.error())
        {
            return status;
        }
    }

    if (o->kind() == ANNOTATION)
    {
        status = writeAnnotationObjectProperty(writer, o, DESCRIPTION, e_description, _strShared);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, o, FONT, e_font, _strShared);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, o, FONT_SIZE, e_font_size, _strShared);
        if (status.error())
        {
            return status;
        }
    }

    if (o->kind() == LINK)
    {
        status = writeAnnotationObjectProperty(writer, o, CONTROL_POINTS, e_control_points, _vecDblShared);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, o, STYLE, e_style, _strShared);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, o, THICK, e_thick, _vecDblShared);
        if (status.error())
        {
            return status;
        }
        int color;
        status = writeAnnotationObjectProperty(writer, o, COLOR, e_color, color);
        if (status.error())
        {
            return status;
        }
        int kind;
        status = writeAnnotationObjectProperty(writer, o, KIND, e_kind, kind);
        if (status.error())
        {
            return status;
        }
    }

    // write LABEL : a reference to an Xcos Annotation
    if (o->kind() == BLOCK || o->kind() == LINK)
    {
        status = writeAnnotationLabel(writer, o);
        if (status.error())
        {
            return status;
        }
    }

    if (o->kind() == PORT)
    {
        status = writeAnnotationObjectProperty(writer, o, DATATYPE, e_datatype, _vecIntShared);
        if (status.error())
        {
            return status;
        }
        bool implicit = false;
        status = writeAnnotationObjectProperty(writer, o, IMPLICIT, e_implicit, implicit);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, o, STYLE, e_style, _strShared);
        if (status.error())
        {
            return status;
        }
        _vecDblShared.resize(1);
        status = writeAnnotationObjectProperty(writer, o, FIRING, e_firing, _vecDblShared[0]);
        if (status.error())
        {
            return status;
        }
    }

    if (o->kind() == DIAGRAM)
    {
        // write COLOR
        status = writeAnnotationObjectProperty(writer, o, COLOR, e_color, _vecIntShared);
        if (status.error())
        {
            return status;
        }

        // write PROPERTIES (eg. Simulation properties)
        status = writeAnnotationObjectProperty(writer, o, PROPERTIES, e_properties, _vecDblShared);
        if (status.error())
        {
            return status;
        }

        // write DEBUG_LEVEL
        _vecIntShared.resize(1);
        status = writeAnnotationObjectProperty(writer, o, DEBUG_LEVEL, e_debug_level, _vecIntShared[0]);
        if (status.error())
        {
            return status;
        }

        // write CONTEXT
        status = writeAnnotationObjectProperty(writer, o, DIAGRAM_CONTEXT, e_context, _vecStrShared);
        if (status.error())
        {
            return status;
        }

        // TODO write named parameters

        // write VERSION_NUMBER
        status = writeAnnotationObjectProperty(writer, o, VERSION_NUMBER, e_version, _strShared);
        if (status.error())
        {
            return status;
        }

        // write PATH
        status = writeAnnotationObjectProperty(writer, o, PATH, e_path, _strShared);
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

    //
    // write other tools annotations
    //

    if (!controller.getObjectProperty(o, SSP_ANNOTATION, _vecStrShared))
    {
        return Result::Error(o, SSP_ANNOTATION);
    }
    for (const std::string& s : _vecStrShared)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatRaw(writer, "%s\n", s.c_str()));
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

SSPResource::Result SSPResource::writeAnnotationLabel(xmlTextWriterPtr writer, model::BaseObject* o)
{
    Result status = Result::Ok();

    ScicosID label{};
    if (!controller.getObjectProperty(o, LABEL, label))
    {
        return Result::Error(o, LABEL);
    }
    if (label != ScicosID())
    {
        model::BaseObject* labelObj = controller.getBaseObject(label);
        if (labelObj == nullptr)
        {
            return Result::Error(o, LABEL);
        }

        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_xcos], rawKnownStr[e_label], nullptr));
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_xcos], rawKnownStr[e_geometry], nullptr));
        if (status.error())
        {
            return status;
        }
        {
            if (!controller.getObjectProperty(labelObj, GEOMETRY, _vecDblShared))
            {
                return Result::Error(labelObj, GEOMETRY);
            }
            if (_vecDblShared.size() != 4)
            {
                return Result::Error(labelObj, GEOMETRY);
            }

            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x], BAD_CAST(to_string(_vecDblShared[0]).c_str())));
            if (status.error())
            {
                return status;
            }

            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y], BAD_CAST(to_string(_vecDblShared[1]).c_str())));
            if (status.error())
            {
                return status;
            }

            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_width], BAD_CAST(to_string(_vecDblShared[2]).c_str())));
            if (status.error())
            {
                return status;
            }

            status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_height], BAD_CAST(to_string(_vecDblShared[3]).c_str())));
            if (status.error())
            {
                return status;
            }

            status = Result::FromXML(xmlTextWriterEndElement(writer));
            if (status.error())
            {
                return status;
            }
        }

        status = writeAnnotationObjectProperty(writer, labelObj, DESCRIPTION, e_description, _strShared);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, labelObj, FONT, e_font, _strShared);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, labelObj, FONT_SIZE, e_font_size, _strShared);
        if (status.error())
        {
            return status;
        }

        status = Result::FromXML(xmlTextWriterEndElement(writer));
        if (status.error())
        {
            return status;
        }
    }

    return Result::Ok();
}

SSPResource::Result SSPResource::writeAnnotations(xmlTextWriterPtr writer, const ChildrenCategories::all_port_t &all_port_info)
{
    Result status = Result::Ok();

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssd], rawKnownStr[e_Annotations], nullptr));
    if (status.error())
    {
        return status;
    }

    //
    // write Xcos annotations
    //

    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_ssc], rawKnownStr[e_Annotation], nullptr));
    if (status.error())
    {
        return status;
    }

    status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_type], rawKnownStr[e_org_scilab_xcos_ssp]));
    if (status.error())
    {
        return status;
    }

    model::BaseObject* port = all_port_info.outter_port;
    if (port == nullptr)
    {
        port = all_port_info.inner_port;
    }
    if (port != nullptr)
    {
        status = writeAnnotationObjectProperty(writer, port, UID, e_uid, _strShared);
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, port, DATATYPE, e_datatype, _vecIntShared);
        if (status.error())
        {
            return status;
        }
        bool implicit = false;
        status = writeAnnotationObjectProperty(writer, port, IMPLICIT, e_implicit, implicit);
        if (status.error())
        {
            return status;
        }
        _vecDblShared.resize(1);
        status = writeAnnotationObjectProperty(writer, port, FIRING, e_firing, _vecDblShared[0]);
        if (status.error())
        {
            return status;
        }
    }

    // write the Geometry from the inner block
    if (all_port_info.block != nullptr)
    {
        status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_xcos], rawKnownStr[e_geometry], nullptr));
        if (status.error())
        {
            return status;
        }
        if (!controller.getObjectProperty(all_port_info.block, GEOMETRY, _vecDblShared))
        {
            return Result::Error(all_port_info.block, GEOMETRY);
        }
        if (_vecDblShared.size() != 4)
        {
            return Result::Error(all_port_info.block, GEOMETRY);
        }
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_x], BAD_CAST(to_string(_vecDblShared[0]).c_str())));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_y], BAD_CAST(to_string(_vecDblShared[1]).c_str())));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_width], BAD_CAST(to_string(_vecDblShared[2]).c_str())));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_height], BAD_CAST(to_string(_vecDblShared[3]).c_str())));
        if (status.error())
        {
            return status;
        }
        status = Result::FromXML(xmlTextWriterEndElement(writer));
        if (status.error())
        {
            return status;
        }
        status = writeAnnotationObjectProperty(writer, all_port_info.block, STYLE, e_style, _strShared);
        if (status.error())
        {
            return status;
        }
    }

    // write the block_index to load in order CHILDREN using the ipar attribute
    status = Result::FromXML(xmlTextWriterStartElementNS(writer, rawKnownStr[e_xcos], rawKnownStr[e_ipar], nullptr));
    if (status.error())
    {
        return status;
    }
    if (all_port_info.block_index > 0)
    {
        status = Result::FromXML(xmlTextWriterWriteAttribute(writer, rawKnownStr[e_value], BAD_CAST(to_string(all_port_info.block_index).c_str())));
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

    // write block LABEL
    if (all_port_info.block != nullptr)
    {
        status = writeAnnotationLabel(writer, all_port_info.block);
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

    //
    // write other tools annotations
    //

    if (!controller.getObjectProperty(port, SSP_ANNOTATION, _vecStrShared))
    {
        return Result::Error(port, SSP_ANNOTATION);
    }
    for (const std::string& s : _vecStrShared)
    {
        status = Result::FromXML(xmlTextWriterWriteFormatRaw(writer, "%s\n", s.c_str()));
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

} // namespace org_scilab_modules_scicos
