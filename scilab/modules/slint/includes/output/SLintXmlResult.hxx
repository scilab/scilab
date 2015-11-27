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
