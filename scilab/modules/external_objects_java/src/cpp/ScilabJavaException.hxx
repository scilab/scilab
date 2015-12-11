#ifndef __SCILABJAVAEXCEPTION_HXX__
#define __SCILABJAVAEXCEPTION_HXX__

extern "C" {
#include "localization.h"
#include "backtrace_print.h"
}

#include "ExternalObjects.h"

#include <jni.h>

#include <exception>
#include <sstream>
#include <string>

#define EOJ_EXCEPTION_BUFFER_SIZE 4096

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{
class ScilabJavaException : public std::exception
{

    std::string message;
    std::string file;
    int line;

public:

    ScilabJavaException(const int _line, const char * _file, const char * _message, ...) : message(""), file(_file), line(_line)
    {
        char str[EOJ_EXCEPTION_BUFFER_SIZE];
        va_list args;

        va_start(args, _message);
        vsnprintf(str, EOJ_EXCEPTION_BUFFER_SIZE, _message, args);
        va_end(args);

        message = getDescription(std::string(str));
    }

    ScilabJavaException(const int _line, const char * _file, std::string _message, ...) : message(""), file(_file), line(_line)
    {
        char str[EOJ_EXCEPTION_BUFFER_SIZE];
        va_list args;

        va_start(args, _message);
        vsnprintf(str, EOJ_EXCEPTION_BUFFER_SIZE, _message.c_str(), args);
        va_end(args);

        message = getDescription(std::string(str));
    }

    ~ScilabJavaException() throw() { }

    inline std::string getDescription(std::string m) const
    {
        std::ostringstream os;

        os << m << std::endl;

#if defined(EODEBUG)

        if (line != -1)
        {
            const char * bt = backtrace_print(0, 1);

            os << std::endl
               << "DEBUG Informations:" << std::endl
               << gettext("Exception thrown in file") << " " << file << " " << gettext("at line") << " " << line << std::endl
               << bt;

            free(const_cast<char *>(bt));
        }

#endif

        os.flush();

        return os.str();
    }

    virtual const char * what() const throw()
    {
        return message.c_str();
    }
};
}

#endif // __SCILABJAVAEXCEPTION_HXX__
