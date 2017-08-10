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

#ifndef __SLINT_XML_RESULT_HXX__
#define __SLINT_XML_RESULT_HXX__

#include <fstream>
#include <string>

#include "output/SLintResult.hxx"

namespace slint
{

class SciFile;

class SLintXmlResult : public SLintResult
{
    const std::wstring path;
    std::ofstream * out;
    SciFilePtr current;

public:

    SLintXmlResult(const std::wstring & _path);

    virtual ~SLintXmlResult();
    virtual void handleFiles(const std::vector<SciFilePtr> & files) { }
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg);
    virtual void finalize();

    static std::wstring replaceByEntities(const std::wstring & seq);
    static const std::string getStr(const std::wstring & str);

private:

    void print(const SciFilePtr & file);
    void print(const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg);
    void print(const Location & loc);
    void print(const SLintChecker & checker, const unsigned sub);
    void print(const std::wstring & msg);

    inline static void pushEntity(std::vector<wchar_t> & buf, const wchar_t * wstr, const unsigned char len)
    {
        for (unsigned int i = 0; i < len; ++i)
        {
            buf.push_back(wstr[i]);
        }
    }
};

} // namespace slint

#endif // __SLINT_XML_RESULT_HXX__
