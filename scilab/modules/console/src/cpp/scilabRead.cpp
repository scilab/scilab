/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#include <algorithm>
#include "configvariable.hxx"
#include "threadmanagement.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "scilabRead.h"
#include "SetConsolePrompt.h"
#include "prompt.h"
#include "TermReadAndProcess.h"
#include "os_string.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "diary.h"
}

static SCILAB_INPUT_METHOD _reader;

void setScilabInputMethod(SCILAB_INPUT_METHOD reader)
{
    _reader = reader;
}

void C2F(scilabread)(char* strRead, int len)
{
    scilabRead();
    char* str = ConfigVariable::getConsoleReadStr();
    int size = std::min(static_cast<int>(strlen(str)), len - 1);
    strncpy(strRead, str, size);
    strRead[size] = '\0';
    FREE(str);
}

int scilabRead()
{
    ThreadManagement::LockScilabRead();
    if (getScilabMode() == SCILAB_STD)
    {
        /* Send new prompt to Java Console, do not display it */
        if (GetTemporaryPrompt() != NULL)
        {
            SetConsolePrompt(GetTemporaryPrompt());
        }
        else
        {
            char pstCurrentPrompt[PROMPT_SIZE_MAX];
            GetCurrentPrompt(pstCurrentPrompt);
            SetConsolePrompt(pstCurrentPrompt);
        }
    }

    //call reader
    char* pstTemp = (*_reader)();

    //add prompt to diary
    static char pstPrompt[PROMPT_SIZE_MAX];
    GetCurrentPrompt(pstPrompt);
    wchar_t* pwstPrompt = to_wide_string(pstPrompt);
    diaryWrite(pwstPrompt, TRUE);
    FREE(pwstPrompt);

    //add input to diary
    wchar_t* pwstIn = to_wide_string(pstTemp);
    diaryWriteln(pwstIn, TRUE);
    FREE(pwstIn);

    ConfigVariable::setConsoleReadStr(pstTemp);
    int isSciCmd = ConfigVariable::isScilabCommand();
    ThreadManagement::UnlockScilabRead();

    return isSciCmd;
}
