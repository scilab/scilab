/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __BREAKPOINT_HXX__
#define __BREAKPOINT_HXX__

#include <string>
#include "macro.hxx"
#include "exp.hxx"
#include "tasks.hxx"

namespace debugger
{
struct Breakpoint
{
    Breakpoint(const std::wstring& functionName, int iLine = -1, const std::wstring& condition = L"")
        : _pFunctionName(functionName), _iMacroLine(iLine), _file(L""), _iFileLine(0), _condition(condition), _conditionExp(NULL), enable(true) {}

    ~Breakpoint()
    {
        if (_conditionExp)
        {
            delete _conditionExp;
        }
    };

    void setFile(std::wstring& file)
    {
        _file = file;
    }
    const std::wstring& getFile() const
    {
        return _file;
    }

    void setFunctionName(std::wstring& functionName)
    {
        _pFunctionName = functionName;
    }
    const std::wstring& getFunctioName() const
    {
        return _pFunctionName;
    }

    void setMacroLine(int line)
    {
        _iMacroLine = line;
    }
    int getMacroLine() const
    {
        return _iMacroLine;
    }

    void setFileLine(int line)
    {
        _iFileLine = line;
    }
    int getFileLine() const
    {
        return _iFileLine;
    }

    bool isMacro() const
    {
        return _pFunctionName.empty() == false;
    }

    bool isFile() const
    {
        return isMacro() == false;
    }

    void setEnable()
    {
        enable = true;
    }
    void setDisable()
    {
        enable = false;
    }
    bool isEnable() const
    {
        return enable;
    }

    const std::wstring& getCondition() const
    {
        return _condition;
    }

    ast::Exp* getConditionExp()
    {
        if (_conditionExp)
        {
            return _conditionExp;
        }

        _conditionExp = parseCommand(_condition);
        return _conditionExp;
    }

private:
    std::wstring _pFunctionName;
    int _iMacroLine;
    std::wstring _file;
    int _iFileLine;
    std::wstring _condition;
    ast::Exp* _conditionExp;
    bool enable;
};

typedef std::vector<Breakpoint*> Breakpoints;

}

#endif /* !__BREAKPOINT_HXX__ */
