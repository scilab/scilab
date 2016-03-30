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

#ifndef __SLINT_SCILAB_RESULT_HXX__
#define __SLINT_SCILAB_RESULT_HXX__

#include <map>
#include <unordered_map>
#include <vector>

#include "output/SLintResult.hxx"

namespace types
{
class Struct;
}

namespace slint
{

class SLintScilabResult : public SLintResult
{

protected:

    std::unordered_map<std::wstring, std::multimap<Location, std::wstring>> results;

public:

    SLintScilabResult();
    virtual ~SLintScilabResult();
    virtual void handleFiles(const std::vector<SciFilePtr> & files) { }
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg);
    virtual void finalize();
};

class SLintScilabOut : public SLintResult
{

    // { Filename => { Id => { Location, Msg } }
    std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::vector<std::pair<Location, std::wstring>>>> results;

public:

    SLintScilabOut();
    virtual ~SLintScilabOut();
    virtual void handleFiles(const std::vector<SciFilePtr> & files) { }
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg);
    virtual void finalize();

    types::Struct * getStruct() const;
};

} // namespace slint

#endif // __SLINT_SCILAB_RESULT_HXX__
