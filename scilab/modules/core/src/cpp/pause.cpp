/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

extern "C" {
#include "pause.h"
}

void pause(void)
{
    ConfigVariable::IncreasePauseLevel();

    //return to console so change mode to 2
    int iOldMode = ConfigVariable::getPromptMode();
    ConfigVariable::setPromptMode(2);

    // unlock console thread to display prompt again
    ThreadManagement::SendConsoleExecDoneSignal();

    int iPauseLevel = ConfigVariable::getPauseLevel();
    while (ConfigVariable::getPauseLevel() == iPauseLevel)
    {
        ThreadManagement::SendAwakeRunnerSignal();
        ThreadManagement::WaitForRunMeSignal();
        if(StaticRunner::isRunnerAvailable())
        {
            StaticRunner::launch();
        }
    }

    //return from console so change mode to initial
    ConfigVariable::setPromptMode(iOldMode);
}