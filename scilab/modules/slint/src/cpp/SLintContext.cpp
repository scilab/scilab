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

#include "SciFile.hxx"
#include "SLint.hxx"
#include "SLintContext.hxx"

namespace slint
{

SLintContext::SLintContext(const SLint & _project) : project(_project), LHSExp(nullptr) { }

SLintContext::~SLintContext()
{
}

void SLintContext::setSciFile(const SciFilePtr & file)
{
    currentFile = file;
}

const SciFilePtr & SLintContext::getSciFile() const
{
    return currentFile;
}

const std::wstring & SLintContext::getFilename() const
{
    return currentFile->getFilename();
}

const ast::Exp * SLintContext::getTree() const
{
    return currentFile->getTree();
}

unsigned int SLintContext::getCodeLength() const
{
    return currentFile->getCodeLength();
}

const wchar_t * SLintContext::getCode() const
{
    return currentFile->getCode();
}

const ast::Exp * SLintContext::getLHSExp() const
{
    return LHSExp;
}

void SLintContext::setLHSExp(const ast::Exp * _LHSExp)
{
    LHSExp = _LHSExp;
}

void SLintContext::pushFn(const ast::FunctionDec * e)
{
    funStack.push(e);
    getInOut(e);
}

const ast::FunctionDec * SLintContext::popFn()
{
    if (funStack.empty())
    {
        return nullptr;
    }

    const ast::FunctionDec * e = funStack.top();
    funStack.pop();
    if (!funStack.empty())
    {
        getInOut(funStack.top());
    }
    else
    {
        funIn.clear();
        funOut.clear();
    }

    return e;
}

const ast::FunctionDec * SLintContext::topFn()
{
    if (funStack.empty())
    {
        return nullptr;
    }

    return funStack.top();
}

bool SLintContext::isFirstLevelFn() const
{
    return funStack.size() == 1;
}

void SLintContext::pushLoop(const ast::Exp * e)
{
    loopStack.push(e);
}

const ast::Exp * SLintContext::popLoop()
{
    if (loopStack.empty())
    {
        return nullptr;
    }

    const ast::Exp * e = loopStack.top();
    loopStack.pop();

    return e;
}

const ast::Exp * SLintContext::topLoop()
{
    if (loopStack.empty())
    {
        return nullptr;
    }

    return loopStack.top();
}

const std::unordered_set<std::wstring> & SLintContext::getFunIn() const
{
    return funIn;
}

const std::unordered_set<std::wstring> & SLintContext::getFunOut() const
{
    return funOut;
}

bool SLintContext::isFunIn(const symbol::Symbol & sym) const
{
    return isFunIn(sym.getName());
}

bool SLintContext::isFunOut(const symbol::Symbol & sym) const
{
    return isFunOut(sym.getName());
}

bool SLintContext::isFunIn(const std::wstring & name) const
{
    return funIn.find(name) != funIn.end();
}

bool SLintContext::isFunOut(const std::wstring & name) const
{
    return funOut.find(name) != funOut.end();
}

const ast::AssignExp * SLintContext::getAssignExp() const
{
    if (LHSExp)
    {
        return static_cast<const ast::AssignExp *>(LHSExp->getParent());
    }
    return nullptr;
}

bool SLintContext::getPosition(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    return currentFile->getPosition(loc, out);
}

bool SLintContext::getFromPositionToEOL(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    return currentFile->getFromPositionToEOL(loc, out);
}

bool SLintContext::checkLineLength(const unsigned int max, std::vector<unsigned int> & out) const
{
    return currentFile->checkLineLength(max, out);
}

unsigned int SLintContext::countLines() const
{
    return currentFile->countLines();
}

unsigned int SLintContext::countLines(const unsigned from, const unsigned to) const
{
    return currentFile->countLines(from, to);
}

bool SLintContext::isAssignedVar(const ast::SimpleVar & var) const
{
    if (getLHSExp())
    {
        if (&var == getLHSExp())
        {
            // a = ...
            return true;
        }
        else if (var.getParent() == getLHSExp())
        {
            ast::Exp * parent = var.getParent();
            switch (parent->getType())
            {
                case ast::Exp::CALLEXP:
                {
                    ast::CallExp * ce = static_cast<ast::CallExp *>(parent);
                    if (&ce->getName() == &var)
                    {
                        // a(...) = ...
                        return true;
                    }
                    break;
                }
                case ast::Exp::CELLCALLEXP:
                {
                    ast::CellCallExp * cce = static_cast<ast::CellCallExp *>(parent);
                    if (&cce->getName() == &var)
                    {
                        // a{...} = ...
                        return true;
                    }
                    break;
                }
                case ast::Exp::FIELDEXP:
                {
                    ast::FieldExp * fe = static_cast<ast::FieldExp *>(parent);
                    if (fe->getHead() == &var)
                    {
                        // a.foo = ...
                        return true;
                    }
                    break;
                }
                case ast::Exp::ASSIGNLISTEXP:
                {
                    // [..., a, ...] = ...
                    return true;
                }
                default:
                {
                    return false;
                }
            }
        }
    }
    return false;
}

std::wstring SLintContext::getRHSCallName() const
{
    if (LHSExp)
    {
        const ast::AssignExp & ae = *getAssignExp();
        if (ae.getRightExp().isCallExp())
        {
            const ast::CallExp & ce = static_cast<const ast::CallExp &>(ae.getRightExp());
            if (ce.getName().isSimpleVar())
            {
                return static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            }
        }
    }
    return L"";
}

void SLintContext::getInOut(const ast::FunctionDec * e)
{
    const ast::exps_t & args = e->getArgs().getVars();
    funIn.clear();
    for (const auto arg : args)
    {
        funIn.emplace(static_cast<const ast::SimpleVar *>(arg)->getSymbol().getName());
    }
    const ast::exps_t & rets = e->getReturns().getVars();
    funOut.clear();
    for (const auto ret : rets)
    {
        funOut.emplace(static_cast<const ast::SimpleVar *>(ret)->getSymbol().getName());
    }
}

bool SLintContext::isPrivateFunction(const symbol::Symbol & sym) const
{
    return currentFile->isPrivateFunction(sym);
}

const ast::FunctionDec * SLintContext::getPrivateFunction(const std::wstring & name) const
{
    return currentFile->getPrivateFunction(name);
}

bool SLintContext::isExternPrivateFunction(const symbol::Symbol & sym, std::wstring & name) const
{
    const std::vector<SciFilePtr> & files = project.getFiles();
    for (const auto & file : files)
    {
        if (currentFile != file && file->isPrivateFunction(sym))
        {
            name = file->getFilename();
            return true;
        }
    }
    return false;
}

void SLintContext::addPublicFunction(const ast::FunctionDec * fundec)
{
    if (fundec)
    {
        publicFunctions[fundec->getSymbol().getName()] = fundec;
    }
}

const ast::FunctionDec * SLintContext::getPublicFunction(const std::wstring & name) const
{
    auto i = publicFunctions.find(name);
    if (i == publicFunctions.end())
    {
        return nullptr;
    }
    return i->second;
}

const ast::Exp * SLintContext::getNextRelevantExp() const
{
    return project.getVisitor().getNextRelevantExp();
}
}
