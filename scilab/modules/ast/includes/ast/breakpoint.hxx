/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    //Breakpoint(std::wstring& file, int iLine = -1, ast::Exp* condition = NULL)
    //    : _pFunctionName(NULL), _iMacroLine(iLine), _file(file), _condition(condition) {}

    ~Breakpoint()
    {
        if(_conditionExp)
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
    std::wstring _file;
    std::wstring _condition;
    ast::Exp* _conditionExp;
    int _iFileLine;
    int _iMacroLine;
    bool enable;
};

typedef std::vector<Breakpoint*> Breakpoints;

}

#endif /* !__BREAKPOINT_HXX__ */
