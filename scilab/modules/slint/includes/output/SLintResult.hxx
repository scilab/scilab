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
    void report(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::wstring & err, Args... args)
    {
        handleMessage(context, loc, checker, 0, make_string(err, args...));
    }

    template<typename... Args>
    void report(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::string & err, Args... args)
    {
        wchar_t * _err = to_wide_string(err.c_str());
        handleMessage(context, loc, checker, 0, make_string(std::wstring(_err), args...));
        FREE(_err);
    }

    template<typename... Args>
    void report(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & err, Args... args)
    {
        handleMessage(context, loc, checker, sub, make_string(err, args...));
    }

    template<typename... Args>
    void report(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::string & err, Args... args)
    {
        wchar_t * _err = to_wide_string(err.c_str());
        handleMessage(context, loc, checker, sub, make_string(std::wstring(_err), args...));
        FREE(_err);
    }

    virtual void handleFiles(const std::vector<SciFilePtr> & files) = 0;
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg) = 0;

    virtual void finalize() { }

private:

    template<typename... Args>
    const std::wstring make_string(const std::wstring & err, Args... args)
    {
        std::wostringstream wos;
        make_string(wos, err.c_str(), args...);
        return wos.str();
    }

    void make_string(std::wostringstream & out, const wchar_t * p)
    {
        out << p;
    }

    template<typename T, typename... Args>
    void make_string(std::wostringstream & out, const wchar_t * p, const T & value, Args... args)
    {
        while (*p)
        {
            if (*p == L'%' && *(++p) != L'%')
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
