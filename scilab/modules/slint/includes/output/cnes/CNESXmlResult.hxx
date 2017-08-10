/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SLINT_CNES_XML_RESULT_HXX__
#define __SLINT_CNES_XML_RESULT_HXX__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "config/cnes/ToolConfiguration.hxx"
#include "output/SLintResult.hxx"

namespace types
{
class String;
}

namespace slint
{

class SciFile;

namespace CNES
{

class CNESXmlResult : public SLintResult
{

    struct __Info
    {
        const Location loc;
        const std::wstring msg;
        const std::wstring funName;

        __Info(const Location & _loc, const std::wstring & _msg, const std::wstring & _funName) : loc(_loc), msg(_msg), funName(_funName) { }
    };

    const ToolConfigurationType tct;
    const std::wstring path;
    std::ofstream * out;
    SciFilePtr current;
    std::unordered_map<std::wstring, std::vector<__Info>> res;

public:

    CNESXmlResult(const ToolConfiguration & tc, types::String * conf, const std::wstring & id, const std::wstring & _path);

    virtual ~CNESXmlResult();
    virtual void handleFiles(const std::vector<SciFilePtr> & files);
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg);
    virtual void finalize();

private:

    void printRes();
};

} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_XML_RESULT_HXX__
