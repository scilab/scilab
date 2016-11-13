/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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
        handleMessage(context, loc, checker, 0, make_string(err, args...));
    }

    template<typename... Args>
    void report(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::string & err, Args... args)
    {
        handleMessage(context, loc, checker, sub, make_string(err, args...));
    }

    virtual void handleFiles(const std::vector<SciFilePtr> & files) = 0;
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::string & msg) = 0;

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
