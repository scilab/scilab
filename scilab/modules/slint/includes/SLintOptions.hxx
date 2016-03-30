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

#ifndef __SLINT_OPTIONS_HXX__
#define __SLINT_OPTIONS_HXX__

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "exp.hxx"
#include "SLintContext.hxx"
#include "checkers/SLintChecker.hxx"

namespace slint
{

class SLintOptions
{

    struct __Hasher
    {
        inline std::size_t operator()(const ast::Exp::ExpType type) const
        {
            return std::hash<unsigned char>()((unsigned char)type);
        }
    };

public:

    typedef std::unordered_multimap<ast::Exp::ExpType, std::shared_ptr<SLintChecker>, __Hasher> MapCheckers;
    typedef MapCheckers::value_type value_type;
    typedef MapCheckers::iterator iterator;
    typedef MapCheckers::const_iterator const_iterator;
    typedef std::pair<MapCheckers::iterator, MapCheckers::iterator> range;
    typedef std::pair<MapCheckers::const_iterator, MapCheckers::const_iterator> const_range;
    typedef std::vector<std::shared_ptr<SLintChecker>> FileCheckers;

    FileCheckers fileCheckers;

private:

    MapCheckers checkers;
    std::unordered_set<std::wstring> excludedFiles;
    std::wstring id;

public:

    SLintOptions();

    void addDefault(SLintChecker * checker);
    MapCheckers & getCheckers();
    FileCheckers & getFileCheckers();
    void addExcludedFile(const std::string & filename);
    bool isExcluded(const std::wstring & filename) const;
    void setId(const std::string & _id);
    const std::wstring & getId() const;

    template<typename... Args>
    void add(SLintChecker * checker, Args... args)
    {
        std::shared_ptr<SLintChecker> _checker(checker);
        add(_checker, args...);
    }

private:

    template<typename... Args>
    void add(std::shared_ptr<SLintChecker> & checker, ast::Exp::ExpType type, Args... args)
    {
        checkers.emplace(type, checker);
        add(checker, args...);
    }

    void add(std::shared_ptr<SLintChecker> & checker, ast::Exp::ExpType type)
    {
        checkers.emplace(type, checker);
    }

};

} // namespace slint

#endif // __SLINT_OPTIONS_HXX__

