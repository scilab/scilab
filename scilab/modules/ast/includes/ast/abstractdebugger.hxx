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
    virtual void onErrorInFile(const std::string& filemane) = 0;//when an error is trigger in a file
    virtual void onErrorInScript(const std::string& funcname) = 0;//when an error is trigger in a script
    virtual void onQuit() = 0; //when debugger is killed or replace by another one

    virtual void updateBreakpoints() = 0;

};

typedef std::vector<AbstractDebugger*> Debuggers;
}
#endif /* !__ABSTRACT_DEBUGGER_HXX__ */
