/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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

/*--------------------------------------------------------------------------*/
#include "SetConsolePrompt.hxx"
#include "CallScilabBridge.hxx"
/*--------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge;
BOOL SetConsolePrompt(const char *Sci_prompt)
{
    JavaVM* vm = getScilabJavaVM();

    // The prompt is also set by threads Java does not know, as the one of a
    // debug adapter. The call attaches them: left attached, a thread that
    // outlives the interpreter keeps the Java machine from stopping at exit.
    JNIEnv* env = nullptr;
    const bool wasDetached = vm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_EDETACHED;

    CallScilabBridge::setPrompt(vm, Sci_prompt);

    if (wasDetached)
    {
        vm->DetachCurrentThread();
    }

    return TRUE;
}
