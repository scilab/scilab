/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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

#ifndef LOGGERVIEW_HXX_
#define LOGGERVIEW_HXX_

#include <cwchar>
#include <string>
#include <sstream>

#include "View.hxx"
#include "utilities.hxx"

namespace org_scilab_modules_scicos
{

enum LogLevel
{
    LOG_UNDEF = -1,   //!< Undefined value
    LOG_TRACE = 0,    //!< more detailed information. Expect these to be written to logs only.
    LOG_DEBUG = 1,    //!< detailed information on the flow through the system. Expect these to be written to logs only.
    LOG_INFO = 2,    //!< Interesting runtime events (startup/shutdown). Expect these to be immediately visible on a console, so be conservative and keep to a minimum.
    LOG_WARNING = 3,    //!<
    LOG_ERROR = 4,    //!< Other runtime errors or unexpected conditions. Expect these to be immediately visible on a status console.
    LOG_FATAL = 5,    //!< Severe errors that cause premature termination. Expect these to be immediately visible on a status console.
};

class LoggerView: public View
{
public:
    LoggerView();
    ~LoggerView();

    /*
     * Implement a classical Logger interface
     */

    static enum LogLevel indexOf(const wchar_t* name);
    static const wchar_t* toString(enum LogLevel level);
    static const char* toDisplay(enum LogLevel level);

    enum LogLevel getLevel() const
    {
        return m_level;
    }
    void setLevel(enum LogLevel level)
    {
        this->m_level = level;
    }

    void log(enum LogLevel level, const std::stringstream& msg);
    void log(enum LogLevel level, const std::string& msg);
    void log(enum LogLevel level, const char* msg, ...);
    void log(enum LogLevel level, const wchar_t* msg, ...);

    /*
     * Implement the Logger as a View
     */

    void objectCreated(const ScicosID& uid, kind_t k);
    void objectReferenced(const ScicosID& uid, kind_t k, unsigned refCount);
    void objectUnreferenced(const ScicosID& uid, kind_t k, unsigned refCount);
    void objectDeleted(const ScicosID& uid, kind_t k);
    void propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p, update_status_t u);

private:
    enum LogLevel m_level;
};

} /* namespace org_scilab_modules_scicos */

#endif /* LOGGERVIEW_HXX_ */
