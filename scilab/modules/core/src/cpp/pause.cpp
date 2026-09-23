/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2019-2019 - ESI Group - Cedric Delamarre
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "configvariable.hxx"
#include "threadmanagement.hxx"
#include "runner.hxx"
#include "runtimeaccess.hxx"

extern "C" {
#include "pause.h"
}

void pause_interpreter(void)
{
    ConfigVariable::IncreasePauseLevel();

    // allow error when paused in a try
    bool bSilentError = ConfigVariable::isSilentError();
    ConfigVariable::setSilentError(false);

    //return to console so change mode to 2
    int iOldMode = ConfigVariable::getPromptMode();
    ConfigVariable::setPromptMode(2);

    // unlock console thread to display prompt again
    ThreadManagement::SendConsoleExecDoneSignal();

    // The runtime context is stable while the interpreter is paused. Allow
    // editor services to refresh it, then take exclusive ownership back when
    // the paused execution resumes.
    {
        RuntimeUnlockGuard runtimeUnlockGuard;

        int iPauseLevel = ConfigVariable::getPauseLevel();
        while (ConfigVariable::getPauseLevel() == iPauseLevel)
        {
            StaticRunner::launch();
        }
    }

    //return from console so change mode to initial
    ConfigVariable::setPromptMode(iOldMode);

    // restore try/catch flag
    ConfigVariable::setSilentError(bSilentError);
}
