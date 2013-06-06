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

#define BUFFER_SIZE 1024

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{
class ScilabJavaException : public std::exception
{

    std::string message;
    std::string file;
    int line;
    //    PyObject * type;
    //    PyObject * value;
    //    PyObject * traceback;

public:

    ScilabJavaException(const int _line, const char * _file, const char * _message, ...) //: message(""), file(_file), line(_line), type(0), value(0), traceback(0)
    {
        char str[BUFFER_SIZE];
        va_list args;

        va_start(args, _message);
        vsnprintf(str, BUFFER_SIZE, _message, args);
        va_end(args);

        message = getDescription(std::string(str));
    }
    /*
        ScilabJavaException(const int _line, const char * _file, std::string _message, ...) : message(""), file(_file), line(_line), type(0), value(0), traceback(0)
        {
            char str[BUFFER_SIZE];
            va_list args;

            va_start(args, _message);
            vsnprintf(str, BUFFER_SIZE, _message.c_str(), args);
            va_end(args);

            message = getDescription(std::string(str));
        }
    */
    /*    ScilabJavaException(PyObject * _type, PyObject * _value, PyObject * _traceback, const char * _message, ...) : message(""), file(""), line(-1), type(_type), value(_value), traceback(_traceback)
        {
            char str[BUFFER_SIZE];
            va_list args;

            va_start(args, _message);
            vsnprintf(str, BUFFER_SIZE, _message, args);
            va_end(args);

            message = getDescription(std::string(str));
        }

        ScilabJavaException(const int _line, const char * _file, PyObject * _type, PyObject * _value, PyObject * _traceback, const char * _message, ...) : message(""), file(_file), line(_line), type(_type), value(_value), traceback(_traceback)
        {
            char str[BUFFER_SIZE];
            va_list args;

            va_start(args, _message);
            vsnprintf(str, BUFFER_SIZE, _message, args);
            va_end(args);

            message = getDescription(std::string(str));
        }

        ScilabJavaException(const int _line, const char * _file, PyObject * _type, PyObject * _value, PyObject * _traceback, std::string _message, ...) : message(""), file(_file), line(_line), type(_type), value(_value), traceback(_traceback)
        {
            char str[BUFFER_SIZE];
            va_list args;

            va_start(args, _message);
            vsnprintf(str, BUFFER_SIZE, _message.c_str(), args);
            va_end(args);

            message = getDescription(std::string(str));
        }
    */

    ~ScilabJavaException() throw() { }

    inline std::string getDescription(std::string m) const
    {
        std::ostringstream os;

        os << m << std::endl;
        /*
                if (traceback || type || value)
                {
                    os << gettext("Java interpreter threw an exception:") << std::endl;

                    if (traceback)
                    {
                        PyTracebackObject * tb = reinterpret_cast<PyTracebackObject *>(traceback);
                        int err = 0;

                        os << gettext("Traceback:") << std::endl;

                        while (tb && err == 0)
                        {
                            os << "  " << gettext("File") << " " << PyString_AsString(tb->tb_frame->f_code->co_filename)
                               << ", " << gettext("line") << " " << tb->tb_lineno << ", " << gettext("in") << " "
                               << PyString_AsString(tb->tb_frame->f_code->co_name) << std::endl;
                            tb = tb->tb_next;
                            err = PyErr_CheckSignals();
                        }

                        Py_DECREF(traceback);
                    }

                    if (type)
                    {
                        os << reinterpret_cast<PyTypeObject *>(type)->tp_name << ": ";
                        Py_DECREF(type);
                    }

                    if (value)
                    {
                        PyObject * _value = PyObject_Str(value);
                        os << PyString_AsString(_value) << std::endl;
                        Py_DECREF(_value);
                        Py_DECREF(value);
                    }

            }
        */
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
