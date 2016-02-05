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

#ifndef __SLINT_CONTEXT_HXX__
#define __SLINT_CONTEXT_HXX__

#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

#include "SciFile.hxx"

namespace slint
{

class SLint;

class SLintContext
{

    const SLint & project;
    std::unordered_map<std::wstring, const ast::FunctionDec *> publicFunctions;
    SciFilePtr currentFile;
    const ast::Exp * LHSExp;
    std::stack<const ast::FunctionDec *> funStack;
    std::stack<const ast::Exp *> loopStack;
    std::unordered_set<std::wstring> funIn;
    std::unordered_set<std::wstring> funOut;

public:

    SLintContext(const SLint & _project);
    ~SLintContext();

    void setSciFile(const SciFilePtr & file);
    const SciFilePtr & getSciFile() const;
    const std::wstring & getFilename() const;
    const ast::Exp * getTree() const;
    unsigned int getCodeLength() const;
    const wchar_t * getCode() const;
    const ast::Exp * getLHSExp() const;
    void setLHSExp(const ast::Exp * _LHSExp);
    void pushFn(const ast::FunctionDec * e);
    const ast::FunctionDec * popFn();
    const ast::FunctionDec * topFn();
    bool isFirstLevelFn() const;
    void pushLoop(const ast::Exp * e);
    const ast::Exp * popLoop();
    const ast::Exp * topLoop();
    const std::unordered_set<std::wstring> & getFunIn() const;
    const std::unordered_set<std::wstring> & getFunOut() const;
    bool isFunIn(const symbol::Symbol & sym) const;
    bool isFunOut(const symbol::Symbol & sym) const;
    bool isFunIn(const std::wstring & name) const;
    bool isFunOut(const std::wstring & name) const;
    const ast::AssignExp * getAssignExp() const;
    bool checkLineLength(const unsigned int max, std::vector<unsigned int> & out) const;
    unsigned int countLines() const;
    unsigned int countLines(const unsigned from, const unsigned to) const;
    bool getPosition(const Location & loc, std::pair<unsigned int, unsigned int> & out) const;
    bool getFromPositionToEOL(const Location & loc, std::pair<unsigned int, unsigned int> & out) const;
    bool isAssignedVar(const ast::SimpleVar & var) const;
    std::wstring getRHSCallName() const;
    bool isPrivateFunction(const symbol::Symbol & sym) const;
    const ast::FunctionDec * getPrivateFunction(const std::wstring & name) const;
    bool isExternPrivateFunction(const symbol::Symbol & sym, std::wstring & name) const;
    void addPublicFunction(const ast::FunctionDec * fundec);
    const ast::FunctionDec * getPublicFunction(const std::wstring & name) const;
    const ast::Exp * getNextRelevantExp() const;

private:

    void getInOut(const ast::FunctionDec * e);
};

} // namespace slint

#endif // __SLINT_CONTEXT_HXX__
