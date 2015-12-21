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

#ifndef __CONSOLE_DEBUGGER_HXX__
#define __CONSOLE_DEBUGGER_HXX__

#include "abstractdebugger.hxx"

extern "C"
{
#include "sciprint.h"
#include "charEncoding.h"
#include "dynlib_ast.h"
}

namespace debugger
{
class EXTERN_AST ConsoleDebugger : public AbstractDebugger
{

    void printExp();
public:

    ConsoleDebugger() {}
    virtual ~ConsoleDebugger() {}

    virtual void onStop(int index);
    virtual void onResume();
    virtual void onAbort();
    virtual void onErrorInFile(const std::wstring& filemane);
    virtual void onErrorInScript(const std::wstring& funcname);
    virtual void onQuit();
    virtual void updateBreakpoints();
};
}

#endif /* !__CONSOLE_DEBUGGER_HXX__ */
