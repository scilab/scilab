/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __H5EXCEPTION_HXX__
#define __H5EXCEPTION_HXX__

extern "C"
{
#include "localization.h"
#include "backtrace_print.h"
}

#include "HDF5Objects.h"

#include <cstdlib>
#include <exception>
#include <sstream>
#include <string>
#include <cstdio>
#include <stdarg.h>

#define BUFFER_SIZE 1024

namespace org_modules_hdf5
{
class H5Exception : public std::exception
{
    std::string message;
    std::string file;
    int line;

public :

    H5Exception(const int _line, const char * _file, std::string _message, ...) : message(""), file(_file), line(_line)
    {
        char str[BUFFER_SIZE];
        va_list args;

        va_start(args, _message);
        vsnprintf(str, BUFFER_SIZE, _message.c_str(), args);
        va_end(args);

        message = getDescription(std::string(str));
    }

    H5Exception(const int _line, const char * _file, const char * _message, ...) : message(""), file(_file), line(_line)
    {
        char str[BUFFER_SIZE];
        va_list args;

        va_start(args, _message);
        vsnprintf(str, BUFFER_SIZE, _message, args);
        va_end(args);

        message = getDescription(std::string(str));
    }

    virtual ~H5Exception() throw() { }

    virtual const char * what() const throw()
    {
        return message.c_str();
    }

private:

    static std::string getHDF5ErrorMsg()
    {
        hid_t stid = H5Eget_current_stack();
        if (stid < 0)
        {
            return std::string(_("Cannot get the current stack of errors."));
        }

        ssize_t stackSize = H5Eget_num(stid);
        std::string ret;

        if (stackSize)
        {
            herr_t err = H5Ewalk2(stid, H5E_WALK_UPWARD, getStackErrorMsg, &ret);
            H5Eclear2(stid);
        }

        return ret;
    }

    static herr_t getStackErrorMsg(unsigned int n, const H5E_error2_t * eptr, void * client_data)
    {
        std::string * str = (std::string *)client_data;
        str->append(eptr->desc);

        return -1;
    }

    inline std::string getDescription(std::string m) const
    {
        std::ostringstream os;
        std::string err = getHDF5ErrorMsg();
        if (!err.empty())
        {
            os << m << std::endl
               << _("HDF5 description") << ": " << err << "." << std::flush;

            m = os.str();
            os.str("");
        }

#if defined(__HDF5OBJECTS_DEBUG__)

        if (line == -1)
        {
            return m;
        }

        const char * bt = backtrace_print(0, 1);

        os << m << std::endl
           << "DEBUG Information:" << std::endl
           << gettext("Exception thrown in file") << " " << file << " " << gettext("at line") << " " << line << std::endl
           << bt << std::flush;

        free(const_cast<char *>(bt));

        return os.str();

#else

        return m;

#endif
    }
};
}

#endif // __H5EXCEPTION_HXX__
