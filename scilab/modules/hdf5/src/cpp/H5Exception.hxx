/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

    inline std::string getDescription(std::string m) const
    {

#if defined(__HDF5OBJECTS_DEBUG__)

        if (line == -1)
        {
            return m;
        }

        std::ostringstream os;
        const char * bt = backtrace_print(0, 1);

        os << m << std::endl
           << "DEBUG Informations:" << std::endl
           << gettext("Exception thrown in file") << " " << file << " " << gettext("at line") << " " << line << std::endl
           << bt;

        free(const_cast<char *>(bt));
        os.flush();

        return os.str();

#else

        return m;

#endif
    }
};
}

#endif // __H5EXCEPTION_HXX__
