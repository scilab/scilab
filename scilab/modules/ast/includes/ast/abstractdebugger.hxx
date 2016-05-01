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

#ifndef __ABSTRACT_DEBUGGER_HXX__
#define __ABSTRACT_DEBUGGER_HXX__

#include <string>
#include <vector>

extern "C"
{
#include "dynlib_ast.h"
}


namespace debugger
{
class EXTERN_AST AbstractDebugger
{
public:
    AbstractDebugger() {}
    virtual ~AbstractDebugger() {}

    //interface
    virtual void onStop(int index) = 0; //when a bp is trigger
    virtual void onResume() = 0;//when "play" is trigger
    virtual void onAbort() = 0;//when execution is abort
    virtual void onErrorInFile(const std::wstring& filemane) = 0;//when an error is trigger in a file
    virtual void onErrorInScript(const std::wstring& funcname) = 0;//when an error is trigger in a script
    virtual void onQuit() = 0; //when debugger is killed or replace by another one

    virtual void updateBreakpoints() = 0;

};

typedef std::vector<AbstractDebugger*> Debuggers;
}
#endif /* !__ABSTRACT_DEBUGGER_HXX__ */
