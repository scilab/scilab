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
