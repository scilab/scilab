/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2019-2019 - ESI Group - Cedric Delamarre
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric Delamarre
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <string>

#include "configvariable.hxx"
#include "debugmanager.hxx"
#include "threadmanagement.hxx"
#include "runner.hxx"
#include "runtimeaccess.hxx"

extern "C" {
#include "pause.h"
#include "configvariable_interface.h"
#include "prompt.h"
#include "SetConsolePrompt.h"
#include "BrowseVarManager.h"
}

// Shows in place the prompt of the pause level, and the one found again when it is left.
class PausePromptGuard
{
public:
    PausePromptGuard()
    {
        RefreshConsolePrompt();
    }

    // An abort leaves every pause level, one guard after the other.
    ~PausePromptGuard()
    {
        RefreshConsolePrompt();
    }
};

/*
 * The objects to destroy when an abort leaves the pause live here, not in
 * pause_interpreter(): that one is extern "C", and under /EHsc MSVC does not
 * run the destructors of an extern "C" frame an exception goes through.
 */
static void runPauseLevel()
{
    // Before the console is released: from then on it redraws its prompt.
    PausePromptGuard pausePromptGuard;

    // The variables of the scope the execution waits in: the browser is
    // otherwise refreshed when a command ends only.
    if (getScilabMode() != SCILAB_NWNI)
    {
        UpdateBrowseVar();
    }

    // unlock console thread to display prompt again
    ThreadManagement::SendConsoleExecDoneSignal();

    // The runtime context is stable while the interpreter is paused. Allow
    // editor services to refresh it, then take exclusive ownership back when
    // the paused execution resumes.
    RuntimeUnlockGuard runtimeUnlockGuard;

    // A pause outside debug mode leaves Scilab at a prompt, free to run what
    // is not debugged: debuggers are told so, then told the execution goes on.
    // A stop of the debugger is announced by the debugger itself.
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    const bool userPause = debugger::DebuggerManager::isDebugging() == false;
    if (userPause)
    {
        manager->sendExecutionReleased();
    }

    int iPauseLevel = ConfigVariable::getPauseLevel();
    while (ConfigVariable::getPauseLevel() == iPauseLevel)
    {
        StaticRunner::launch();
    }

    if (userPause)
    {
        manager->sendExecution();
    }
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

    runPauseLevel();

    //return from console so change mode to initial
    ConfigVariable::setPromptMode(iOldMode);

    // restore try/catch flag
    ConfigVariable::setSilentError(bSilentError);
}
