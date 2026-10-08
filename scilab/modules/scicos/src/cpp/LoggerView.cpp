/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cwchar>
#include <iostream>
#include <sstream>
#include <string>

#include "scilabexception.hxx"
#include "scilabWrite.hxx"
extern "C"
{
#include "Scierror.h"
#include "Sciwarning.h"
}

#include "Controller.hxx"
#include "LoggerView.hxx"

namespace org_scilab_modules_scicos
{

static const bool USE_SCILAB_WRITE = true;

// set the shared buffer with non-allocating size
LoggerView::LoggerView() : View(), m_level(LOG_WARNING), m_lastObject(ScicosID()), buffer(15, '\0') {}

LoggerView::~LoggerView() {}

static std::wstring levelTable[] =
    {
        L"TRACE",
        L"DEBUG",
        L"INFO",
        L"WARNING",
        L"ERROR",
        L"FATAL",
};

static std::string displayTable[] =
    {
        "Xcos trace:   ",
        "Xcos debug:   ",
        "Xcos info:    ",
        "Xcos warning: ",
        "Xcos error:   ",
        "Xcos fatal:   ",
};

enum LogLevel LoggerView::indexOf(const wchar_t* name)
{
    for (int i = LOG_TRACE; i <= LOG_FATAL; i++)
    {
        if (!wcscmp(name, levelTable[i].data()))
        {
            return static_cast<enum LogLevel>(i);
        }
    }
    return LOG_UNDEF;
}

const wchar_t* LoggerView::toString(enum LogLevel level)
{
    if (LOG_TRACE <= level && level <= LOG_FATAL)
    {
        return levelTable[level].data();
    }
    return L"";
}

const std::string LoggerView::toDisplay(enum LogLevel level)
{
    if (LOG_TRACE <= level && level <= LOG_FATAL)
    {
        return displayTable[level];
    }
    return "";
}

static int toSeverity(enum LogLevel level)
{
    if (level >= LOG_ERROR)
    {
        return SCI_MSG_ERROR;
    }
    if (level == LOG_WARNING)
    {
        return SCI_MSG_WARNING;
    }
    return SCI_MSG_OUTPUT;
}

void LoggerView::log(enum LogLevel level, const std::string& msg)
{
    if (level >= this->m_level)
    {
        if (USE_SCILAB_WRITE)
        {
            scilab_print((LoggerView::toDisplay(level) + msg).data(), toSeverity(level));
        }
        else
        {
            std::cerr << LoggerView::toDisplay(level) << msg;
        }
    }
}

void LoggerView::log(enum LogLevel level, const char* msg, ...)
{
    if (level >= this->m_level)
    {
        if (buffer.size() < N)
        {
            buffer.resize(N);
        }
        char* str = buffer.data();

        va_list opts;
        va_start(opts, msg);
        vsnprintf(str, N, msg, opts);
        va_end(opts);

        if (USE_SCILAB_WRITE)
        {
            std::string msg = LoggerView::toDisplay(level) + str;
            if (level == LOG_WARNING)
            {
                // map to a Scilab warning
                Sciwarning(msg.data());
            }
            else if (level >= LOG_ERROR)
            {
                // map to a Scilab error
                throw ast::InternalError(msg);
            }
            else
            {
                // report to the console
                scilabForcedWrite(msg.data());
            }
        }
        else
        {
            std::cerr << LoggerView::toDisplay(level) << str;
        }
    }
}

void LoggerView::log(enum LogLevel level, const wchar_t* msg, ...)
{
    if (level >= this->m_level)
    {
        std::vector<wchar_t> buffer(N);
        wchar_t* str = buffer.data();

        va_list opts;
        va_start(opts, msg);
        vswprintf(str, N, msg, opts);
        va_end(opts);

        if (USE_SCILAB_WRITE)
        {
            scilab_print(LoggerView::toDisplay(level).data(), toSeverity(level));
            scilab_printW(str, toSeverity(level));
        }
        else
        {
            std::cerr << LoggerView::toDisplay(level);
            std::wcerr << str;
        }
    }
}

void LoggerView::log(enum LogLevel level, const std::function <std::to_chars_result(char* first, char* last)> to_chars_fun)
{
    if (level >= this->m_level)
    {
        auto result = to_chars_fun(buffer.data(), buffer.data() + buffer.size());
        // early exit if the result is not an error
        if (result.ec == std::errc())
        {
            *result.ptr = '\0';
            if (USE_SCILAB_WRITE)
            {
                scilab_print(LoggerView::toDisplay(level).c_str(), toSeverity(level));
                scilab_print(buffer.data(), toSeverity(level));
            }
            else
            {
                std::cerr << LoggerView::toDisplay(level) << std::string(buffer.data(), result.ptr);
            }
            return;
        }

        // slow case, we need to resize the string
        while (result.ec == std::errc::value_too_large && buffer.size() < N)
        {
            // grow capacity (will reallocate)
            buffer.reserve(buffer.capacity() + 1);
            // make it available
            buffer.resize(buffer.capacity());
            // transform
            result = to_chars_fun(buffer.data(), buffer.data() + buffer.size());
        }

        // handle errors
        if (result.ec == std::errc())
        {
            *result.ptr = '\0';
            if (USE_SCILAB_WRITE)
            {
                scilab_print(LoggerView::toDisplay(level).c_str(), toSeverity(level));
                scilab_print(buffer.data(), toSeverity(level));
            }
            else
            {
                std::cerr << LoggerView::toDisplay(level) << std::string(buffer.data(), result.ptr);
            }
        }
        else if (result.ec == std::errc::invalid_argument)
        {
            // Handle invalid argument case, e.g., log an error message
            log(LOG_ERROR, "programming error: to_chars function called with invalid arguments");
        }
        else
        {
            std::string str = std::make_error_code(result.ec).message();
            // Handle error case, e.g., log an error message
            log(LOG_ERROR, "Failed to convert to_chars: error code %d \"%s\"", result.ec, str.c_str());
        }
    }
}

// operator<<-like function using to_chars(), renamed to avoid conflict with operator<<
template<typename T>
static std::ostream& concat_with_to_chars(std::ostream& os, T t)
{
    std::string str(15, '\0');
    auto result = to_chars(str.data(), str.data() + str.size(), t);
    // fast non allocating case with small string optimization
    if (result.ec == std::errc())
    {
        str.resize(result.ptr - str.data());
        os << str;
        return os;
    }
    // slow case, we need to resize the string
    // limit the resize to 1K bytes to avoid infinite loop
    while(result.ec == std::errc::value_too_large && str.size() < 1024)
    {
        str.resize(str.size() * 2, '\0');
        result = to_chars(str.data(), str.data() + str.size(), t);
    }
    // handle errors
    if (result.ec == std::errc())
    {
        str.resize(result.ptr - str.data());
        os << str;
        return os;
    }
    switch(result.ec)
    {
        case std::errc::invalid_argument:
            return os << "programming error: operator<<(typename T) caller is buggy";
        case std::errc::value_too_large:
            return os << "programming error: operator<<(typename T) is buggy";
        default:
            return os << "programming error on operator<<(typename T)";
    }
    return os;
}

// explicit implementation of operator<< for kind_t
std::ostream& operator<<(std::ostream& os, kind_t k)
{
    return concat_with_to_chars(os, k);
}

// explicit implementation of operator<< for object_properties_t
std::ostream& operator<<(std::ostream& os, object_properties_t p)
{
    return concat_with_to_chars(os, p);
}

void LoggerView::objectCreated(const ScicosID& uid, kind_t k)
{
    log(LOG_INFO, [=](char* first, char* last) {
        return to_chars_t(first, last) + "objectCreated( " + id(uid) + " , " + k + " )\n";
    });
}

void LoggerView::objectReferenced(const ScicosID& uid, kind_t k, unsigned refCount)
{
    log(LOG_TRACE, [=](char* first, char* last) {
        return to_chars_t(first, last) + "objectReferenced( " + id(uid) + " , " + k + " ) : " + refCount + "\n";
    });
}

void LoggerView::objectUnreferenced(const ScicosID& uid, kind_t k, unsigned refCount)
{
    log(LOG_TRACE, [=](char* first, char* last) {
        return to_chars_t(first, last) + "objectUnreferenced( " + id(uid) + " , " + k + " ) : " + refCount + "\n";
    });
}

void LoggerView::objectDeleted(const ScicosID& uid, kind_t k)
{
    log(LOG_INFO, [=](char* first, char* last) {
        return to_chars_t(first, last) + "objectDeleted( " + id(uid) + " , " + k + " )\n";
    });
}

void LoggerView::objectCloned(const ScicosID& uid, const ScicosID& cloned, kind_t k)
{
    log(LOG_INFO, [=](char* first, char* last) {
        return to_chars_t(first, last) + "objectCloned( " + id(uid) + " , " + id(cloned) + " , " + k + " )\n";
    });
}

//
// used to debug a link connection, will be compiled out
//

/* 0-based index of id in content, -1 if not found */
inline int indexOf(ScicosID id, const std::vector<ScicosID>& content)
{
    const auto& it = std::find(content.begin(), content.end(), id);
    if (it == content.end())
        return -1;
    return (int)std::distance(content.begin(), it);
};

/* Scilab-like connected port  */
static inline std::string to_string_port(Controller& controller, ScicosID uid, kind_t k, object_properties_t p)
{
    if (k != LINK)
        return "";
    if (p != SOURCE_PORT && p != DESTINATION_PORT)
        return "";

    ScicosID endID = ScicosID();
    controller.getObjectProperty(uid, k, p, endID);
    if (endID == ScicosID())
    {
        return "";
    }

    ScicosID sourceBlock = ScicosID();
    controller.getObjectProperty(endID, PORT, SOURCE_BLOCK, sourceBlock);
    if (sourceBlock == ScicosID())
    {
        return "";
    }

    ScicosID parent = ScicosID();
    kind_t parentKind = BLOCK;
    controller.getObjectProperty(uid, k, PARENT_BLOCK, parent);
    std::vector<ScicosID> children;
    // Added to a superblock
    if (parent == ScicosID())
    {
        // Added to a diagram
        controller.getObjectProperty(uid, k, PARENT_DIAGRAM, parent);
        parentKind = DIAGRAM;
    }
    if (parent == ScicosID())
    {
        return "";
    }
    controller.getObjectProperty(parent, parentKind, CHILDREN, children);

    std::vector<ScicosID> sourceBlockPorts;
    int portIndex;
    object_properties_t port;
    for (object_properties_t ports : {INPUTS, OUTPUTS, EVENT_INPUTS, EVENT_OUTPUTS})
    {
        controller.getObjectProperty(sourceBlock, BLOCK, ports, sourceBlockPorts);
        portIndex = indexOf(endID, sourceBlockPorts) + 1;
        port = ports;
        if (portIndex > 0)
            break;
    }

    int startOrEnd = 0;
    if (port == INPUTS && p == SOURCE_PORT)
        startOrEnd = 1;
    else if (port == EVENT_INPUTS && p == SOURCE_PORT)
        startOrEnd = 1;
    else if (port == OUTPUTS && p == SOURCE_PORT)
        startOrEnd = 0;
    else if (port == EVENT_OUTPUTS && p == SOURCE_PORT)
        startOrEnd = 0;
    else if (port == INPUTS && p == DESTINATION_PORT)
        startOrEnd = 1;
    else if (port == EVENT_INPUTS && p == DESTINATION_PORT)
        startOrEnd = 1;
    else if (port == OUTPUTS && p == DESTINATION_PORT)
        startOrEnd = 0;
    else if (port == EVENT_OUTPUTS && p == DESTINATION_PORT)
        startOrEnd = 0;

    return "[" + std::to_string(indexOf(sourceBlock, children) + 1) + " " + std::to_string(portIndex) + " " + std::to_string(startOrEnd) + "]";
};

/* Scilab-like connected link  */
static inline std::string to_string_link(Controller& controller, ScicosID uid, kind_t k)
{
    if (k != LINK)
        return "";

    std::vector<ScicosID> path;
    path.push_back(ScicosID());
    ScicosID last = path.back();
    controller.getObjectProperty(uid, k, PARENT_BLOCK, path.back());
    while (path.back() != ScicosID())
    {
        last = path.back();
        path.push_back(ScicosID());
        controller.getObjectProperty(last, BLOCK, PARENT_BLOCK, path.back());
    }
    path.pop_back();

    ScicosID diagram = ScicosID();
    controller.getObjectProperty(last, BLOCK, PARENT_DIAGRAM, diagram);

    // display path
    std::vector<ScicosID> children;
    controller.getObjectProperty(diagram, DIAGRAM, CHILDREN, children);

    std::stringstream ss;
    ss << "LoggerView " << uid;
    ss << " scs_m.objs(";
    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        ss << indexOf(*it, children) + 1 << ").model.rpar.objs(";
        controller.getObjectProperty(*it, BLOCK, CHILDREN, children);
    }
    ss << indexOf(uid, children) + 1 << ") ";

    // display connection
    ss << " = scicos_link(";
    ss << "from=" << to_string_port(controller, uid, k, SOURCE_PORT);
    ss << ",";
    ss << "to=" << to_string_port(controller, uid, k, DESTINATION_PORT);
    ss << ")";

    return ss.str();
};

template<> inline
std::to_chars_result to_chars(char* first, char* last, std::string t)
{
    char* c = t.data();
    // .data() output a null terminated string
    while(*c != '\0' && first < last)
    {
        *first++ = *c++;
    }
    return {first, {}};
}
template<> inline
std::to_chars_result to_chars(char* first, char* last, double t)
{
    return std::to_chars(first, last, (int) t);
}
template<> inline
std::to_chars_result to_chars(char* first, char* last, ScicosID t)
{
    return std::to_chars(first, last, (int) t);
}
template<typename T> inline
std::to_chars_result to_chars(char* first, char* last, std::vector<T> t)
{
    to_chars_t io(first, last);
    io = io + "[";
    if (t.size() > 0) {
        io = io + " " + t[0];
    }
    for (size_t i = 1; i < t.size(); ++i)
    {
        io = io + ", " + t[i];
    }
    io = io + "]";
    return io;
}

void LoggerView::propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p, update_status_t u)
{
    auto to_chars_fun = [=](char* first, char* last) -> std::to_chars_result {
        to_chars_t io(first, last);

        io = io + "propertyUpdated( " + id(uid) + " , " + k + " , " + p + " ) : " + u;

        Controller controller;
        // DEBUG if (p == CHILDREN || p == INPUTS || p == OUTPUTS || p == EVENT_INPUTS || p == EVENT_OUTPUTS || p == CONNECTED_SIGNALS)
        // DEBUG {
        // DEBUG     std::vector<ScicosID> end;
        // DEBUG     controller.getObjectProperty(uid, k, p, end);
        // DEBUG     io = io + end;
        // DEBUG }
        // DEBUG if (p == SOURCE_PORT || p == DESTINATION_PORT || p == SOURCE_BLOCK)
        // DEBUG {
        // DEBUG     ScicosID end;
        // DEBUG     controller.getObjectProperty(uid, k, p, end);
        // DEBUG     io = io + " " + id(end);
        // DEBUG }

        // if (p == DESCRIPTION || p == NAME)
        // {
        //     std::string v;
        //    controller.getObjectProperty(uid, k, p, v);
        //     io = io + " " + v;
        // }

        return io + "\n";
    };

    if (u == NO_CHANGES)
    {
        log(LOG_TRACE, to_chars_fun);
    }
    else
    {
        log(LOG_DEBUG, to_chars_fun);
    }

    // DEBUG if (u == SUCCESS && p == NAME)
    // DEBUG {
    // DEBUG     log(LOG_INFO, [=](char* first, char* last) -> std::to_chars_result {;
    // DEBUG         to_chars_t io(first, last);
    // DEBUG         Controller controller;
    // DEBUG         std::string name;
    // DEBUG         controller.getObjectProperty(uid, k, p, name);
    // DEBUG         io = io + "propertyUpdated( " + id(uid) + " , " + k + " , " + p + " ) :";
    // DEBUG         io = io + " " + name;
    // DEBUG         return io + "\n";
    // DEBUG     });
    // DEBUG }
    // DEBUG
    // DEBUG if (u == SUCCESS && p == DESCRIPTION)
    // DEBUG {
    // DEBUG     log(LOG_INFO, [=](char* first, char* last) -> std::to_chars_result {;
    // DEBUG         to_chars_t io(first, last);
    // DEBUG         Controller controller;
    // DEBUG         std::string name;
    // DEBUG         controller.getObjectProperty(uid, k, p, name);
    // DEBUG         io = io + "propertyUpdated( " + id(uid) + " , " + k + " , " + p + " ) :";
    // DEBUG         io = io + " " + name;
    // DEBUG         return io + "\n";
    // DEBUG     });
    // DEBUG }
    // DEBUG
    // DEBUG if (u == SUCCESS && p == GEOMETRY)
    // DEBUG {
    // DEBUG     log(LOG_INFO, [=](char* first, char* last) -> std::to_chars_result {;
    // DEBUG         to_chars_t io(first, last);
    // DEBUG         Controller controller;
    // DEBUG         std::vector<double> geom;
    // DEBUG         controller.getObjectProperty(uid, k, p, geom);
    // DEBUG         io = io + "propertyUpdated( " + id(uid) + " , " + k + " , " + p + " ) :";
    // DEBUG         io = io + " " + geom;
    // DEBUG         return io + "\n";
    // DEBUG     });
    // DEBUG }
}

} /* namespace org_scilab_modules_scicos */
