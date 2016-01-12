/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SLINT_RESULT_HXX__
#define __SLINT_RESULT_HXX__

#include <iostream>
#include <string>

#include "location.hxx"
#include "SLintContext.hxx"
#include "SciFile.hxx"

namespace slint
{

class SLintChecker;

class SLintResult
{

public:

    SLintResult() { }
    virtual ~SLintResult() { }

    template<typename... Args>
    void report(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::string & err, Args... args)
    {
        handleMessage(context, loc, checker, make_string(err, args...));
    }

    //template<typename... Args>
    //void report(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::string & err, Args... args)
    //{
    //    char * _err = to_wide_string(err.c_str());
    //    handleMessage(context, loc, checker, make_string(std::string(_err), args...));
    //    FREE(_err);
    //}

    virtual void handleFiles(const std::vector<SciFilePtr> & files) = 0;
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::string & msg) = 0;

    virtual void finalize() { }

private:

    template<typename... Args>
    const std::string make_string(const std::string & err, Args... args)
    {
        std::ostringstream os;
        make_string(os, err.c_str(), args...);
        return os.str();
    }

    void make_string(std::ostringstream & out, const char * p)
    {
        out << p;
    }

    template<typename T, typename... Args>
    void make_string(std::ostringstream & out, const char* p, const T & value, Args... args)
    {
        while (*p)
        {
            if (*p == '%' && *(++p) != '%')
            {
                out << value;
                ++p;
                make_string(out, p, args...);
                return;
            }
            out << *p++;
        }
    }
};

} // namespace slint

#endif // __SLINT_RESULT_HXX__
