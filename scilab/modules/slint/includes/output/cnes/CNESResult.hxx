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

#ifndef __SLINT_CNES_RESULT_HXX__
#define __SLINT_CNES_RESULT_HXX__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

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

class CNESResult : public SLintResult
{
    const std::wstring path;
    std::ofstream * out;
    SciFilePtr current;
    std::unordered_map<std::wstring, std::vector<std::pair<Location, std::wstring>>> res;

public:

    CNESResult(types::String * conf, const std::wstring & id, const std::wstring & _path);

    virtual ~CNESResult();
    virtual void handleFiles(const std::vector<SciFilePtr> & files);
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::wstring & msg);
    virtual void finalize();

private:

    void printRes();
};

} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_RESULT_HXX__
